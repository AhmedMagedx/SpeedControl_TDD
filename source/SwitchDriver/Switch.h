#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED

typedef enum {SW_PREPRESSED,SW_PRESSED,SW_PRERELEASED, SW_RELEASED} SWITCH_STATE_t;
typedef enum {PV_switch, MV_switch, P_switch, NONE} SWITCH_NAME_t;


void        SWITCH_init(void);
/*  The Test harnesses */
 SWITCH_NAME_t   (*SWITCH_GetPressedSwitch)(void); /* Turned into pointer so we can make it point to another fake function */
 unsigned char (*SWITCH_getTimePressed)(void);

void   SWITCH_update(void);



#endif
