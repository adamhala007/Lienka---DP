#include "hw_timer2.h"
#include "driver/hw_timer.h"
#include "user_interface.h"

struct Timer {
    uint32 period;
    uint32 next;
    HwTimer2Func func;
    void *data;
};

struct Timer timers[HW_TIMER2_NUM_TIMERS] = { 0 };

#define BEFORE(x,y) (ESP_U32_DIFF((y),(x)) < ESP_U32_DIFF((x),(y)))

/*
 * Note: Once started, it cannot be stopped (at least not via
 * hw_timer.h interface). Also, even if not armed in no-autoload
 * mode, it will trigger at max interval.
 */
void hw_timer2_callback(void)
{
    int i;
    uint32 now;

    for (i=0; i<HW_TIMER2_NUM_TIMERS; ++i) {
        if (timers[i].period && timers[i].func) {
            now = system_get_time();
            if (BEFORE(timers[i].next, now)) {
                timers[i].func(timers[i].data);
            }
        }
    }

    now = system_get_time();
    uint32 next = 0x800000;
    for (i=0; i<HW_TIMER2_NUM_TIMERS; ++i) {
        if (timers[i].period && timers[i].func) {
            while (BEFORE(timers[i].next, now)) {
                timers[i].next += timers[i].period;
            }
            uint32 diff = ESP_U32_DIFF(timers[i].next, now);
            if (diff < next) {
                next = diff;
            }
        }
    }
    if (next < 0x800000) {
        hw_timer_arm(next);
    }
}

void hw_timer2_setFunc(int id, HwTimer2Func func, void *data)
{
    if (id >= HW_TIMER2_NUM_TIMERS) {
        return;
    }
    timers[id].func = func;
}

void hw_timer2_setPeriod(int id, uint32 period)
{
    if (id >= HW_TIMER2_NUM_TIMERS) {
        return;
    }
    ETS_FRC1_INTR_DISABLE();
    timers[id].period = period;
    timers[id].next = system_get_time() + period;
    hw_timer2_callback();
    ETS_FRC1_INTR_ENABLE();
}

void hw_timer2_init()
{
    hw_timer_init(FRC1_SOURCE, 0);
    hw_timer_set_func(hw_timer2_callback);
}


/* vim: set sw=4 sts=4 ts=4 et : */
