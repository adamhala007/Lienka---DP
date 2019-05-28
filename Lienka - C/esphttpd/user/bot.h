#ifndef HAVE_BEEBOT_USER_BOT_H
#define HAVE_BEEBOT_USER_BOT_H 1

#include "os_type.h"
#include "perform.h"
//#include "hw.h"

/*#define ADC_MIN 639

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

PROG(ProgRight, CMDS({
    {Leds, 1},
    {MotorRight, MotorTime90},
    {Leds, 0},
    {Sound, 440},
    {Sleep, 200},
    {NoSound},
}));*/

#ifdef BUTTONS_SCHEMA
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
#endif

enum InputMode {
    NormalInputMode,
    MusicBoxInputMode,
};

#define BOT_PROGRAM_SIZE 50
typedef struct _Bot {
    uint8 program[BOT_PROGRAM_SIZE];
    int length;
    int pc;
    Perform p;
    enum InputMode inputMode;
    os_timer_t adcCheckTimer;
    int currentSong;
    int x, y, dir;
} Bot;

void bot_init(Bot* bot, Bot* botWeb);
//void bot_init(Bot* bot);
//void bot_step(void *arg);

#endif //HAVE_BEEBOT_USER_BOT_H
/* vim: set sw=4 sts=4 ts=4 et : */
