#ifndef FAKESW_H_INCLUDED
#define FAKESW_H_INCLUDED

/* Include Real Switch module header file to expose Types */
#include "../../source/SwitchDriver/Switch.h"

void       FAKE_SW_init(void);

    /*  The Test harnesses */
extern SWITCH_NAME_t(*SWITCH_GetPressedSwitch)(void); /* Turned into pointer so we can make it point to another fake function */
extern unsigned char (*SWITCH_getTimePressed)(void);

void       Fake_SWITCH_update(void);

//SWITCH_STATE_t FAKE_SW_getSwState(SWITCHES_NAME_t SwitchName);
SWITCH_STATE_t    FAKE_SW_GetSwState(SWITCH_NAME_t SwitchName);
void       FAKE_SW_setSwState(SWITCH_NAME_t SwitchName ,SWITCH_STATE_t state);
void       Fake_SW_SetTimePressed(unsigned char Time);
void       FAKE_SW_destroy(void);



#endif // FAKESW_H_INCLUDED

