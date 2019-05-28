#include "perform.h"
#include "user_interface.h"
#include "osapi.h"
#include "hw.h"

int ICACHE_FLASH_ATTR perform_finalizeStep(Perform *p)
{
    if (p->pc != -1) {
        uint32 cmd = p->prog->cmds[p->pc].cmd;
        uint32 arg1 = p->prog->cmds[p->pc].arg1;
        uint32 arg2 = p->prog->cmds[p->pc].arg2;
        switch (cmd) {
            case Goto:
                return arg1;
            case MotorFwd:
            case MotorBack:
            case MotorRight:
            case MotorLeft:
                hw_motor_setDir(0,0);
                break;
            default:
                break;
        }
    }

    return p->pc + 1;
}

bool ICACHE_FLASH_ATTR perform_step(Perform *p)
{
    uint32 cmd = p->prog->cmds[p->pc].cmd;
    uint32 arg1 = p->prog->cmds[p->pc].arg1;
    uint32 arg2 = p->prog->cmds[p->pc].arg2;
    switch (cmd) {
        case None:
            return false;

        case Sleep:
            os_timer_disarm(&p->timer);
            os_timer_arm(&p->timer, arg1, 0);
            return true;

        case Goto:
            return false;

        case MotorFwd:
            hw_motor_setDir(1,-1);
            // TODO steps
            os_timer_disarm(&p->timer);
            os_timer_arm(&p->timer, arg1, 0);
            return true;
        case MotorBack:
            hw_motor_setDir(-1,1);
            // TODO steps
            os_timer_disarm(&p->timer);
            os_timer_arm(&p->timer, arg1, 0);
            return true;
        case MotorRight:
            hw_motor_setDir(-1,-1);
            // TODO steps
            os_timer_disarm(&p->timer);
            os_timer_arm(&p->timer, arg1, 0);
            return true;
        case MotorLeft:
            hw_motor_setDir(1,1);
            // TODO steps
            os_timer_disarm(&p->timer);
            os_timer_arm(&p->timer, arg1, 0);
            return true;

        case MotorOff:
            hw_motor_setDir(0,0);
            return false;

        case Sound:
            hw_sound(arg1);
            return false;

        case NoSound:
            hw_nosound();
            return false;

        case Leds:
            hw_setLed(0, arg1 & BIT0);
            hw_setLed(1, arg1 & BIT1);
            return false;

        case LedLeft:
            hw_setLed(0, arg1);
            return false;

        case LedRight:
            hw_setLed(1, arg1);
            return false;

        default:
            os_printf("Unknown cmd %d at position %d\n", cmd, p->pc);
            return false;
    }
}

void ICACHE_FLASH_ATTR perform_run(Perform *p)
{

    if (!perform_isRunning(p)) {
        return;
    }

    hw_keepAwake(60000);

    while (p->pc < p->prog->len) {
        p->pc = perform_finalizeStep(p);
        if (p->pc >= p->prog->len) {
            break;
        }

        bool async = perform_step(p);
        if (async) {
            return;
        }
    }
    p->isRunning = false;
    hw_allOff();
    if (p->finishFunc) {
        FinishFunc ff = p->finishFunc;
        void *fd = p->finishData;
        p->finishFunc = NULL;
        p->finishData = NULL;
        ff(fd);
    }
}

bool ICACHE_FLASH_ATTR perform_isRunning(Perform *p)
{
    return p->isRunning;
}

void ICACHE_FLASH_ATTR perform_stop(Perform *p)
{
    if (!perform_isRunning(p)) {
        return;
    }

    p->finishFunc = NULL;
    p->finishData = NULL;
    os_timer_disarm(&p->timer);
    hw_allOff();
    p->isRunning = false;
}

void ICACHE_FLASH_ATTR perform_start(Perform *p, const Program *prog)
{
    if (perform_isRunning(p)) {
        perform_stop(p); // TODO async stop?
    }

    p->prog = prog;
    p->isRunning = true;
    p->pc = -1;
    os_timer_arm(&p->timer, 0, 0);
}

void ICACHE_FLASH_ATTR perform_startCb(Perform *p, const Program *prog, FinishFunc func, void *data)
{
    perform_start(p, prog);
    perform_setFinishFunc(p, func, data);
}

void ICACHE_FLASH_ATTR perform_setFinishFunc(Perform *p, FinishFunc func, void *data)
{
    p->finishFunc = func;
    p->finishData = data;
}

static void ICACHE_FLASH_ATTR perform_timerfunc(void *data)
{
    perform_run(data);
}

void ICACHE_FLASH_ATTR perform_init(Perform *p)
{
    p->prog = NULL;
    p->isRunning = false;
    p->pc = -1;
    p->finishFunc = NULL;
    p->finishData = NULL;
    os_timer_disarm(&p->timer);
    os_timer_setfn(&p->timer, (os_timer_func_t *)perform_timerfunc, p);
}

/* vim: set sw=4 sts=4 ts=4 et : */
