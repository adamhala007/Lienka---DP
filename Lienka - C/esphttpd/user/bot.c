#include "bot.h"
#include "osapi.h"
#include "hw.h"
#include "feedback.h"
#include "music.h"
//#include <esp8266.h> //!

#define ADC_MIN 639
#define ORIGINAL_BOT 0 // 1 if protoype, 0 if original

#define WheelDiam 50
#define WheelDist 64
#define WheelCirc ((WheelDiam)*31415/10000)
#define WheelDistCirc ((WheelDist)*31415/10000)
#define Tile 150
#define Turn90 (WheelDistCirc/4)
#define MotorStepsPerRevolution (32*64) // 4-step
#define StepsTile (Tile*MotorStepsPerRevolution/WheelCirc)
#define Steps90 (Turn90*MotorStepsPerRevolution/WheelCirc)
#define MotorTimeTile (StepsTile*1000/MotorHz)
#define MotorTime90 (Steps90*1000/MotorHz)

PROG(ProgFwd, CMDS({
    {Leds, 3},
    {MotorFwd, MotorTimeTile},
    {Leds, 0},
    {Sound, 440},
    {Sleep, 200},
    {NoSound},
}));
PROG(ProgBack, CMDS({
    {Leds, 3},
    {MotorBack, MotorTimeTile},
    {Leds, 0},
    {Sound, 440},
    {Sleep, 200},
    {NoSound},
}));
PROG(ProgLeft, CMDS({
    {Leds, 2},
    {MotorLeft, MotorTime90},
    {Leds, 0},
    {Sound, 440},
    {Sleep, 200},
    {NoSound},
}));
PROG(ProgRight, CMDS({
    {Leds, 1},
    {MotorRight, MotorTime90},
    {Leds, 0},
    {Sound, 440},
    {Sleep, 200},
    {NoSound},
}));
PROG(ProgWait, CMDS({
    {Leds, 3},
    {Sleep, 1000},
    {Sound, 440},
    {Sleep, 200},
    {NoSound},
}));

#define ProgSound Scale

//#define BUTTONS_SCHEMA 1

/*#ifdef BUTTONS_SCHEMA
// schema
#define BTN_LEFT BIT(0)
#define BTN_FORWARD BIT(1)
#define BTN_STARTSTOP BIT(2)
#define BTN_BACK BIT(3)
#define BTN_RIGHT BIT(4)
#define BTN_OP1 BIT(5)
#define BTN_OP2 BIT(6)
#define BTN_CLEAR BIT(7)
#else
// prototype
#define BTN_FORWARD BIT(0)
#define BTN_LEFT BIT(1)
#define BTN_STARTSTOP BIT(2)
#define BTN_RIGHT BIT(3)
#define BTN_BACK BIT(4)
#define BTN_OP1 BIT(5)
#define BTN_OP2 BIT(6)
#define BTN_CLEAR BIT(7)
#endif*/

static ETSTimer testTimer3;

const char ICACHE_FLASH_ATTR
*btnStr(uint8 btn)
{
    switch(btn) {
        case BTN_STARTSTOP: return "Start/Stop";
        case BTN_CLEAR: return "Clear";
        case BTN_RIGHT: return "Right";
        case BTN_FORWARD: return "Forward";
        case BTN_LEFT: return "Left";
        case BTN_BACK: return "Back";
        case BTN_OP1: return "Op1";
        case BTN_OP2: return "Op2";
        default: return "UKNOWN";
    }
}

static int dx[] = {  0,  1,  0, -1};
static int dy[] = {  1,  0, -1,  0};

void bot_clearProgram(Bot *bot);

void  ICACHE_FLASH_ATTR
bot_step(void *arg)
{
    Bot *bot = arg;
//    os_printf("%s: bot %p\n", __FUNCTION__, bot);

    if (++bot->pc >= bot->length) {
        os_printf("==> Finished program\n");
        bot->pc = -1;
        perform_start(&bot->p, &FeedbackAck2);
        return;
    }

    uint8 instr = bot->program[bot->pc];
    os_printf("===> Start %s\n", btnStr(instr));
    switch(instr) {
        case BTN_RIGHT:
            bot->dir = (bot->dir + 1) % 4;
            perform_startCb(&bot->p, &ProgRight, bot_step, bot);
            break;
        case BTN_LEFT:
            bot->dir = (bot->dir + 3) % 4;
            perform_startCb(&bot->p, &ProgLeft, bot_step, bot);
            break;
        case BTN_FORWARD:
            bot->x += dx[bot->dir];
            bot->y += dy[bot->dir];
            perform_startCb(&bot->p, &ProgFwd, bot_step, bot);
            break;
        case BTN_BACK:
            bot->x -= dx[bot->dir];
            bot->y -= dy[bot->dir];
            perform_startCb(&bot->p, &ProgBack, bot_step, bot);
            break;
        case BTN_OP1:
            perform_startCb(&bot->p, &ProgWait, bot_step, bot);
            break;
        case BTN_OP2:
            {
                int song = ((bot->y + 5*1000) % 5) + 5 * bot->x;
                song %= NUM_SONGS;
                while (song < 0) {
                    song += NUM_SONGS;
                }
                os_printf("Playing song %d at [%d, %d]\n", song, bot->x, bot->y);
                perform_startCb(&bot->p, AllSongs[song], bot_step, bot);
            }
            break;
        default:
        {
            os_printf("%s: unknown instruction %x, terminating program.", __FUNCTION__,
                    instr);
        }
    }
}

bool ICACHE_FLASH_ATTR
bot_isRunning(Bot *bot)
{
    return bot->pc >= 0;
}

bool ICACHE_FLASH_ATTR
bot_isEmptyProgram(Bot *bot)
{
    return !bot->length;
}

void ICACHE_FLASH_ATTR
bot_stopProgram(Bot *bot)
{
    if (!bot_isRunning(bot)) {
        return;
    }
    os_printf("==> Stopping program\n");
    perform_stop(&bot->p);
    hw_allOff();
    bot->pc = -1;
}

void ICACHE_FLASH_ATTR
bot_startProgram(Bot *bot)
{
    bot_stopProgram(bot);
    int i;
    os_printf("==> Starting program\n");
    for (i=0; i<bot->length; ++i) {
        os_printf("%3d: %s\n", i, btnStr(bot->program[i]));
    }

    bot->x = 0;
    bot->y = 0;
    bot->dir = 0;

    bot_step(bot);
}

void ICACHE_FLASH_ATTR
bot_clearProgram(Bot *bot)
{
    bot_stopProgram(bot);
    bot->length = 0;
}

void ICACHE_FLASH_ATTR
bot_playAllSongs(Bot *bot)
{
    if (bot->currentSong >= NUM_SONGS) {
        return;
    }
    perform_startCb(&bot->p, AllSongs[bot->currentSong++], (FinishFunc)bot_playAllSongs, bot);
}

void ICACHE_FLASH_ATTR
bot_handleMusicBoxInput(Bot *bot, int btn, int value)
{
    if (!value) {
        return;
    }

    // why not ;)
    // (BTN_CLEAR is msb)
    uint8 song = (hw_btnState() & 0x7f);

    if (song == 0b11111) {
        bot->currentSong = 0;
        bot_playAllSongs(bot);
    }

    if (song) {
        song = (song - 1) % NUM_SONGS;
        os_printf("Playing song #%d\n", song + 1);
        perform_start(&bot->p, AllSongs[song]);
    }
}

// nova funkcia, sprav 1 step

void ICACHE_FLASH_ATTR
bot_handleNormalInput(Bot *bot, int btn, int value)
{
    if (!value) {
        return;
    }
    
    switch (btn) {
        case BTN_STARTSTOP:
            if (bot_isRunning(bot)) {
                bot_stopProgram(bot);
                perform_start(&bot->p, &FeedbackErr);
            }
            else if (bot_isEmptyProgram(bot)) {
                os_printf("==> Empty program\n");
                perform_start(&bot->p, &FeedbackEmptyProg);
            }
            else {
                perform_startCb(&bot->p, &FeedbackAck, (FinishFunc)bot_startProgram, bot);
            }
            break;
        case BTN_CLEAR:
            os_printf("==> Clearing program\n");
            bot_clearProgram(bot);
            perform_start(&bot->p, &FeedbackEmptyProg);
            break;
        case BTN_RIGHT:
        case BTN_FORWARD:
        case BTN_LEFT:
        case BTN_BACK:
        case BTN_OP1:
        case BTN_OP2:
            if (bot->length < BOT_PROGRAM_SIZE) {
                bot->program[bot->length++] = btn;
                perform_start(&bot->p, &FeedbackAck);
            }
            else {
                perform_start(&bot->p, &FeedbackErr);
            }
            break;
        default:
            os_printf("%s: unhandled button %2x %s\n", __FUNCTION__, btn, btnStr(btn));
    }
}

static void ICACHE_FLASH_ATTR apTimerCb(void *arg) {
    
    int val = 2; //3
    wifi_set_opmode(val);
    system_restart();
}

void ICACHE_FLASH_ATTR
bot_btnHandler(void *data, void *dataWeb, int btn, int value)
{
    Bot *bot = data;
    Bot *botWeb = dataWeb;

    bot_stopProgram(botWeb);
    os_printf("bot: Button event %x %s   value %d\n", btn, btnStr(btn), value);

    hw_keepAwake(60000);

    // mode switch
    if (value && btn != BTN_CLEAR && (hw_btnState() & BTN_CLEAR)) {
        switch (btn) {
            case BTN_FORWARD:
                os_printf("==> Switched to music box mode\n");
                bot_stopProgram(bot);
                perform_start(&bot->p, &DoReMi);
                bot->inputMode = MusicBoxInputMode;
                return;
            case BTN_STARTSTOP:
                os_printf("==> Switched to normal mode\n");
                bot_stopProgram(bot);
                perform_start(&bot->p, &FeedbackEmptyProg);
                bot->inputMode = NormalInputMode;
                return;
            case BTN_BACK:
                os_printf("==> Switched to AP mode\n");
                os_timer_disarm(&testTimer3);
                os_timer_setfn(&testTimer3, apTimerCb, NULL);
                os_timer_arm(&testTimer3, 2000, 0);
                return;
        }
    }

    switch (bot->inputMode) {
        case MusicBoxInputMode:
            bot_handleMusicBoxInput(bot, btn, value);
            return;
        case NormalInputMode:
        default:
            bot_handleNormalInput(bot, btn, value);
            return;
    }
}

bool ICACHE_FLASH_ATTR
bot_checkAdc(Bot *bot)
{
    uint16 value = hw_adc();
    uint32 V = (1000 * value *  1217 / 217) / 1024 ;
    os_printf("Power: %u (%d.%03dV)\n",  value, V / 1000, V % 1000);
    os_timer_arm(&bot->adcCheckTimer, 10000, 0);
    if (value < ADC_MIN && ORIGINAL_BOT) {
        perform_start(&bot->p, &FeedbackPowerWarning);
        return true;
    }
    return false;
}

static void ICACHE_FLASH_ATTR
bot_adcCheckTimerfunc(void *data)
{
    Bot *bot = data;
    bot_checkAdc(bot);
}

void ICACHE_FLASH_ATTR
bot_init(Bot* bot, Bot* botWeb)
{
    bot->length = 0;
    bot->pc = -1;
    perform_init(&bot->p);

    botWeb->length = 0;
    botWeb->pc = -1;
    perform_init(&botWeb->p);
    

    if (!hw_wasDeepSleep() || hw_wasSleepInterrupt()) {
        hw_keepAwake(60000);

        os_timer_disarm(&bot->adcCheckTimer);
        os_timer_setfn(&bot->adcCheckTimer, (os_timer_func_t *)bot_adcCheckTimerfunc, bot);
        os_timer_arm(&bot->adcCheckTimer, 0, 0);

        //hw_setButtonHandler(bot_btnHandler, bot);
        hw_setButtonHandler(bot_btnHandler, bot, botWeb);

        hw_allOff();

        if (!hw_wasDeepSleep()) {
            perform_start(&bot->p, &FeedbackPowerUp);
        }
    }
    else {
        if (!bot_checkAdc(bot) && ORIGINAL_BOT) {
            // woke from sleep, no interrupt, no power alarm
            // can go back to sleep immediately
            os_printf("Woke from sleep, no interrupt, no power alarm\n");
            hw_deepSleep();
        }
    }
}

/* vim: set sw=4 sts=4 ts=4 et : */
