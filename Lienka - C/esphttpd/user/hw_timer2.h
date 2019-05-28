#ifndef HAVE_BEEBOT_USER_HW_TIMER2_H
#define HAVE_BEEBOT_USER_HW_TIMER2_H 1
#include "os_type.h"

#define HW_TIMER2_NUM_TIMERS 2

typedef void (*HwTimer2Func)(void *data);

void hw_timer2_setFunc(int id, HwTimer2Func, void *data);
void hw_timer2_setPeriod(int id, uint32 period);
void hw_timer2_init();


#endif //HAVE_BEEBOT_USER_HW_TIMER2_H
