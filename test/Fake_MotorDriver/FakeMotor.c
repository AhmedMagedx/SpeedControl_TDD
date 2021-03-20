
#include "FakeMotor.h"
#include <string.h>
#include "../unity/unity_fixture.h"

/* maybe useful for calibration and mapping the SharedAngle
   from SCmodule to the real effictive angle                */
#define RATIO_MAXSPEED (100/100)
#define RATIO_MEDSPEED (100/100)
#define RATIO_MINSPEED (100/100)

#ifndef NULL
    #define NULL
#endif // !NULL



/* Helper Function */
void WriteToFile(const unsigned char angle);


/* privates */
static unsigned char SharedAngle = 0;
static int effictiveAngle = 0;




/*prototype*/
static void MOTOR_writeToHardware(const unsigned angle);
static unsigned char (*SC_CallBack_GetSharedAnglePtr)(void);
static void Fake_MOTOR_SetSharedAngleGetter(unsigned char (* pGetSharedAngle)(void));



void FAKE_MOTOR_init(void)
{
    effictiveAngle = RATIO_MEDSPEED * SharedAngle;
    //SC_CallBack_GetSharedAnglePtr = pGetSharedAngle;
    MOTOR_SetSharedAngleGetter = Fake_MOTOR_SetSharedAngleGetter;
}

void FAKE_MOTOR_Deinit(void)
{
    MOTOR_SetSharedAngleGetter = NULL;   /* Pre_init value */
}

/**
 * @brief in case if the user pass the callback pointer points to zero, 
 *          the local callback will points to here instead
 * 
 * @return unsigned char : Zero value 
 */
static unsigned char ERROR_HANDLER_RETURN_ZERO(void)
{
    return 0;
}


static void Fake_MOTOR_SetSharedAngleGetter(unsigned char (* pGetSharedAngle)(void))
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




void FAKE_MOTOR_update(void)
{

    SharedAngle = SC_CallBack_GetSharedAnglePtr();
    switch (SharedAngle)
    {
    case 10:
        effictiveAngle = RATIO_MAXSPEED * SharedAngle;     /* just in case we need some sort of mapping to real values*/
        FakeMOTOR_writeToHardware(effictiveAngle);
        break;

    case 90:
        effictiveAngle = RATIO_MEDSPEED * SharedAngle;     /* just in case we need some sort of mapping to real values*/
        FakeMOTOR_writeToHardware(effictiveAngle);
        break;

    case 140:
        effictiveAngle = RATIO_MINSPEED * SharedAngle;     /* just in case we need some sort of mapping to real values*/
        FakeMOTOR_writeToHardware(effictiveAngle);
        break;

    default:
        printf("wrong angle recieved");
        break;
    }
}



void MOTOR_writeToHardware(const unsigned angle)
{
    ;  /*calling pwm with angle*/
}


void FakeMOTOR_writeToHardware(const unsigned angle)
{
    /*write to the file*/
    WriteToFile(angle);
}



void WriteToFile(const unsigned char angle)
{
    FILE* f = fopen("MotorAngles.txt","a");
    if(f)
    {
        fprintf(f,"%d\n",angle);
        fclose(f);
    }
}


unsigned char FAKE_MOTOR_GetAngle(void)
{
    return SharedAngle;
}
