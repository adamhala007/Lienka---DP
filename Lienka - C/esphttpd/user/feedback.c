#include "perform.h"
#include "notes_int.h"

PROG(FeedbackPowerUp, CMDS({
    {Leds, 3},
    {Sleep, 500},
    {Leds, 0},
    {Sleep, 500},
    {Leds, 3},
    {Sleep, 500},
    {Leds, 0},
}));

PROG(FeedbackAck, CMDS({
    {Leds, 3},
    {Sound, 110},
    {Sleep, 200},
    {NoSound},
    {Leds, 0},
}));

PROG(FeedbackAck2, CMDS({
    {Leds, 3},
    {Sound, 110},
    {Sleep, 200},
    {NoSound},
    {Leds, 0},
    {Sleep, 200},
    {Leds, 3},
    {Sound, 110},
    {Sleep, 200},
    {NoSound},
    {Leds, 0},
}));

PROG(FeedbackErr, CMDS({
    {Leds, 1},
    {Sound, 220},
    {Sleep, 200},
    {Leds, 2},
    {Sound, 110},
    {Sleep, 200},
    {NoSound},
    {Leds, 0},
}));

PROG(FeedbackEmptyProg, CMDS({
    {Sound, 110},
    {Leds, 1},
    {Sleep, 200},
    {Leds, 2},
    {Sleep, 200},
    {Leds, 1},
    {Sleep, 200},
    {Leds, 2},
    {Sleep, 200},
    {Leds, 1},
    {Sleep, 200},
    {Leds, 0},
    {NoSound}
}));

PROG(FeedbackPowerWarning, CMDS({
    {Sound, 880},
    {Sleep, 1000},
    {NoSound},
    {Sleep, 1000},
    {Goto, 0},
}));

/* vim: set sw=4 sts=4 ts=4 et : */
