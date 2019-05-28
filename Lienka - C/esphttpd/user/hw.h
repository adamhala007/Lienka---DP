#ifndef HAVE_BEEBOT_USER_HW_H
#define HAVE_BEEBOT_USER_HW_H 1
#include "c_types.h"

#define MotorHz 400

void hw_motor_setDir(int dir1, int dir2);
bool hw_init();
typedef void (*ButtonHandler)(void *data, void *dataWeb, int button, int value); //+
void hw_setButtonHandler(ButtonHandler handler, void *data, void *dataWeb);
void hw_sound(uint32 hz);
void hw_nosound();
void hw_setLed(int num, int state);
uint8 hw_btnState();
void hw_allOff();
uint16 hw_adc();
void hw_deepSleep();
bool hw_wasDeepSleep();
bool hw_wasSleepInterrupt();
void hw_keepAwake(uint32 ms);

#endif //HAVE_BEEBOT_USER_HW_H
/* vim: set sw=4 sts=4 ts=4 et : */
