#ifndef HAVE_BEEBOT_USER_PERFORM_H
#define HAVE_BEEBOT_USER_PERFORM_H 1

#include "os_type.h"

#define PROG(name, p) \
    const Cmd ICACHE_RODATA_ATTR name ## Cmds[] = p; \
    const Program ICACHE_RODATA_ATTR name = { name ## Cmds, sizeof(name ## Cmds)/sizeof(name ## Cmds[0]) }

#define CMDS(x...) x

enum CmdType {
    None = 0,

    // Sleep MILISECONDS
    Sleep,

    // Goto LINE
    Goto,

    // MotorXX STEPS
    MotorFwd,
    MotorBack,
    MotorRight,
    MotorLeft,

    MotorOff,

    Sound,
    // same as Sound 0
    NoSound,

    // Leds STATE, set both leds to STATE (bit0 left, bit1 right)
    Leds,
    // LedXXX STATE (0/1)
    LedLeft,
    LedRight,
};

typedef struct _Cmd {
    uint32 cmd;
    uint32 arg1;
    uint32 arg2;
} Cmd;

typedef struct _Program {
    const Cmd *cmds;
    int len;
} Program;

typedef void (*FinishFunc)(void *data);

typedef struct _Perform {
    const Program *prog;
    bool isRunning;
    int pc;
    os_timer_t timer;
    FinishFunc finishFunc;
    void *finishData;
} Perform;

void perform_init(Perform *p);
void perform_start(Perform *p, const Program *prog);
void perform_startCb(Perform *p, const Program *prog, FinishFunc func, void *data);
void perform_stop(Perform *p);
bool perform_isRunning(Perform *p);

void perform_setFinishFunc(Perform *p, FinishFunc func, void *data);
bool perform_step(Perform *p);
void perform_run(Perform *p);

#endif //HAVE_BEEBOT_USER_PERFORM_H
/* vim: set sw=4 sts=4 ts=4 et : */
