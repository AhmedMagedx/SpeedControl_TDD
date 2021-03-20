#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED
#include "../common/std_types.h"

typedef enum {SW_PREPRESSED,SW_PRESSED,SW_PRERELEASED, SW_RELEASED} SWITCH_STATE_t;
typedef enum {PV_switch, MV_switch, P_switch, NONE} SWITCH_NAME_t;


void        SWITCH_init(void);

/*  for Test harnesses */
SWITCH_NAME_t   (*SWITCH_GetPressedSwitch)(void); /* Turned into pointer so we can make it point to another fake function */
unsigned char (*SWITCH_getTimePressed)(void);

ERROR_t   SWITCH_update(void);



#endif
