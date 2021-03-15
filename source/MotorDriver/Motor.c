#include "Motor.h"


/* maybe useful for calibration and mapping the SharedAngle
   from SCmodule to the real effictive angle                */
#define RATIO_MAXSPEED (100/100)
#define RATIO_MEDSPEED (100/100)
#define RATIO_MINSPEED (100/100)
#define NULL 0

static unsigned char SharedAngle = 0;
static int effictiveAngle = 0;

/* prototype */
static void MOTOR_writeToHardware(const unsigned angle);
static void MOTOR_SetSharedAngleGetter_real(unsigned char (* pGetSharedAngle)(void));
static unsigned char (*SC_CallBack_GetSharedAnglePtr)(void);
void (* MOTOR_SetSharedAngleGetter)(unsigned char (* pGetSharedAngle)(void));

void MOTOR_init(void)
{
    effictiveAngle = RATIO_MEDSPEED * SharedAngle;
    MOTOR_SetSharedAngleGetter = MOTOR_SetSharedAngleGetter_real;
}



/**
 * @brief in case if the user pass the callback pointer points to NULL, 
 *          the local callback will points to here instead
 * 
 * @return unsigned char : Zero value 
 */
static unsigned char ERROR_HANDLER_RETURN_ZERO(void)
{
    return 0;
}


static void MOTOR_SetSharedAngleGetter_real(unsigned char (* pGetSharedAngle)(void))
{
    if(NULL != pGetSharedAngle)
    {
        SC_CallBack_GetSharedAnglePtr = pGetSharedAngle;
    }
    else
    {
        SC_CallBack_GetSharedAnglePtr = ERROR_HANDLER_RETURN_ZERO();
    }
}




void MOTOR_update(void)
{
    SharedAngle = SC_CallBack_GetSharedAnglePtr();
    switch (SharedAngle)
    {
    case 10:    /* Max speed */
        effictiveAngle = RATIO_MAXSPEED * SharedAngle;     /* just in case we need some sort of mapping to real values*/
        MOTOR_writeToHardware(effictiveAngle);
        break;

    case 90:    /* Med speed */
        effictiveAngle = RATIO_MEDSPEED * SharedAngle;     /* just in case we need some sort of mapping to real values*/
        MOTOR_writeToHardware(effictiveAngle);
        break;

    case 140:    /* Min speed */
        effictiveAngle = RATIO_MINSPEED * SharedAngle;     /* just in case we need some sort of mapping to real values*/
        MOTOR_writeToHardware(effictiveAngle);
        break;

    default:
        printf("wrong angle recieved");
        break;
    }
}



static void MOTOR_writeToHardware(const unsigned angle)
{
    ;  /*calling pwm with angle*/
}



