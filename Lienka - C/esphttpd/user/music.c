#include "perform.h"
#include "notes_int.h"

#define N(P,T) {Sound, P}, {Sleep, T}, {NoSound, 0}, {Sleep, ((T)/16)}
#define P(T) {Sleep, ((T)+(T)/16) }

PROG(DoReMi, CMDS({
    N(C4, T4), N(D4,T4), N(E4,T4)
}));

PROG(Scale, CMDS({
    N(C4,T8), N(D4,T8), N(E4,T8), N(F4,T8), N(G4,T8), N(A4,T8), N(B4,T8), N(C5,T8),
}));

#undef TEMPO
#define TEMPO 120
PROG(CervenyKachel, CMDS({
    N(C4, T4), N(C4, T8), N(D4, T8),  N(E4, T4), N(C4, T4),
    N(D4, T4), N(D4, T4),  N(D4, T4), P(T4),
    N(D4, T8), N(D4, T8), N(D4, T8), N(E4, T8),  N(F4, T4), N(D4, T4),
    N(C4, T4), N(E4, T4),  N(G4, T4), P(T4),

    N(G4, T8), N(G4, T8), N(G4, T8), N(F4, T8),  N(E4, T4), N(C4, T4),
    N(F4, T4), N(G4, T4),  N(A4, T4), P(T4),
    N(C5, T4), N(B4, T8), N(A4, T8),  N(G4, T4), N(C4, T4),
    N(E4, T4), N(D4, T4),  N(C4, T4), P(T4),
}));

#undef TEMPO
#define TEMPO 100
PROG(Cizicek, CMDS({
    N(D4, T8), N(Cs4, T8), N(D4, T8), N(E4, T8),  N(Fs4, T4), N(D4, T4),
    N(E4, T4), N(D4, T8), N(Cs4, T8),  N(D4, T4), N(A3, T4),
    N(D4, T8), N(Cs4, T8), N(D4, T8), N(E4, T8),  N(Fs4, T4), N(D4, T4),
    N(E4, T8), N(E4, T8), N(D4, T8), N(Cs4, T8),  N(D4, T4), P(T4),

    N(G4, T8), N(G4, T8), N(E4, T4),  N(Fs4, T8), N(Fs4, T8), N(D4, T4),
    N(E4, T8), N(E4, T8), N(Cs4, T4),  N(D4, T8), N(E4, T8), N(Fs4, T4),
    N(G4, T8), N(G4, T8), N(E4, T4),  N(Fs4, T8), N(Fs4, T8), N(D4, T4),
    N(E4, T8), N(E4, T8), N(Cs4, T4),  N(D4, T8), N(Fs4, T8), N(D4, T4),

}));

#undef TEMPO
#define TEMPO 110
PROG(KrasnaKrasna, CMDS({
    N(G4, T4), N(E4, T4), N(G4, T4), N(E4, T4),
    N(G4, T8), N(C5, T8), N(B4, T8), N(A4, T8), N(G4, T4), N(E4, T4),
    N(G4, T8), N(A4, T8), N(G4, T8), N(F4, T8), N(E4, T4), N(E4, T4),
    N(E4, T8), N(F4, T8), N(E4, T8), N(D4, T8), N(C4, T4), N(C4, T4),
    N(G4, T8), N(A4, T8), N(G4, T8), N(F4, T8), N(E4, T4), N(E4, T4),
    N(E4, T8), N(F4, T8), N(E4, T8), N(D4, T8), N(C4, T4), N(C4, T4),
}));

#undef TEMPO
#define TEMPO 100
PROG(Kohutik, CMDS({
    N(C4, T8), N(D4, T8), N(E4, T4), N(F4, T4), N(F4, T8), N(F4, T8),
    N(F4, T8), N(E4, T8), N(D4, T4), N(E4, T4), N(E4, T8), N(E4, T8),
    N(E4, T8), N(D4, T8), N(C4, T4), N(D4, T4), N(D4, T8), N(D4, T8),
    N(D4, T8), N(E4, T8), N(D4, T4), N(C4, T4), N(C4, T8), N(C4, T8),
}));

#undef TEMPO
#define TEMPO 106
PROG(Kukulienka, CMDS({
    N(C5, T4), N(D5, T4),  N(C5, T4d), N(Bb4, T8),
    N(A4, T8), N(Bb4, T8), N(C5, T4),  N(F4, T4), P(T4),
    N(Bb4, T4), N(A4, T4),  N(G4, T4d), N(F4, T8),
    N(E4, T8), N(F4, T8), N(G4, T4),  N(C4, T4), P(T4),

    N(A4, T8), N(Bb4, T8), N(C5, T4),  N(F4, T8), N(G4, T8), N(A4, T4),
    N(C5, T8), N(Bb4, T8), N(A4, T8), N(G4, T8),  N(F4, T8), N(F4, T8), N(F4, T8), P(T8),

    N(A4, T8), N(Bb4, T8), N(C5, T4),  N(F4, T8), N(G4, T8), N(A4, T4),
    N(C5, T8), N(Bb4, T8), N(A4, T8), N(G4, T8),  N(F4, T8), N(F4, T8), N(F4, T8), P(T8),
}));

#undef TEMPO
#define TEMPO 100
PROG(MalickaSom, CMDS({
    N(E4, T8), N(E4, T8), N(D4, T4),  N(C4, T4), P(T4),
    N(E4, T8), N(E4, T8), N(D4, T4),  N(C4, T4), P(T4),
    N(C4, T8), N(E4, T8), N(C4, T8), N(E4, T8),  N(C4, T4), N(E4, T4),
    N(F4, T8), N(E4, T8), N(D4, T4),  N(G4, T4), P(T4),
    N(C4, T8), N(E4, T8), N(C4, T8), N(E4, T8),  N(C4, T4), N(E4, T4),
    N(F4, T8), N(E4, T8), N(D4, T4),  N(C4, T4), P(T4),
}));

#undef TEMPO
#define TEMPO 100
PROG(MaliMuzikanti, CMDS({
    N(C4, T8), N(D4, T8), N(E4, T8), N(F4, T8),  N(G4, T8), N(A4, T8), N(G4, T8), N(E4, T8),
    N(G4, T8), N(F4, T8), N(E4, T8), N(D4, T8),  N(C4, T8), N(E4, T8), N(D4, T4),
    N(C4, T8), N(D4, T8), N(E4, T8), N(F4, T8),  N(G4, T8), N(A4, T8), N(G4, T8), N(E4, T8),
    N(G4, T8), N(F4, T8), N(E4, T8), N(D4, T8),  N(C4, T8), N(C4, T8), N(C4, T4),

    N(D4, T8), N(D4, T8), N(D4, T8), N(D4, T8),  N(D4, T8), N(E4, T8), N(F4, T8), N(D4, T8),
    N(E4, T8), N(E4, T8), N(E4, T8), N(E4, T8),  N(E4, T8), N(F4, T8), N(G4, T8), N(E4, T8),
    N(C4, T8), N(D4, T8), N(E4, T8), N(F4, T8),  N(G4, T8), N(A4, T8), N(G4, T4),
    N(G4, T8), N(F4, T8), N(E4, T8), N(D4, T8),  N(C4, T8), N(C4, T8), N(C4, T4),
}));

#undef TEMPO
#define TEMPO 82
PROG(Medvedku, CMDS({
    N(C4, T4), N(D4, T4), N(E4, T4), P(T4),
    N(E4, T4), N(D4, T4), N(C4, T4), P(T4),
    N(C4, T4), N(D4, T4), N(E4, T4), P(T4),
    N(E4, T4), N(D4, T4), N(C4, T4), P(T4),

    N(E4, T8), N(F4, T8), N(G4, T8), N(G4, T8),
    N(G4, T8), N(F4, T8), N(E4, T8), N(D4, T8),
    N(D4, T8), N(E4, T8), N(F4, T8), N(F4, T8),
    N(G4, T8), N(F4, T8), N(E4, T8), N(D4, T8),

    N(C4, T4), N(D4, T4), N(E4, T4), P(T4),
    N(E4, T4), N(D4, T4), N(C4, T4), P(T4),
    N(C4, T4), N(D4, T4), N(E4, T4), P(T4),
    N(E4, T4), N(D4, T4), N(C4, T4), P(T4),
}));

#undef TEMPO
#define TEMPO 68
PROG(MojaMamka, CMDS({
    N(C4, T8), N(D4, T8), N(E4, T8), N(F4, T8), N(G4, T8), N(A4, T8), N(G4, T4),
    N(G4, T8), N(A4, T8), N(G4, T4), N(G4, T8), N(A4, T8), N(G4, T8), N(F4, T8),
    N(E4, T8), N(D4, T8), N(C4, T4), N(E4, T8), N(D4, T8), N(C4, T4),

    N(C4, T8), N(D4, T8), N(E4, T8), N(F4, T8), N(G4, T8), N(A4, T8), N(G4, T4),
    N(G4, T8), N(A4, T8), N(G4, T4), N(G4, T8), N(A4, T8), N(G4, T8), N(F4, T8),
    N(E4, T8), N(D4, T8), N(C4, T4), N(E4, T8), N(D4, T8), N(C4, T4),
}));

#undef TEMPO
#define TEMPO 92
PROG(NaZelenejLuke, CMDS({
            /* TODO s/Ds/Cs/ ??? */
    N(D4, T8), N(E4, T8), N(Fs4, T8), N(G4, T8),  N(A4, T4), N(Fs4, T4),
    N(G4, T8), N(Fs4, T8), N(E4, T4),  N(Fs4, T4), P(T4),
    N(D4, T8), N(E4, T8), N(Fs4, T8), N(G4, T8),  N(A4, T4), N(Fs4, T4),
    N(G4, T8), N(Fs4, T8), N(E4, T4),  N(D4, T4), P(T4),

    N(A4, T8), N(A4, T8), N(Fs4, T8), N(Fs4, T8),  N(G4, T8), N(G4, T8), N(E4, T8), N(E4, T8),
    N(A4, T8), N(A4, T8), N(Fs4, T8), N(Fs4, T8),  N(G4, T8), N(G4, T8), N(E4, T8), N(E4, T8),
    N(G4, T8), N(Fs4, T8), N(E4, T4),  N(A4, T4), P(T4),
    N(G4, T8), N(Fs4, T8), N(E4, T4),  N(D4, T4), P(T4),
}));

#undef TEMPO
#define TEMPO 80
PROG(PojdemeMyDoHajicka, CMDS({
    N(C4, T8), N(D4, T8), N(E4, T8), N(F4, T8),  N(G4, T8), N(G4, T8), N(G4, T8), N(G4, T8),
    N(G4, T8), N(F4, T8), N(E4, T8), N(D4, T8),  N(C4, T8), N(C4, T8), N(C4, T8), N(C4, T8),
    N(C4, T8), N(D4, T8), N(E4, T8), N(F4, T8),  N(G4, T4), P(T4),
    N(G4, T4), P(T4),  N(G4, T8), N(F4, T8), N(E4, T8), N(D4, T8),
    N(C4, T4), P(T4),  N(C4, T4), P(T4),
}));

#undef TEMPO
#define TEMPO 110
PROG(PrsiPrsi, CMDS({
    N(G4, T8), N(G4, T8), N(A4, T8), N(G4, T8),
    N(G4, T8), N(G4, T8), N(A4, T8), N(G4, T8),
    N(G4, T8), N(G4, T8), N(A4, T8), N(G4, T8),
    N(G4, T8), N(G4, T8), N(A4, T8), N(G4, T8),

    N(F4, T8), N(F4, T8), N(F4, T4),
    N(E4, T8), N(E4, T8), N(E4, T4),
    N(D4, T8), N(D4, T8), N(G4, T8), N(G4, T8),
    N(C4, T8), N(C4, T8), N(C4, T8), P(T8),

    N(F4, T8), N(F4, T8), N(F4, T4),
    N(E4, T8), N(E4, T8), N(E4, T4),
    N(D4, T8), N(D4, T8), N(G4, T8), N(G4, T8),
    N(C4, T8), N(C4, T8), N(C4, T8), P(T8),
}));

#undef TEMPO
#define TEMPO 108
PROG(SedemdesiatSukien, CMDS({
    N(C4, T4d), N(D4, T8),  N(E4, T4), N(E4, T4), N(D4, T4), N(D4, T4), N(C4, T4), N(C4, T4),
    N(E4, T4d), N(F4, T8), N(G4, T4), N(G4, T4), N(F4, T4), N(F4, T4), N(E4, T4), N(E4, T4),

    N(E4, T4d), N(F4, T8), N(G4, T4), N(G4, T4),
    N(A4, T8), N(A4, T8), N(A4, T8), N(A4, T8), N(G4, T4), N(C5, T8), P(T8),
    N(C4, T4d), N(D4, T8), N(E4, T4), N(E4, T4), N(D4, T4), N(D4, T4), N(C4, T4), N(C4, T4),

    N(E4, T4d), N(F4, T8), N(G4, T4), N(G4, T4),
    N(A4, T8), N(A4, T8), N(A4, T8), N(A4, T8), N(G4, T4), N(C5, T8), P(T8),
    N(C4, T4d), N(D4, T8), N(E4, T4), N(E4, T4), N(D4, T4), N(D4, T4), N(C4, T4), N(C4, T4),
}));


#undef TEMPO
#define TEMPO 114
PROG(SosvickaHrasokFazula, CMDS({
    N(G4, T8), N(G4, T8), N(G4, T8), N(G4, T8),  N(E4, T4), N(G4, T4),
    N(C4, T8), N(C4, T8), N(C4, T4),
    N(A4, T8), N(A4, T8), N(A4, T8), N(A4, T8),  N(F4, T4), N(A4, T4),
    N(G4, T8), N(G4, T8), N(G4, T4),

    N(C5, T8), N(G4, T8), N(C5, T8), N(G4, T8),  N(E4, T4), N(G4, T4),
    N(C4, T4), N(E4, T4),
    N(F4, T8), N(D4, T8), N(F4, T8), N(D4, T8),  N(B3, T4), N(D4, T4),
    N(C4, T4), N(C4 ,T4),

    N(C5, T8), N(G4, T8), N(C5, T8), N(G4, T8),  N(E4, T4), N(G4, T4),
    N(C4, T4), N(E4, T4),
    N(F4, T8), N(D4, T8), N(F4, T8), N(D4, T8),  N(B3, T4), N(D4, T4),
    N(C4, T4), N(C4 ,T4),
}));

#undef TEMPO
#define TEMPO 70
PROG(SpiDietatkoSpize, CMDS({
    N(Fs4, T8), N(Fs4, T8), N(E4, T8), N(E4, T8),  N(D4, T4), N(A3, T4),
    N(Fs4, T8), N(Fs4, T8), N(E4, T8), N(E4, T8),  N(D4, T4), N(A3, T4),
    N(D4, T8), N(D4, T8), N(E4, T4),  N(Fs4, T8), N(Fs4, T8), N(E4, T4),
    N(Fs4, T8), N(Fs4, T8), N(E4, T8), N(E4, T8),  N(D4, T4), N(D4, T4),
    N(D4, T8), N(D4, T8), N(E4, T4),  N(Fs4, T8), N(Fs4, T8), N(E4, T4),
    N(Fs4, T8), N(Fs4, T8), N(E4, T8), N(E4, T8),  N(D4, T4), N(D4, T4),
}));


#undef TEMPO
#define TEMPO 100
PROG(SpiSynackuSpi, CMDS({
    N(E4, T4), N(E4, T4), N(D4, T4), N(D4, T4),  N(C4, T2), P(2),
    N(F4, T4), N(F4, T4), N(E4, T4), N(E4, T4),  N(D4, T2), P(2),

    N(E4, T4), N(D4, T4), N(E4, T4), N(F4, T4),  N(G4, T4), N(A4, T4), N(G4, T4), N(F4, T4),
    N(E4, T4), N(D4, T4), N(E4, T4), N(F4, T4),  N(G4, T4), N(A4, T4), N(G4, T4), N(F4, T4),
    N(E4, T4), N(E4, T4), N(D4, T4), N(D4, T4),  N(C4, T2), P(2),
}));

#undef TEMPO
#define TEMPO 100
PROG(Spievanky, CMDS({
    N(G4, T8), N(F4, T8), N(E4, T4),  N(G4, T4), N(C5, T8), N(G4, T8),
    N(G4, T8), N(F4, T8), N(E4, T4),  N(D4, T4), N(D4, T8), N(D4, T8),
    N(D4, T8), N(E4, T8), N(F4, T4),  N(A4, T4), N(A4, T8), N(G4, T8),
    N(F4, T8), N(E4, T8), N(D4, T4),  N(C4, T4), N(C4, T8), N(C4, T8),
}));

#undef TEMPO
#define TEMPO 104
PROG(VstavajHonzo, CMDS({
    N(F4, T4), N(F4, T4),  N(F4, T8), N(C4, T8), N(F4, T8), N(A4, T8),
    N(G4, T4), N(G4, T4),  N(G4, T8), N(C4, T8), N(G4, T8), N(A4, T8),
    N(F4, T4), N(F4, T4),  N(F4, T8), N(C4, T8), N(F4, T8), N(A4, T8),
    N(G4, T4), N(C4, T4),  N(Bb4, T8), N(A4, T8), N(G4, T4),
    N(F4, T4),
}));

#undef TEMPO
#define TEMPO 100
PROG(ZlataBrana, CMDS({
    N(G4, T4), N(A4, T4),  N(G4, T4), N(E4, T4),  N(G4, T4), N(A4, T4), N(G4, T4), N(E4, T4),
    N(G4, T4), N(A4, T4),  N(G4, T4), N(E4, T4),  N(G4, T4), N(A4, T4), N(G4, T4), N(E4, T4),
    N(G4, T4), N(G4, T8), N(E4, T8),  N(G4, T4), N(G4, T4),
    N(G4, T4), N(G4, T8), N(E4, T8),  N(G4, T4), N(G4, T4),
    N(G4, T8), N(G4, T8), N(G4, T8), N(G4, T8),  N(G4, T8), N(E4, T8), N(G4, T4),
    N(G4, T8), N(E4, T8), N(G4, T8), N(G4, T8),  N(E4, T8), N(E4, T8), N(E4, T4),
}));

#undef TEMPO
#define TEMPO 800
PROG(ImperialMarch, CMDS({
N(G4, M(8)), N(G4, M(8)), N(G4, M(8)), N(Eb4, M(6)), N(Bb4, M(2)),
N(G4, M(8)), N(Eb4, M(6)), N(Bb4, M(2)), N(G4, M(16)),
N(D5, M(8)), N(D5, M(8)), N(D5, M(8)), N(Eb5, M(6)), N(Bb4, M(2)),
N(Gb4, M(8)), N(Eb4, M(6)), N(Bb4, M(2)), N(G4, M(16)),
N(G5, M(8)), N(G4, M(6)), N(G4, M(2)), N(G5, M(8)), N(Gb5, M(6)), N(F5, M(2)),
N(E5, M(2)), N(Ds5, M(2)), N(E5, M(4)), P(M(4)), N(Gs4, M(4)), N(Cs5, M(8)), N(C5, M(6)), N(B4, M(2)),
N(Bb4, M(2)), N(A4, M(2)), N(Bb4, M(4)), P(M(4)), N(Eb4, M(4)), N(Gb4, M(8)), N(Eb4, M(6)), N(Gb4, M(2)),
N(Bb4, M(8)), N(G4, M(6)), N(Bb4, M(2)), N(D5, M(16)),
N(G5, M(8)), N(G4, M(6)), N(G4, M(2)), N(G5, M(8)), N(Gb5, M(6)), N(F5, M(2)),
N(E5, M(2)), N(Ds5, M(2)), N(E5, M(4)), P(M(4)), N(Gs4, M(4)), N(Cs5, M(8)), N(C5, M(6)), N(B4, M(2)),
N(Bb4, M(2)), N(A4, M(2)), N(Bb4, M(4)), P(M(4)), N(Eb4, M(4)), N(Gb4, M(8)), N(Eb4, M(6)), N(Bb4, M(2)),
N(G4, M(8)), N(Eb4, M(6)), N(Bb4, M(2)), N(G4, M(16)),
}));

const Program* AllSongs[] = {
    &MalickaSom,
    &Cizicek,
    &Kohutik,
    &KrasnaKrasna,
    &Kukulienka,

    &CervenyKachel,
    &PrsiPrsi,
    &Medvedku,
    &MojaMamka,

    &NaZelenejLuke,

    &PojdemeMyDoHajicka,
    &MaliMuzikanti,
    &ImperialMarch,
    &SedemdesiatSukien,

    &SosvickaHrasokFazula,
    &SpiDietatkoSpize,
    &Spievanky,
    &VstavajHonzo,
    &ZlataBrana,
};

/* vim: set sw=4 sts=4 ts=4 et : */
