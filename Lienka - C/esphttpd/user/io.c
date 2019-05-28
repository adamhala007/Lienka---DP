
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <esp8266.h>
#include <perform.h>
#include <hw.h>
#include <bot.h>
#include "os_type.h"
//2
#define LEDGPIO 5
#define BTNGPIO 0

#define LEFT 4
#define RIGHT 6
#define FORWARD 8
#define BACKWARD 2

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

/*PROG(ProgFwd, CMDS({
    {Leds, 3},
    {MotorFwd, MotorTimeTile},
    {Leds, 0},
    {Sound, 440},
    {Sleep, 200},
    {NoSound},
}));*/

static ETSTimer resetBtntimer;
os_timer_t timer;

void ICACHE_FLASH_ATTR ioLed(int ena) {
	//gpio_output_set is overkill. ToDo: use better mactos
	/*if (ena) {
		gpio_output_set((1<<LEDGPIO), 0, (1<<LEDGPIO), 0);
	} else {
		gpio_output_set(0, (1<<LEDGPIO), (1<<LEDGPIO), 0);
	}*/
    int gpio = 5;
    //gpio_output_set((1<<gpio), 0, (1<<gpio), 0);
    //hw_keepAwake(60000);
    switch (ena) {
        case LEFT:
            //hw_motor_setDir(1,1);
            hw_motor_step8(3);
            //gpio_output_set((1<<gpio), 0, (1<<gpio), 0);
            os_printf("Direction LEFT\n");
            break;
        case RIGHT:
            //hw_motor_setDir(-1,-1);
            hw_motor_step8(2);
            //gpio_output_set((1<<gpio), 0, (1<<gpio), 0);
            os_printf("Direction RIGHT\n");
            break;
        case FORWARD:
            //hw_motor_setDir(1,-1);
            hw_motor_step8(1);
            //gpio_output_set((1<<gpio), 0, (1<<gpio), 0);
            os_printf("Direction FORWARD\n");
            break;
        case BACKWARD:
            hw_motor_setDir(-1,1);
            //gpio_output_set((1<<gpio), 0, (1<<gpio), 0);
            os_printf("Direction BACKWARD\n");
            break;
        default:
            gpio_output_set(0, (1<<gpio), (1<<gpio), 0);
            hw_motor_setDir(0,0);
            os_printf("Direction DEFAULT\n");
            break;
    }
    
}

static void ICACHE_FLASH_ATTR resetBtnTimerCb(void *arg) {
	static int resetCnt=0;
	if (!GPIO_INPUT_GET(BTNGPIO)) {
		resetCnt++;
	} else {
		if (resetCnt>=6) { //3 sec pressed
			wifi_station_disconnect();
			wifi_set_opmode(0x3); //reset to AP+STA mode
			os_printf("Reset to AP mode. Restarting system...\n");
			system_restart();
		}
		resetCnt=0;
	}
}

void ioInit() {
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
	gpio_output_set(0, 0, (1<<LEDGPIO), (1<<BTNGPIO));
	os_timer_disarm(&resetBtntimer);
	os_timer_setfn(&resetBtntimer, resetBtnTimerCb, NULL);
	os_timer_arm(&resetBtntimer, 500, 1);
}

