#include "hw.h"
#include "i2c_mcp.h"
#include "osapi.h"
#include "gpio.h"
#include "ets_sys.h"
#include "os_type.h"
#include "user_interface.h"
#include "driver/i2c_master.h"
#include "hw_timer2.h"

#define HWTIMER_MOTOR 0
#define HWTIMER_SOUND 1

#define PIEZO_GPIO 12

static const uint8 motor_steps8[8] = {
    0b1000,
    0b1100,
    0b0100,
    0b0110,
    0b0010,
    0b0011,
    0b0001,
    0b1001,
};

static const uint8 motor_steps4[4] = {
    0b1000,
    0b0100,
    0b0010,
    0b0001,
};

static int step[2] = {0,0};
static int dir[2] = {0,0};

bool ICACHE_FLASH_ATTR hw_i2c_init()
{
    //IOCON configuration
    uint8 config = 0
        | (1 << 1) // INTPOL: 0 active low / 1 active high
        | (0 << 2) // ODR: 0 active driver / 1 open drain
        | (0 << 6) // MIRROR: 1 INTs are mirrored (must read both banks) / 0 INTs are separate
    ;
    if (!mcp_writeRegister(MCP23017_addr, MCP23017_registerIOCON, config, 1)) {
        os_printf("%s: failed to set IOCON\n", __FUNCTION__);
        return false;
    }
    // GPIOA input, GPIOB output
    if (!mcp23017_setDirection(0xff00)) {
        return false;
    }
    // Pullups for input
    if (!mcp_writeRegister(MCP23017_addr,MCP23017_registerGPPU, 0xff00, 2)) {
        os_printf("%s: failed to set GPPU\n", __FUNCTION__);
        return false;
    }
    // and reverse polarity just for fun
    if (!mcp_writeRegister(MCP23017_addr,MCP23017_registerIPOL, 0xff00, 2)) {
        os_printf("%s: failed to set IPOL\n", __FUNCTION__);
        return false;
    }

    // INTCONfig (0 - change, 1 diff to IODEF)
    // This is power on value, not really need to change it...
    if (!mcp_writeRegister(MCP23017_addr,MCP23017_registerINTCON, 0x00, 1)) {
        os_printf("%s: failed to set INTCON\n", __FUNCTION__);
        return false;
    }
    // INTENable for GPIOA
    if (!mcp_writeRegister(MCP23017_addr,MCP23017_registerINTEN, 0xff, 1)) {
        os_printf("%s: failed to set INTEN\n", __FUNCTION__);
        return false;
    }

    return true;
}

void ICACHE_FLASH_ATTR hw_motor_setDir(int dir1, int dir2)
{
    dir[0] = dir1;
    dir[1] = dir2;
    if (dir1 || dir2) {
        hw_timer2_setPeriod(HWTIMER_MOTOR, 1000000/MotorHz);
    }
    else {
        hw_timer2_setPeriod(HWTIMER_MOTOR, 0);
        mcp23017_setGpioB(0);
    }
}

bool ICACHE_FLASH_ATTR hw_motor_step(int which, const uint8 steps[], int nsteps)
{
    if ((!dir[0] || !(which & 1)) && (!dir[1] || !(which & 2))) {
        // no movement
        return true;
    }

    uint8 val = 0;
    int m;
    for (m=0; m<2; ++m) {
        if (which & (1 << m)) {
            step[m] = (step[m] + dir[m] + nsteps) % nsteps;
        }
        val |= steps[step[m]] << (m*4);
    }

    if (!mcp23017_setGpioB(val)) {
        return false;
    }

    return true;
}

bool ICACHE_FLASH_ATTR hw_motor_step8(int which)
{
    return hw_motor_step(which, motor_steps8, 8);
}

bool ICACHE_FLASH_ATTR hw_motor_step4(int which)
{
    return hw_motor_step(which, motor_steps4, 4);
}

volatile uint32 lastInterruptDuration = 0;
static void hw_motor_timer(void *data) // ICACHE_FLASH_ATTR
{
    uint32 tstart = system_get_time();

    hw_motor_step4(3);

    uint32 tstop = system_get_time();
    lastInterruptDuration = ESP_U32_DIFF(tstop, tstart);
}

static void hw_sound_timer(void *data)
{
    static int val = 0;
    val = !val;
    GPIO_OUTPUT_SET(GPIO_ID_PIN(PIEZO_GPIO), val);
}

void ICACHE_FLASH_ATTR hw_sound(uint32 hz)
{
    hw_timer2_setPeriod(HWTIMER_SOUND,
            hz ? 1000000/hz/2 : 0
    );

    if (!hz) {
        GPIO_OUTPUT_SET(GPIO_ID_PIN(PIEZO_GPIO), 0);
    }
}

void ICACHE_FLASH_ATTR hw_nosound()
{
    hw_sound(0);
}

void ICACHE_FLASH_ATTR hw_setLed(int num, int state)
{
    int gpio = 13 + num;
    GPIO_OUTPUT_SET(GPIO_ID_PIN(gpio), !state);
}

#define INTR_GPIO 5
static volatile uint8 gpioAState;
static os_timer_t btnHandler_timer;
static ButtonHandler btnHandler = 0;
static void *btnHandlerData = 0;
static void *btnHandlerWebData = 0; //+

void ICACHE_FLASH_ATTR hw_setButtonHandler(ButtonHandler handler, void *data, void *dataWeb) //+
{
    btnHandler = handler;
    btnHandlerData = data;
    btnHandlerWebData = dataWeb;
}

uint8 hw_btnState()
{
    return gpioAState;
}

static bool btnAfterSleep = false;

static void btnHandler_timerfunc(void *arg)
{
    // this can't be interrupted by motor driving hwtimer
    // (though maybe we should only skip motor in the interrupt
    // and not the PWMs)
    uint8 ga;
    ETS_FRC1_INTR_DISABLE();
    if (btnAfterSleep) {
        btnAfterSleep = false;
        // button state change during sleep, let's read
        // the captured state
        ga = mcp23017_readIntcapA();
        // and then also the current one (the button
        // might have been release alredy)
        os_timer_arm(&btnHandler_timer, 0, 0);
    }
    else {
        ga = mcp23017_readGpioA();
    }
    ETS_FRC1_INTR_ENABLE();

    uint8 change = ga ^ gpioAState;
    if (change) {
        gpioAState = ga;
        uint8 btn = 1;
        while (change) {
            if (change & 1) {
                btnHandler(btnHandlerData,btnHandlerWebData, btn, ga & 1);
            }
            change >>= 1;
            ga >>= 1;
            btn <<= 1;
        }

    }
}

static void gpio_intr_handler(void *data)
{
    ETS_GPIO_INTR_DISABLE();
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(INTR_GPIO));

    // Read values in main loop with 10ms delay to debounce.
    // Note: expander clears the interrupt after i2c read, so
    // we don't need to be afraid of re-arming before it times out.
    os_timer_arm(&btnHandler_timer, 10, 0);

    ETS_GPIO_INTR_ENABLE();
}

uint16 ICACHE_FLASH_ATTR hw_adc()
{
    uint16 sum = 0;
    int i;
    for(i = 0; i<10; ++i) {
        sum += system_adc_read();
    }
    return sum / 10;
}

static bool wasSleepInterrupt = false;
static os_timer_t deepSleep_timer;

void ICACHE_FLASH_ATTR hw_deepSleep()
{
    os_printf("Going to deep sleep\n");
    uint32 gas = gpioAState;
    system_rtc_mem_write(65, &gas, 4);
    system_deep_sleep(1123123);
}

bool ICACHE_FLASH_ATTR hw_wasDeepSleep()
{
    return system_get_rst_info()->reason == REASON_DEEP_SLEEP_AWAKE;
}

bool ICACHE_FLASH_ATTR hw_checkDeepWakeup()
{
    if (hw_wasDeepSleep()) {
        uint32 gas;
        system_rtc_mem_read(65, &gas, 4); 
        gpioAState = gas;
        return true;
    }
    return false;
}

bool ICACHE_FLASH_ATTR hw_wasSleepInterrupt()
{
    return wasSleepInterrupt;
}

void ICACHE_FLASH_ATTR hw_keepAwake(uint32 ms)
{
    // TODO extend, not set
    os_timer_disarm(&deepSleep_timer);
    os_timer_arm(&deepSleep_timer, ms, 0);
}

static void ICACHE_FLASH_ATTR deepSleep_timerfunc(void *arg)
{
    hw_deepSleep();
}

bool ICACHE_FLASH_ATTR hw_gpio_init() {
    // "post" from interrupt into mainloop... should really be a task...
    os_timer_disarm(&btnHandler_timer);
    os_timer_setfn(&btnHandler_timer, (os_timer_func_t *)btnHandler_timerfunc, NULL);

    ETS_GPIO_INTR_DISABLE();

    // set up interrupt on INTR_GPIO
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
    // leds
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
    // piezo
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);

    gpio_output_set(0, 0, 0, INTR_GPIO);
    ETS_GPIO_INTR_ATTACH(gpio_intr_handler, NULL);
    gpio_pin_intr_state_set(INTR_GPIO, GPIO_PIN_INTR_POSEDGE);

    wasSleepInterrupt = GPIO_INPUT_GET(INTR_GPIO);
    if (wasSleepInterrupt) {
        os_printf("Interrupt up on powerup!\n");
        btnAfterSleep = true;
        os_timer_arm(&btnHandler_timer, 0, 0);
    }

    ETS_GPIO_INTR_ENABLE();

    return true;
}

bool ICACHE_FLASH_ATTR hw_init()
{
    bool wasDeepSleep;
    if ((wasDeepSleep = hw_checkDeepWakeup())) {
        os_printf("Wake from deep sleep\n");
    }

    gpio_init();
    i2c_master_gpio_init();

    if (!hw_gpio_init()) {
        return false;
    }

    if (!wasDeepSleep) {
        if (!hw_i2c_init()) {
            return false;
        }
    }

    hw_timer2_init();
    hw_timer2_setFunc(HWTIMER_MOTOR, hw_motor_timer, NULL);
    hw_timer2_setFunc(HWTIMER_SOUND, hw_sound_timer, NULL);

    os_timer_disarm(&deepSleep_timer);
    os_timer_setfn(&deepSleep_timer, deepSleep_timerfunc, NULL);
    hw_keepAwake(30000);

    return true;
}

void ICACHE_FLASH_ATTR hw_allOff()
{
    hw_motor_setDir(0,0);
    hw_setLed(0,0);
    hw_setLed(1,0);
    hw_nosound();
}

/* vim: set sw=4 sts=4 ts=4 et : */
