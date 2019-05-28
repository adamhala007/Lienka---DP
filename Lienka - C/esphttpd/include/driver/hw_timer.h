#ifndef HW_TIMER_H
#define HW_TIMER_H 1
#include "c_types.h"

typedef enum {
    DIVDED_BY_1 = 0,		//timer clock
    DIVDED_BY_16 = 4,	//divided by 16
    DIVDED_BY_256 = 8,	//divided by 256
} TIMER_PREDIVED_MODE;

typedef enum {			//timer interrupt mode
    TM_LEVEL_INT = 1,	// level interrupt
    TM_EDGE_INT   = 0,	//edge interrupt
} TIMER_INT_MODE;

typedef enum {
    FRC1_SOURCE = 0,
    NMI_SOURCE = 1,
} FRC1_TIMER_SOURCE_TYPE;

void  hw_timer_arm(u32 val);
void  hw_timer_set_func(void (* user_hw_timer_cb_set)(void));
void ICACHE_FLASH_ATTR hw_timer_init(FRC1_TIMER_SOURCE_TYPE source_type, u8 req);

#endif //HW_TIMER_H
