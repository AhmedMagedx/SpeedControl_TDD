
/**
 * @file SpeedControlModule.c
 * @author Ahmed Maged
 * @date 12/03/2021
 * @brief File containing the implementaion of Speed Control module
 */


#include "SpeedControlModule.h"
#include "PrivateSpeedControl.h"

#if 0
/*these variables are read from the switch driver */
extern SWITCH_STATE_t PV_State;
extern SWITCH_STATE_t MV_State;
extern SWITCH_STATE_t P_State;
extern unsigned char TimePressed;
#endif



#define NULL 0

/* Privates */
static SpeedState_t SpeedState = MINIMUM;   /* minimum is a PRE_INITIALIZATION value*/
static unsigned char SharedAngle = 0;       /* Zero is the PRE_INITIALIZATION value */



/**
 * @brief the module initialization to the defaults.
 * 
 */
void SpeedControl_init(void)
{
    SpeedState = MEDIUM;        /* default speed state */
    SharedAngle = 90;           /* default angle (50% duty cycle)*/
    if( NULL != MOTOR_SetSharedAngleGetter)
    {
         MOTOR_SetSharedAngleGetter(SpeedControl_GetAngle); /* Set the callback pointer (whether i`m setting in fake or real !!)*/
    }
}

/**
 * @brief The main function of the module , this function is supposed to called
 *        periodically to update the speed states according to the switches state
 */
void SpeedControl_update(void)
{

    SWITCH_NAME_t Button;
    Button = SWITCH_GetPressedSwitch();     /* get the pressed switch name */

    switch (Button)
    {
        case P_switch:
        {
            if( SpeedState != MINIMUM)
            {   /*if the current speed is NOT acually the minimum */
                SpeedState--;
            }   
            break;
        }
        case MV_switch:
        {
            if( SpeedState != MINIMUM)
            {   /*if the current speed is NOT acually the minimum */
                SpeedState--;
            }
            break;
        }
        case PV_switch:
        {
            if( SpeedState != MAXIMUM)
            {   /*if the current speed is NOT acually the minimum */
                SpeedState++;
            }
            break;
        }
        default:
                /* Do nothing */
            break;
    }

#if 0
    if( Button == P_switch )
    {
        if( SpeedState != MINIMUM)
        {   /*if the current speed is NOT acually the minimum */
            SpeedState--;
        }
    }

    else if ( Button == MV_switch )
    {
        if( SpeedState != MINIMUM)
        {   /*if the current speed is NOT acually the minimum */
            SpeedState--;
        }
    }

    else if ( Button == PV_switch )
    {
        if( SpeedState != MAXIMUM)
        {   /*if the current speed is NOT acually the minimum */
            SpeedState++;
        }
    }

    else  /* None Pressed */
    {
        /* Do nothing */
    }
#endif

    updateAngle();

}





/********************  Getters of the module **********************/

/**
 * @brief The current Speed State Getter of the SpeedControl module
 *
 * @return SpeedState_t
 * could be one of the following enumulation ( MINIMUM, MEDIUM, or MAXIMUM)
 */
SpeedState_t SpeedControl_GetSpeedState(void)
{
    return SpeedState;
}

/**
 * @brief The current Angle Getter of the SpeedControl module
 *
 * @return unsigned char
 * the current shared angle which is being set
 * at THIS module and used at the motor driver module
 */
unsigned char SpeedControl_GetAngle(void)
{
    return SharedAngle;
}





/**
 * @brief a private function used to update the Shared
 *        angle according to the current SpeedState.
 */
static void updateAngle(void)
{
    switch (SpeedState)
    {
        case MINIMUM:
            SharedAngle = 140;
            break;

        case MEDIUM:
            SharedAngle = 90;
            break;

        case MAXIMUM:
            SharedAngle = 10;
            break;

    }
}