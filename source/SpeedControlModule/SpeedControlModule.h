#ifndef SPEEDCONTROL_H_INCLUDED
#define SPEEDCONTROL_H_INCLUDED

#include "../SwitchDriver/Switch.h"
#include "../MotorDriver/Motor.h"

/* Shared data types */
typedef enum{MINIMUM, MEDIUM, MAXIMUM} SpeedState_t;     /* This should be in private */


/************ Module APIs ******************/
void SpeedControl_init(void);                   /* call this API to initiate the module */
void SpeedControl_update(void);                 /* call this API perodically to update the speed state */
SpeedState_t SpeedControl_GetSpeedState(void);  /* call this API to get the current speed state */
unsigned char SpeedControl_GetAngle(void);      /* call this API to get the current angle */

/* To Refactor */
/*
*   1 - to change   SpeedState_t SpeedControl_GetSpeedState(void)  API to   
                    ERROR_STATE_t SpeedControl_GetSpeedState(SpeedState_t *pSpeedState)
                    where ERROR_STATE_t is enumelation of {E_OK, E_NOK, PENDING}
    2 - also for void SpeedControl_update(void)
    3 - also for unsigned char SpeedControl_GetAngle(void)
*/

#endif
