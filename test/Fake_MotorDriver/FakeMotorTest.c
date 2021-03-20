#include "../Fake_SwitchDriver/FakeSwitch.h"  /* Fake to get data */
//#include "../../source/SpeedControlModule/SpeedControlModule.h"     /* Real to test */
#include "FakeMotor.h"      /* Fake to write data to the file */
#include "../unity/unity_fixture.h"

#define SET_STATE_TO_MIN SpeedControl_init();\
                        FAKE_SW_setSwState(PV_switch, SW_RELEASED);\
                        FAKE_SW_setSwState(MV_switch, SW_PRESSED );\
                        FAKE_SW_setSwState(P_switch,  SW_RELEASED);\
                        SpeedControl_update()

#define SET_STATE_TO_MED SpeedControl_init();\
                        SpeedControl_update()

#define SET_STATE_TO_MAX SpeedControl_init();\
                        FAKE_SW_setSwState(PV_switch, SW_PRESSED);\
                        FAKE_SW_setSwState(MV_switch, SW_RELEASED);\
                        FAKE_SW_setSwState(P_switch, SW_RELEASED);\
                        SpeedControl_update()







/*/************** debug only ************************/
#if 0
#include <stdio.h>
#include <string.h>
#include "../Fake_SwitchDriver/FakeSwitch.h"
extern SpeedState_t SpeedState;

/* Switches States Global and shared for SpeedControlModule*/
extern SWITCH_STATE_t PV_State;
extern SWITCH_STATE_t MV_State;
extern SWITCH_STATE_t P_State;
extern unsigned char TimePressed;
void DEBUG_PRINT(void);
#endif


TEST_GROUP(FakeMotor);


TEST_SETUP(FakeMotor)
{
    FAKE_MOTOR_init();    /* set callback in the fake module*/
    //printf("\n**************HERE****************\n");
    //MOTOR_SetSharedAngleGetter( SpeedControl_GetAngle );
    //printf("\n**************HERE****************\n");
    SpeedControl_init();  /* State >> MED ,  Angle >> 90*/
}


TEST_TEAR_DOWN(FakeMotor)
{
    FAKE_MOTOR_Deinit();
}


/* Helper Function */
void WriteToFile(const unsigned char);
unsigned char GetFromTheFile(unsigned char testNum);

TEST(FakeMotor, receiveAngleFromSC_CLK)
{
    FAKE_MOTOR_update();
    LONGS_EQUAL(90, FAKE_MOTOR_GetAngle());
}




#if 0 /***++++++++++++++++++++++++++++++++++++++++++***/


TEST(FakeMotor, receiveAngleFromSCmoduleDefualt)
{
    /*!
        * @par Given : Text File to write the angles is exist in the same directory
        * @par When  : switchesUpdate and SCmoduleUpdate are called
        * @par Then  :
	*/

    pfSWITCH_update(1);
    SpeedControl_update();
    FAKE_MOTOR_update();

    LONGS_EQUAL(90, GetFromTheFile(1));
}


TEST(FakeMotor, fromMedToMaxWithPV)
{
    /*!
        * @par Given : Current angle 90 (med) and SCmodule has recentlly updated the angle
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the new angle should be written to the file
	*/

    SET_STATE_TO_MED;

    pfSWITCH_update(2);
    SpeedControl_update();
    FAKE_MOTOR_update();

    LONGS_EQUAL(10, GetFromTheFile(2));
}

TEST(FakeMotor, noChangeWhenPVbuttonWhileMax)
{
    /*!
        * @par Given : Current angle 10 (max) and +V pressed
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should still be written 10.
	*/

    SET_STATE_TO_MAX;
    pfSWITCH_update(3);
    SpeedControl_update();
    FAKE_MOTOR_update();
    //DEBUG_PRINT();

    LONGS_EQUAL(10, GetFromTheFile(3));
}

TEST(FakeMotor, noChangeWhenPbuttonAndInvalidTimeWhileMax)
{
    /*!
        * @par Given : Current angle 10 (max) and p pressed with time less 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should still be written 10.
	*/

    SET_STATE_TO_MAX;


    //DEBUG_PRINT();  /* print all info */

    pfSWITCH_update(4);
    SpeedControl_update();
    FAKE_MOTOR_update();

    //DEBUG_PRINT();  /* print all info */

    LONGS_EQUAL(10, GetFromTheFile(4));
}

TEST(FakeMotor, FromMaxToMedWhenPbuttonAndTime30)
{
    /*!
        * @par Given : Current angle 10 (max) and p pressed with time 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should be 90
	*/

    SET_STATE_TO_MAX;

    pfSWITCH_update(5);
    SpeedControl_update();
    FAKE_MOTOR_update();


    //DEBUG_PRINT();  /* print all info */

    LONGS_EQUAL(90, GetFromTheFile(5));

}


TEST(FakeMotor, FromMedToMinWhenPbuttonAndTime31)
{
    /*!
        * @par Given : Current angle 90 (med) and p pressed with time larger than 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should be 140
	*/

    SET_STATE_TO_MED;

    pfSWITCH_update(6);
    SpeedControl_update();
    FAKE_MOTOR_update();


    //DEBUG_PRINT();  /* print all info */

    LONGS_EQUAL(140, GetFromTheFile(6));

}


TEST(FakeMotor, noChangeWhenPbuttonAndValidTimeWhileMin)
{
    /*!
        * @par Given : Current angle 90 (med) and p pressed with time larger than 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should be 140
	*/

    SET_STATE_TO_MIN;

    pfSWITCH_update(6);
    SpeedControl_update();
    FAKE_MOTOR_update();

    //DEBUG_PRINT();  /* print all info */

    LONGS_EQUAL(140, GetFromTheFile(6));

}


TEST(FakeMotor, fromMinToMedWithPV)
{
    /*!
        * @par Given : Current angle 90 (med) and p pressed with time larger than 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should be 140
	*/

    SET_STATE_TO_MIN;

    //DEBUG_PRINT();

    pfSWITCH_update(2);
    SpeedControl_update();
    FAKE_MOTOR_update();


    //DEBUG_PRINT();  /* print all info */

    LONGS_EQUAL(90, GetFromTheFile(8));

}



TEST(FakeMotor, fromMaxToMedWithMV)
{
    /*!
        * @par Given : Current angle 90 (med) and p pressed with time larger than 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should be 140
	*/

    SET_STATE_TO_MAX;

    pfSWITCH_update(7);
    SpeedControl_update();
    FAKE_MOTOR_update();

    //DEBUG_PRINT();  /* print all info */
    LONGS_EQUAL(90, GetFromTheFile(9));

}

TEST(FakeMotor, fromMedToMinWithMV)
{
    /*!
        * @par Given : Current angle 90 (med) and p pressed with time larger than 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should be 140
	*/

    SET_STATE_TO_MED;

    pfSWITCH_update(7);
    SpeedControl_update();
    FAKE_MOTOR_update();

    //DEBUG_PRINT();  /* print all info */
    LONGS_EQUAL(140, GetFromTheFile(10));

}

TEST(FakeMotor, noChangeWhenMVbuttonWhileMin)
{
    /*!
        * @par Given : Current angle 90 (med) and p pressed with time larger than 30
        * @par When  : FAKE_MOTOR_update are called
        * @par Then  : the angle should be 140
	*/

    SET_STATE_TO_MIN;

    pfSWITCH_update(7);
    SpeedControl_update();
    FAKE_MOTOR_update();

    //DEBUG_PRINT();  /* print all info */
    LONGS_EQUAL(140, GetFromTheFile(11));

}


#endif /***++++++++++++++++++++++++++++++++++++++++++***/

TEST_GROUP_RUNNER(FakeMotor)
{

    RUN_TEST_CASE(FakeMotor, receiveAngleFromSC_CLK);

#if 0 /***++++++++++++++++++++++++++++++++++++++++++***/
    RUN_TEST_CASE(FakeMotor, receiveAngleFromSCmoduleDefualt);
    RUN_TEST_CASE(FakeMotor, fromMedToMaxWithPV);
    RUN_TEST_CASE(FakeMotor, noChangeWhenPVbuttonWhileMax);
    RUN_TEST_CASE(FakeMotor, noChangeWhenPbuttonAndInvalidTimeWhileMax);
    RUN_TEST_CASE(FakeMotor, FromMaxToMedWhenPbuttonAndTime30);
    RUN_TEST_CASE(FakeMotor, FromMedToMinWhenPbuttonAndTime31);
    RUN_TEST_CASE(FakeMotor, noChangeWhenPbuttonAndValidTimeWhileMin);
    RUN_TEST_CASE(FakeMotor, fromMinToMedWithPV);
    RUN_TEST_CASE(FakeMotor, fromMaxToMedWithMV);
    RUN_TEST_CASE(FakeMotor, fromMedToMinWithMV);
    RUN_TEST_CASE(FakeMotor, noChangeWhenMVbuttonWhileMin);
#endif /***++++++++++++++++++++++++++++++++++++++++++***/

}

#if 0
void WriteToFile(const unsigned char angle)
{
    FILE* f = fopen("MotorAngles.txt","a");
    if(f)
    {
        fprintf(f,"%d\n",angle);
        fclose(f);
    }
}
#endif

#if 0
static unsigned char GetFromTheFile(unsigned char testNum)
{
    FILE* fr = fopen("MotorAngles.txt","r+");
    char CharAngle[3] ;

    if(fr)
    {

        for(char i=1; i<= testNum; i++)
        {
            memset(CharAngle, 0, 3);
            fscanf(fr,"%s\n",&CharAngle);
        }

        fclose(fr);

    }
    else
    {
        UnityPrint("Can not open the file");
    }
    //printf("\nAngle: %s\n", CharAngle);
    return atoi(CharAngle);
}
#endif

#if 0
/*for proper work make sure that every variable this function is
  trying to reach and read is public and not static
  These variable are:
         PV_State, MV_State, and P_State  from Fake Switch Module.
         SpeedState                       from SpeedControl module.
         SharedAngle                      from SpeedControl module.

  otherwise the function will throw an exeption.
*/
void DEBUG_PRINT(void)
{

    char str1[20];
    char str2[20];
    char str3[20];
    char str4[20];

    memset(str1, 0, 20);
    memset(str2, 0, 20);
    memset(str3, 0, 20);
    memset(str4, 0, 20);

    switch (PV_State)
    {
    case   SW_RELEASED:
        strcpy(str1, "SW_RELEASED");
        break;

    case   SW_PRESSED:
        strcpy(str1, "SW_PRESSED");
        break;

    case   SW_PRERELEASED:
        strcpy(str1, "SW_PRERELEASED");
         break;

    case   SW_PREPRESSED:
        strcpy(str1, "SW_PREPRESSED");
        break;

    default:
        break;
    }




        switch (MV_State)
    {
    case   SW_RELEASED:
        strcpy(str2, "SW_RELEASED");
         break;

    case   SW_PRESSED:
        strcpy(str2, "SW_PRESSED");
        break;

    case   SW_PRERELEASED:
        strcpy(str2, "SW_PRERELEASED");
         break;

    case   SW_PREPRESSED:
        strcpy(str2, "SW_PREPRESSED");
        break;

    default:
        break;
    }


    switch (P_State)
    {
    case   SW_RELEASED:
        strcpy(str3, "SW_RELEASED");
         break;

    case   SW_PRESSED:
        strcpy(str3, "SW_PRESSED");
        break;

    case   SW_PRERELEASED:
        strcpy(str3, "SW_PRERELEASED");
         break;

    case   SW_PREPRESSED:
        strcpy(str3, "SW_PREPRESSED");
        break;

    default:
        break;
    }

    printf("\n*****************************************************************\n");
    printf("PV state: %s, MV state: %s, P state: %s, Time: %d\n",str1, str2, str3, TimePressed);


    switch (SpeedState)
    {
    case   MINIMUM:
        strcpy(str4, "MINIMUM");
         break;

    case   MEDIUM:
        strcpy(str4, "MEDIUM");
        break;

    case   MAXIMUM:
        strcpy(str4, "MAXIMUM");
         break;

    default:
        break;
    }

    printf("SpeedState: %s\n",str4);

    printf("Angle: %d\n",SharedAngle);

    printf("\n*****************************************************************\n");


}

#endif
