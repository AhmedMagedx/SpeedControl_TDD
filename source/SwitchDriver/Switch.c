
/**
 * @file Switch.c
 * @author Ahmed Maged
 * @date 12/03/2021
 * @brief File containing the implementaion of the switch driver
 */

#include "switch.h"

/* Globals */

/* Switches States shared for SpeedControlModule through the getter only*/
static SWITCH_STATE_t PV_State;     /* Switch state for the Plus Button */
static SWITCH_STATE_t MV_State;     /* Switch state for the Minus Button */
static SWITCH_STATE_t P_State;      /* Switch state for the pressure Button */
static unsigned char TimePressed;   /* the pressure Button pressed time   */



/* Switch Real Getter Prototype */
static SWITCH_NAME_t  SWITCH_getSwState_Real(void);
static unsigned char SWITCH_getTimePressed_Real(void);


/*********************** Global Functions Implementation ***********************/

/**
 * @brief the initialization function 
 * 
 */
void       SWITCH_init(void)
{
    SWITCH_GetPressedSwitch = SWITCH_getSwState_Real;       /* Set the pressed switch getter  to the real Getter*/
    SWITCH_getTimePressed = SWITCH_getTimePressed_Real;     /* Set the pressure pressed time getter to the real Getter*/

    PV_State = SW_RELEASED;         /* initial State */
    MV_State = SW_RELEASED;         /* initial State */
    P_State = SW_RELEASED;          /* initial State */
}


/**
 * @brief the periodic function of the switch 
 * it used to update the switch states
 */
void        SWITCH_update(void)
{
    /* Do nothing for now */
    /* read the hardware  */
    /* and update the real switch states*/
}



/**********  Getters implementation **************/

/*  */
/**
 * @brief Switch Getter Implementation
 * 
 * @return SWITCH_NAME_t the name of the pressed switch is returned 
 *          (a value from this enumelation {PV_switch, MV_switch, P_switch, NONE})
 */
static SWITCH_NAME_t  SWITCH_getSwState_Real(void)
{   /* If a higher priority key is pressed the fn returns it ignoring the other keys states */
    if( (P_State == SW_PRESSED) & (TimePressed >= 30) )
    {
        return P_switch;
    }
    else if ( MV_State == SW_PRESSED )
    {
        return MV_switch;
    }
    else if ( PV_State == SW_PRESSED )
    {
        return PV_switch;
    }
    else
    {   /* None pressed */
        return NONE;
    }

}

/**
 * @brief the pressed time of the pressure switch
 * 
 * @return unsigned char : the pressed time in ms 
 */
static unsigned char SWITCH_getTimePressed_Real(void)
{
    return TimePressed;
}

