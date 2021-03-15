/* Include Header File */
#include "FakeSwitch.h"
#include <stdio.h>

/* Globals */
#if 1
/* Switches States shared for SpeedControlModule through the getter only*/
static SWITCH_STATE_t PV_State;
static SWITCH_STATE_t MV_State;
static SWITCH_STATE_t P_State;
static unsigned char TimePressed;
#endif


/* Switch Fake Getter Prototype */
static SWITCH_NAME_t  FAKE_SW_getSwState(void);

/* Switch Fake Getter Prototype */
static unsigned char Fake_SW_getTimePressed(void);



/* Helper function to reuse tests */
extern void SWITCH_TEST_getTestData(SWITCH_STATE_t* PV_initial_SW_state, SWITCH_STATE_t* MV_initial_SW_state,
                                SWITCH_STATE_t* P_initial_SW_state, unsigned char* TimePressed,
                                unsigned char test_num);


void FAKE_SW_init(void)
{
    SWITCH_GetPressedSwitch = FAKE_SW_getSwState;
    SWITCH_getTimePressed = Fake_SW_getTimePressed;

    PV_State = SW_RELEASED;
    MV_State = SW_RELEASED;
    P_State = SW_RELEASED;
}


void        Fake_SWITCH_update()
{
    /* Do nothing */
}

/**********************  Getters implementation *****************************/

/* Switch Fake Getter Implementation */
static SWITCH_NAME_t  FAKE_SW_getSwState(void)
{   /* If a higher priority key is pressed the fn returns it ignoring the other keys states */
    if( (P_State == SW_PRESSED) && (TimePressed >= 30) )
    {
        //printf("\nin Getter: P_state = PRESSED\n");
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

/* Switch Fake Getter Implementation */
static unsigned char Fake_SW_getTimePressed(void)
{
    return TimePressed;
}




/******   The Extra functions  (No similar in real module )  *********/

SWITCH_STATE_t    FAKE_SW_GetSwState(SWITCH_NAME_t SwitchName)
{
    switch (SwitchName)
    {
        case PV_switch:
        {
            return PV_State;
        }
        case MV_switch:
        {
            return MV_State;
        }
        case P_switch:
        {
            return P_State;
        }
        default:
             break;
    }
}

void FAKE_SW_setSwState(SWITCH_NAME_t SwitchName ,SWITCH_STATE_t state)
{
    switch(SwitchName)
    {
        case PV_switch:
            PV_State = state;
            break;
        case MV_switch:
            MV_State = state;
            break;
        case P_switch:
            P_State = state;
            //if(state == SW_PRESSED) printf(">> P_STATE set to SW_PRESSED");/*debug*/
            break;
        default:
            printf("Wrong Switch name input");
    }

    //if(SwitchName == P_switch){ printf("\n P_switch state: %d", state);}
}

void       Fake_SW_SetTimePressed(unsigned char Time)
{
    TimePressed = Time;
}


void FAKE_SW_destroy(void)
{
    /* Nothing to destroy, Just Initialize everything again */
    PV_State = SW_RELEASED;
    MV_State = SW_RELEASED;
    P_State = SW_RELEASED;
}