/* HI */

#include "../../source/SpeedControlModule/SpeedControlModule.h"
/* Fake */
#include "../Fake_MotorDriver/FakeMotor.h"
#include "../Fake_SwitchDriver/FakeSwitch.h"

/* Real */
#include "../../source/MotorDriver/Motor.h"
#include "../../source/SwitchDriver/Switch.h"

#include "../unity/unity_fixture.h"
#include <string.h>



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

#define CurrentAngle      SpeedControl_GetAngle()
#define CurrentSpeedState SpeedControl_GetSpeedState()



static SWITCH_STATE_t PV_State;
static SWITCH_STATE_t MV_State;
static SWITCH_STATE_t P_State;
static unsigned char TimePressed;
static unsigned char TestLineNum = 1;       /* The number of test lines in the file*/
static unsigned char TestLineCurrent = 0;  /* The current line we run */
static SpeedState_t intialSpeedState;
static unsigned char ExpectedAngle;  /* read from input file*/

static unsigned char GetFromTheFile(unsigned char LineNum);
static char gCLEAR_FILE = 1;



/**************************** Helper functions to reuse tests ****************/
static void getTestData(    SpeedState_t* intialSpeedState , SWITCH_STATE_t* PV_SW_state,
                            SWITCH_STATE_t* MV_SW_state,     SWITCH_STATE_t* P_SW_state,
                            unsigned char* TimePressed,      unsigned char* ExpectedAngle);

static void FileRead_SpeedControl_FakeSwitch_Arrange(void);
static char *  Help_StrPressedButton(void);
static char *  Help_StrPressedSpeedState(void);
static char *  Help_StrSwitchState(SWITCH_NAME_t loc_SN);
void FileReadAndFakeSwitchArrange(void);
/******************************************************************************/



/***************************  SC_GetSpeedState  ************************************/

TEST_GROUP(SC_GetSpeedState);

TEST_SETUP(SC_GetSpeedState)
{

}

TEST_TEAR_DOWN(SC_GetSpeedState)
{
    /* return to the defualt state as the test is never done after tect completion*/

}


TEST(SC_GetSpeedState, PreInitialization)
{
        /*!
		  * @par Given : SC module is not initialized
		  * @par When  : SpeedControl_GetSpeedState
		  * @par Then  : the return should be th pre_initialization values MINIMUM
	    */
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() ); /* pre_int value */

}


TEST_GROUP_RUNNER(SC_GetSpeedState)
{
    RUN_TEST_CASE(SC_GetSpeedState, PreInitialization);
}
/****************************** END SC_GetSpeedState ***********************************/



/***************************  SC_GetSharedAngle  ************************************/


TEST_GROUP(SC_GetSharedAngle);

TEST_SETUP(SC_GetSharedAngle)
{

}

TEST_TEAR_DOWN(SC_GetSharedAngle)
{
    /* return to the defualt state as the test is never done after tect completion*/

}


TEST(SC_GetSharedAngle, PreInitialization)
{
        /*!
		  * @par Given : SC module is not initialized
		  * @par When  : SpeedControl_GetAngle
		  * @par Then  : the return should be th pre_initialization value 0
	    */
    LONGS_EQUAL(0,  SpeedControl_GetAngle() ); /* pre_int value */

}



TEST(SC_GetSharedAngle, AfterInitialization)
{
    SpeedControl_init();
    LONGS_EQUAL(90,  SpeedControl_GetAngle() ); /* default after initialization value */

}

TEST_GROUP_RUNNER(SC_GetSharedAngle)
{
    RUN_TEST_CASE(SC_GetSharedAngle, PreInitialization);
    RUN_TEST_CASE(SC_GetSharedAngle, AfterInitialization);
}

/****************************** END SC_GetSharedAngle ***********************************/


/***************************  SpeedControlInit  ************************************/


TEST_GROUP(SpeedControlInit);

TEST_SETUP(SpeedControlInit)
{
    /* SC init */
    SpeedControl_init();

}

TEST_TEAR_DOWN(SpeedControlInit)
{
    /* return to the defualt state as the test is never done after tect completion*/
}


TEST(SpeedControlInit, IntialValues)
{
        /*!
		  * @par Given : SC module is not initialized
		  * @par When  : SpeedControl_init
		  * @par Then  : SpeedState and Angle should be the default values MEDIUM and 90
	    */
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() ); /* Now it must be MEDIUM after init */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );          /* Now it must be 90 after init */
}

TEST_GROUP_RUNNER(SpeedControlInit)
{
    RUN_TEST_CASE(SpeedControlInit, IntialValues);
}


/****************************** END SpeedControlInit ***********************************/




/* ************************ SpeedControl Update *********************/
static unsigned char GetFromTheFile(unsigned char LineNum);
static void FakeSWarrange(const SWITCH_NAME_t cpyPV,const SWITCH_NAME_t cpyMV,const SWITCH_NAME_t cpyP,const unsigned char time);


TEST_GROUP(SpeedControlUpdate);

TEST_SETUP(SpeedControlUpdate)
{
    /* SC init */
    /* Fake switch init */
    FAKE_SW_init();         /* Make the SwitchGetter points to the fakeGetter*/
    SpeedControl_init();

#if 1
    /*clear the old values stored in the file*/
    if(gCLEAR_FILE)
    {
        FILE* fr = fopen("MotorAngles.txt","w");
        if(fr)
        {
            fclose(fr);
            gCLEAR_FILE = 0;
        }
        else
        {
            printf("\nCan not find the file MotorAngles.txt\nPlease make sure and try again");
        }
    }
#endif
}

TEST_TEAR_DOWN(SpeedControlUpdate)
{
    /* return to the defualt state as the test is never done after tect completion*/
    //printf("\nTEST_TEAR_DOWN\n");
    gCLEAR_FILE = 1;
    FAKE_MOTOR_Deinit();

}



TEST(SpeedControlUpdate, ReadSW_MotorUpdateAngle)
{
        /*!
		  * @par Given : SC module is not initialized
		  * @par When  : get the pressed switch (NONE), then update the SC and motor
		  * @par Then  : the file should contain the default value 90
	    */
    SpeedControl_update();
    FAKE_MOTOR_update();
    LONGS_EQUAL(90, GetFromTheFile(1) );  /* The first line in the output file */
}

    /**
     * 1-N state coverage >> 4 Test Cases:
     *      1 - MED > MIN > MED > MIN.
     *      2 - MED > MIN > MED > MAX.
     *      3 - MED > MAX > MED > MIN.
     *      4 - MED > MAX > MED > MAX.
     */

TEST(SpeedControlUpdate, N1StateTC1)
{
    /* 1 - MED > MIN > MED > MIN. */
        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : SpeedControl_GetSpeedState
		  * @par Then  : angle should contain the default value 90
	    */
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );   /* Assert HERE ?!! */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : MinusV pressed
		  * @par Then  : values should be MINIMUM, 140
	    */
    FakeSWarrange( SW_RELEASED, SW_PRESSED, SW_RELEASED, 0);   /* MinusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(140, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MINIMUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MEDIUM, 90
	    */
    FakeSWarrange( SW_PRESSED, SW_RELEASED, SW_RELEASED, 0);   /* PlusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(90, SpeedControl_GetAngle() );

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : P pressed & time >= 30
		  * @par Then  : values should be MINIMUM, 140
	    */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 50);   /* P pressed & time > 30 */
    SpeedControl_update();
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(140, SpeedControl_GetAngle() );
}


TEST(SpeedControlUpdate, N1StateTC2)
{
    /* 2 - MED > MIN > MED > MAX. */

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  :
		  * @par Then  : values should be MEDIUM, 90
	    */
        /* current State is MED after init */
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );   /* Assert HERE ?!! */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : MinusV pressed
		  * @par Then  : values should be MINIMUM, 140
	    */
    FakeSWarrange( SW_RELEASED, SW_PRESSED, SW_RELEASED, 0);   /* MinusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(140, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MINIMUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MEDIUM, 90
	    */
    FakeSWarrange( SW_PRESSED, SW_RELEASED, SW_RELEASED, 0);   /* PlusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(90, SpeedControl_GetAngle() );

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MAXIMUM, 10
	    */
    FakeSWarrange( SW_PRESSED, SW_RELEASED, SW_RELEASED, 0);   /* PlusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MAXIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(10, SpeedControl_GetAngle() );
}

TEST(SpeedControlUpdate, N1StateTC3)
{
    /* 3 - MED > MAX > MED > MIN. */

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  :
		  * @par Then  : values should be MEDIUM, 90
	    */
    /* current State is MED after init */
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );   /* Assert HERE ?!! */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MAXIMUM, 10
	    */
    FakeSWarrange( SW_PRESSED, SW_RELEASED, SW_RELEASED, 0);   /* PlusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MAXIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(10, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MAXIMUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MEDIUM, 90
	    */
    FakeSWarrange( SW_RELEASED, SW_PRESSED, SW_RELEASED, 0);   /* MinusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(90, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MINIMUM, 140
	    */
    FakeSWarrange( SW_RELEASED, SW_PRESSED, SW_RELEASED, 0);   /* MinusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(140, SpeedControl_GetAngle() );
}



TEST(SpeedControlUpdate, N1StateTC4)
{
    /* 4 - MED > MAX > MED > MAX. */

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  :
		  * @par Then  : values should be MEDIUM, 90
	    */
    /* current State is MED after init */
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );   /* Assert HERE ?!! */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MAXIMUM, 10
	    */
    FakeSWarrange( SW_PRESSED, SW_RELEASED, SW_RELEASED, 0);   /* PlusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MAXIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(10, SpeedControl_GetAngle() );

        /*!
		  * @par Given : state is MAXIMUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MEDIUM, 90
	    */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 50);   /* P pressed & time > 30 */
    SpeedControl_update();
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(90, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should be MAXIMUM, 10
	    */
    FakeSWarrange( SW_PRESSED, SW_RELEASED, SW_RELEASED, 0);   /* PlusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MAXIMUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(10, SpeedControl_GetAngle() );

}



TEST(SpeedControlUpdate, N1StateINVALIDTC5)
{

            /*!
		  * @par Given : state is MEDIUM
		  * @par When  :
		  * @par Then  : values should be MAXIMUM, 10
	    */
        /* current State is MED after init */

    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );   /* Assert HERE ?!! */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );

        /*!
		  * @par Given : state is MINIMUM
		  * @par When  : MinusV pressed or P pressed & time > 30
		  * @par Then  : values should MINIMUM, 140
	    */
    SET_STATE_TO_MIN;       /* Set to min */
    FakeSWarrange( SW_RELEASED, SW_PRESSED, SW_RELEASED, 0);   /* MinusV pressed */
    SpeedControl_update();
    /* Or */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 50);   /* P pressed & time > 30 */
    SpeedControl_update();
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() );  /* Should not change */
    LONGS_EQUAL(140, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MAXIMUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should MAXIMUM, 10
	    */
    SET_STATE_TO_MAX;       /* Set to max */
    FakeSWarrange( SW_PRESSED, SW_RELEASED, SW_RELEASED, 0);   /* PlusV pressed */
    SpeedControl_update();
    LONGS_EQUAL(MAXIMUM, SpeedControl_GetSpeedState() );    /* Should not change */
    LONGS_EQUAL(10, SpeedControl_GetAngle() );              /* Should not change */

}


/**
 *@par  EP + BVA :
 *@par      Time input: Valid range: from 30 to max (255 / u8) and from (0) to 29
 *@par                  invalid range: from -INF to -1  and from 256 to inf
 *@par      pressed switch input: Valid range: from 0 to 3    >> +v, -v, p, NONE
 *@par                            invalid range: from -1 to -INFINITY
 *@par                            invalid range: from 4 to INFINITY
 */

/* This is not invalid!!!! , invalid is {-inf,...,-1} or {266, inf}*/
TEST(SpeedControlUpdate, EP_TC1_INVALID)
{

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : PlusV pressed
		  * @par Then  : values should MEDIUM, 90
	    */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 15);   /* p pressed & time = 15 */
    SpeedControl_update();

    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );  /* Should not change */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );          /* Should not change */

}


TEST(SpeedControlUpdate, EP_TC2_VALID)
{

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : p pressed & time = 143
		  * @par Then  : values should MINIMUM, 140
	    */
    /* Arrange */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 143);   /* p pressed & time = 143 */

    /* Act */
    SpeedControl_update();

    /* Assert */
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() );  /* Should change */
    LONGS_EQUAL(140, SpeedControl_GetAngle() );          /* Should change */

}

/* not invalid too !!*/
TEST(SpeedControlUpdate, BVA_TC1_INVALID)
{

        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : p pressed & time = 143
		  * @par Then  : values should MEDIUM, 90
	    */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 0);   /* p pressed & time = 0 */
    SpeedControl_update();
    /* Or */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 29);   /* p pressed & time = 29 */
    SpeedControl_update();
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );  /* Should not change */
    LONGS_EQUAL(90, SpeedControl_GetAngle() );          /* Should not change */

}

TEST(SpeedControlUpdate, BVA_TC2_VALID)
{

        /*!
		  * @par Given : state is MAXIMUM
		  * @par When  : p pressed & time = 30
		  * @par Then  : values should MEDIUM, 90
	    */
    SET_STATE_TO_MAX;  /* it should be added to arrange function */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 30);   /* p pressed & time = 30 */
    SpeedControl_update();     /* Should decreased to med */
    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(90, SpeedControl_GetAngle() );


        /*!
		  * @par Given : state is MEDIUM
		  * @par When  : p pressed & time = 30
		  * @par Then  : values should MINIMUM, 140
	    */
    FakeSWarrange( SW_RELEASED, SW_RELEASED, SW_PRESSED, 255);   /* p pressed & time = 255 */
    SpeedControl_update();      /* Should decreased to min */
    LONGS_EQUAL(MINIMUM, SpeedControl_GetSpeedState() );  /* Should decreased to min */
    LONGS_EQUAL(140, SpeedControl_GetAngle() );

}






#if 0
TEST(SpeedControlUpdate, ReadSW_MotorUpdateAngle)
{
    printf( "\nSpeedState : %s\n", SpeedStateString() );
    FAKE_MOTOR_update();

    LONGS_EQUAL(MEDIUM, SpeedControl_GetSpeedState() );
    LONGS_EQUAL(90, GetFromTheFile(TestLineCurrent) );
}
#endif

TEST_GROUP_RUNNER(SpeedControlUpdate)
{
    RUN_TEST_CASE(SpeedControlUpdate, ReadSW_MotorUpdateAngle);
    RUN_TEST_CASE(SpeedControlUpdate, N1StateTC1);
    RUN_TEST_CASE(SpeedControlUpdate, N1StateTC2);
    RUN_TEST_CASE(SpeedControlUpdate, N1StateTC3);
    RUN_TEST_CASE(SpeedControlUpdate, N1StateTC4);
    RUN_TEST_CASE(SpeedControlUpdate, N1StateINVALIDTC5);
    RUN_TEST_CASE(SpeedControlUpdate, EP_TC1_INVALID);
    RUN_TEST_CASE(SpeedControlUpdate, EP_TC2_VALID);
    RUN_TEST_CASE(SpeedControlUpdate, BVA_TC1_INVALID);
    RUN_TEST_CASE(SpeedControlUpdate, BVA_TC2_VALID);
}


/******************** END SpeedControl update******************************/




/**********************    Test lines from file    ***********************/
TEST_GROUP(InputTestCases);

TEST_SETUP(InputTestCases)
{

    FAKE_SW_init();
    SpeedControl_init();
    FAKE_MOTOR_init();

    /*clear the old values stored in the file*/
    if(gCLEAR_FILE)
    {
        FILE* fr = fopen("MotorAngles.txt","w");
        if(fr)
        {
            fclose(fr);
            gCLEAR_FILE = 0; /* it will never set again to keep the data written in the file */
        }
        else
        {
            printf("\nCan not find the file MotorAngles.txt\nPlease make sure and try again");
        }
    }
}

TEST_TEAR_DOWN(InputTestCases)
{
    FAKE_MOTOR_Deinit();
}


/* Test Cases */
/*           */
TEST(InputTestCases, TC1)
{
    TestLineCurrent++;
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}

TEST(InputTestCases, TC2)
{

    TestLineCurrent++;
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );

}

#if 1
TEST(InputTestCases, TC3)
{
    TestLineCurrent++;  /* = 3*/
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}

TEST(InputTestCases, TC4)
{
    TestLineCurrent++;  /* = 4*/
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}

TEST(InputTestCases, TC5)
{
    TestLineCurrent++;  /* = 5*/
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}

TEST(InputTestCases, TC6)
{
    TestLineCurrent++;  /* = 5*/
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}

TEST(InputTestCases, TC7)
{
    TestLineCurrent++;
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}

TEST(InputTestCases, TC8)
{
    TestLineCurrent++;
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}


/*********** PS: you can add test lines as much as you want But:
1 - cpy the a test case and rename it (as follows in test case TCx)
2 - cpy runner in the TEST_GROUP_RUNNER with the new name (check TEST_GROUP_RUNNER)*/
#if 0
TEST(InputTestCases, TCx)
{
    TestLineCurrent++;
    FileRead_SpeedControl_FakeSwitch_Arrange();
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf("    Current Line %d     ", TestLineCurrent);
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}
#endif



#endif

TEST_GROUP_RUNNER(InputTestCases)
{
    RUN_TEST_CASE(InputTestCases, TC1);
    RUN_TEST_CASE(InputTestCases, TC2);
    RUN_TEST_CASE(InputTestCases, TC3);
    RUN_TEST_CASE(InputTestCases, TC4);
    RUN_TEST_CASE(InputTestCases, TC5);
    RUN_TEST_CASE(InputTestCases, TC6);
    RUN_TEST_CASE(InputTestCases, TC7);
    RUN_TEST_CASE(InputTestCases, TC8);
#if 0
    RUN_TEST_CASE(InputTestCases, TCx);
#endif
}

/**********************    END Test lines from file         **************/



/*********************** Helper Functions *********************************/

static char *  Help_StrPressedButton(void)
{
    SWITCH_NAME_t locSN = SWITCH_GetPressedSwitch();
    switch( locSN )
    {
        case PV_switch:  return "PV_switch";
        case MV_switch:  return "MV_switch";
        case P_switch:  return "P_switch";
        case NONE:  return "NONE";
    }
    return -1; /* if error only */
}

static char *  Help_StrSwitchState(SWITCH_NAME_t loc_SN)
{
    SWITCH_STATE_t locSS = FAKE_SW_GetSwState(loc_SN);
    switch( locSS )
    {
        case SW_PRESSED:     return "SW_PRESSED";
        case SW_PREPRESSED:  return "SW_PREPRESSED";
        case SW_RELEASED:  return "SW_RELEASED";
        case SW_PRERELEASED:     return "SW_PRERELEASED";
    }
    return -1; /* if error only */
}


static char *  Help_StrSpeedState(void)
{
    SpeedState_t locSS = SpeedControl_GetSpeedState();
    switch( locSS )
    {
        case MINIMUM:  return "MINIMUM";
        case MAXIMUM:  return "MAXIMUM";
        case MEDIUM:  return "MEDIUM";
    }
}


static void FileRead_SpeedControl_FakeSwitch_Arrange(void)
{

    getTestData(&intialSpeedState, &PV_State, &MV_State, &P_State, &TimePressed, &ExpectedAngle);

    /* IMPORTANT: SpeedControl Arrange MUST be before FakeSwitch Arrange */

    /* SpeedControl  Arrange*/
    switch (intialSpeedState)
    {
        case MINIMUM:
        {
            SET_STATE_TO_MIN;
            break;
        }
        case MEDIUM:
        {
            SET_STATE_TO_MED;
            break;
        }
        case MAXIMUM:
        {
            SET_STATE_TO_MAX;
            break;
        }
        default:
            /* ignore for now */
        break;
    }

    /* debug only */
    //if(P_State == SW_PRESSED){printf("\n**** P pressed *****\n"); }

    /* Fake switch  Arrange */
    FAKE_SW_setSwState(PV_switch, PV_State);
    FAKE_SW_setSwState(MV_switch, MV_State);
    FAKE_SW_setSwState(P_switch, P_State);
    Fake_SW_SetTimePressed(TimePressed);
    //printf("\n    Current SpeedState %s    PressedSW: %s    Psw: %s  Time: %d\n", Help_StrSpeedState() , Help_StrPressedButton(), Help_StrSwitchState(P_switch), TimePressed );

}


static void getTestData(    SpeedState_t* intialSpeedState , SWITCH_STATE_t* PV_SW_state,
                            SWITCH_STATE_t* MV_SW_state,     SWITCH_STATE_t* P_SW_state,
                            unsigned char* TimePressed,      unsigned char* ExpectedAngle)
{
    FILE* f = fopen("SwitchesTestData.txt","r+");

    /*  I think the loc variables cause an stack overflow(which lead to weired
        undefined behaviour), so i`m using static to be stored in heap bss region*/

    static char str2[20];
    static char str1[20];
    static char str3[20];
    char loc_ExpectedAngle[3];   /* readed as string because when tring to read string and decimal using fscan and a failure happened for no known reason */
    char loc_Time[3];
    static char loc_initSS[20];

    if(f)
    {
        /* get test line*/
        char CharTestNum[2];                /* Max Test Lines is 99 */
        fscanf(f,"%s\n", CharTestNum);
        TestLineNum = (char)atoi(CharTestNum);

        //printf("\nNumber of Test lines %d\n", TestLineNum);

        /*just get the specific line corresponds to the intended test case */
        for(unsigned char i = 1; i <= TestLineCurrent; i++)
        {
            //memset(loc_intial,0,20);
            memset(str1,0,20);
            memset(str2,0,20);
            memset(str3,0,20);
            memset(loc_ExpectedAngle,0,3);
            memset(loc_Time,0,3);
            memset(loc_initSS,0,3);
           //  fscanf(f,"%20s%20s%20s%20s%3s\n",  &loc_initSS,  &str1,
            //                              &str2,          &str3, &loc_Time, &loc_ExpectedAngle);
            fscanf(f,"%20s%20s%20s%20s%3s%3s\n",  loc_initSS,  str1,
                                                  str2,                  str3,
                                                  loc_Time,       loc_ExpectedAngle);
        }

       // printf("\nSpeedState:%s  PV: %s, MV: %s, P: %s, pTime: %s, ExAngle: %s\n",loc_initSS, str1, str2, str3, loc_Time, loc_ExpectedAngle);


        if(strcmp(str1,"SW_RELEASED") == 0)
        {
            *PV_SW_state = SW_RELEASED;
        }
        else if (strcmp(str1,"SW_PREPRESSED") == 0)
        {
            *PV_SW_state = SW_PREPRESSED;
        }
        else if(strcmp(str1,"SW_PRESSED") == 0)
        {
            *PV_SW_state = SW_PRESSED;
        }
        else if(strcmp(str1,"SW_PRERELEASED") == 0)
        {
            *PV_SW_state = SW_PRERELEASED;
        }
        else
        {
            printf("Incorrect test data for +V push button at test case line %d\n", TestLineCurrent);
            printf("$ The data we read is:>%s<\n", str1);
            return;
        }

        if(strcmp(str2,"SW_RELEASED") == 0)
        {
            *MV_SW_state = SW_RELEASED;
        }
        else if (strcmp(str2,"SW_PREPRESSED") == 0)
        {
            *MV_SW_state = SW_PREPRESSED;
        }
        else if(strcmp(str2,"SW_PRESSED") == 0)
        {
            *MV_SW_state = SW_PRESSED;
        }
        else if(strcmp(str2,"SW_PRERELEASED") == 0)
        {
            *MV_SW_state = SW_PRERELEASED;
        }
        else
        {
            printf("Incorrect test data for -V push button at test case line %d\n", TestLineCurrent);
            printf("$ The data we read is:>%s<\n", str2);
            return;
        }


        if(strcmp(str3,"SW_RELEASED") == 0)
        {
            *P_SW_state = SW_RELEASED;
        }
        else if (strcmp(str3,"SW_PREPRESSED") == 0)
        {
            *P_SW_state = SW_PREPRESSED;
        }
        else if(strcmp(str3,"SW_PRESSED") == 0)
        {
            *P_SW_state = SW_PRESSED;
        }
        else if(strcmp(str3,"SW_PRERELEASED") == 0)
        {
            *P_SW_state = SW_PRERELEASED;
        }
        else
        {
            printf("Incorrect test data for p push button at test case line %d\n", TestLineCurrent);
            printf("$ The data we read is:>%s<\n", str3);
            return;
        }

        if(strcmp(loc_initSS,"MINIMUM") == 0)
        {
            *intialSpeedState = MINIMUM;
        }
        else if (strcmp(loc_initSS,"MEDIUM") == 0)
        {
            *intialSpeedState = MEDIUM;
        }
        else if(strcmp(loc_initSS,"MAXIMUM") == 0)
        {
            *intialSpeedState = MAXIMUM;
        }
        else
        {
            printf("Incorrect test data for initial Speed State at test case line %d\n", TestLineCurrent);
            printf("$ The data we read is:>%s<\n", loc_initSS);
            return;
        }

        *TimePressed = (char)atoi(loc_Time);
        *ExpectedAngle = (char)atoi(loc_ExpectedAngle);

        //fflush(f);
        fclose(f);


    }
    else
    {
        printf("Failed To open the file");
    }

}


static unsigned char GetFromTheFile(unsigned char LineNum)
{

    FILE* fr = fopen("MotorAngles.txt","r+");
    char CharAngle[3];

    if(fr)
    {

        for(char i=1; i<= LineNum; i++)
        {
            memset(CharAngle, 0, 3);
            fscanf(fr,"%s\n", CharAngle);
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

static void FakeSWarrange(const SWITCH_NAME_t cpyPV,const SWITCH_NAME_t cpyMV,const SWITCH_NAME_t cpyP,const unsigned char time)
{
    FAKE_SW_setSwState(PV_switch, cpyPV);
    FAKE_SW_setSwState(MV_switch, cpyMV);
    FAKE_SW_setSwState(P_switch, cpyP);
    Fake_SW_SetTimePressed(time);
}



/************************** END of Helper Functions ***************************/

#if 0

TEST(SpeedControl, TestOperationBetweenFakeSWAndSC)
{
    //FileReadAndFakeSwitchArrange();

    /* Arrange */
    SET_STATE_TO_MAX;
    printf( "\nSpeedState : %s\n", SpeedStateString() );
    /* Act */
    SpeedControl_update();
    FAKE_MOTOR_update();
    printf( "\nSpeedState : %s\n", SpeedStateString() );

    /* Assert */
    //printf("\nExpected: %d, GetFromFile: %d\n", ExpectedAngle, GetFromTheFile(TestLineCurrent));
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}



TEST(SpeedControl, TestLine1)
{
    FileReadAndFakeSwitchArrange();

    SpeedControl_update();

    FAKE_MOTOR_update();
    printf("\nExpected: %d, GetFromFile: %d\n", ExpectedAngle, GetFromTheFile(TestLineCurrent));
    LONGS_EQUAL(ExpectedAngle, GetFromTheFile(TestLineCurrent) );
}





#if 0 /***++++++++++++++++++++++++++++++++++++++++++***/

/*----------------------- Test Cases ---------------------*/

/**
 * @brief Construct a new TEST object
 *  test that the medium speed is the default after initialization
 */
TEST(SpeedControl, mediumSpeedIsDefualtAfterInit)
{
    /*!
		  * @par Given : speed State is unknown
		  * @par When  : SpeedControl_init is called
		  * @par Then  : speed State is medium
	*/

    SpeedControl_init();

    LONGS_EQUAL( MEDIUM, CurrentSpeedState);
}



TEST(SpeedControl, SwitchsStatesReceived)
{
    /*!
		  * @par Given : speed module & Switch module initialized
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : 4 info readed through the globals are the same as the test date
	*/

    pfSWITCH_update(1); /* reading test case number 1*/

    LONGS_EQUAL(SW_RELEASED, PV_State);
    LONGS_EQUAL(SW_RELEASED, MV_State);
    LONGS_EQUAL(SW_RELEASED, P_State);
    LONGS_EQUAL( 0       , TimePressed);

}



TEST(SpeedControl, defaultAngle90degree)
{
    /*!
		  * @par Given : speed module & Switch module initialized
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : 4 info readed through the globals are the same as the test date
	*/

    SpeedControl_update();

    LONGS_EQUAL(90   ,CurrentAngle);

}


TEST(SpeedControl, fromMedToMaxWithPV)
{
    /*!
		  * @par Given : current speed Meduim
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be max with angle 10
	*/

    pfSWITCH_update(2);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MAXIMUM, CurrentSpeedState);
    LONGS_EQUAL(10   ,CurrentAngle);

}

TEST(SpeedControl, noChangeWhenPVbuttonWhileMax )
{
    /*!
		  * @par Given : current speed Maximum
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should still be max with angle 10
	*/

    pfSWITCH_update(3);
    SpeedControl_update();  /* update speed with new values from switches */


    LONGS_EQUAL( MAXIMUM, CurrentSpeedState);
    LONGS_EQUAL(10   ,CurrentAngle);

}

TEST(SpeedControl, noChangeWhenPbuttonAndInvalidTimeWhileMax)
{
    /*!
		  * @par Given : current speed Max
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should still be max with angle 10
	*/

    pfSWITCH_update(4);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MAXIMUM, CurrentSpeedState);
    LONGS_EQUAL( 10  ,CurrentAngle);

}

TEST(SpeedControl, noChangeWhenAllRelesedWhileMax)
{
    /*!
		  * @par Given : current speed Max
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should still be max with angle 10
	*/

    pfSWITCH_update(1);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MAXIMUM, CurrentSpeedState);
    LONGS_EQUAL( 10  ,CurrentAngle);

}

TEST(SpeedControl, FromMaxToMedWhenPbuttonAndTime30)
{
    /*!
		  * @par Given : current speed Max
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be Medium with angle 90
	*/

    pfSWITCH_update(5);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MEDIUM, CurrentSpeedState);
    LONGS_EQUAL(90   ,CurrentAngle);

}


TEST(SpeedControl, FromMedToMinWhenPbuttonAndTime31)
{
    /*!
		  * @par Given : current speed Medium
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be Minimum with angle 140
	*/

    pfSWITCH_update(6);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MINIMUM, CurrentSpeedState);
    LONGS_EQUAL(140  ,CurrentAngle);

}



TEST(SpeedControl, noChangeWhenPbuttonAndValidTimeWhileMin)
{
    /*!
		  * @par Given : current speed Minimum
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should still be minimum with angle 140
	*/

    pfSWITCH_update(7);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MINIMUM, CurrentSpeedState);
    LONGS_EQUAL(140  ,CurrentAngle);

}

TEST(SpeedControl, fromMinToMedWithPV)
{
    /*!
		  * @par Given : current speed Minimum
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be Medium with angle 90
	*/

    pfSWITCH_update(8);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MEDIUM, CurrentSpeedState);
    LONGS_EQUAL(90   ,CurrentAngle);

}


TEST(SpeedControl, fromMedToMaxWithPV_2)
{
    /*!
		  * @par Given : current speed Meduim
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be max with angle 10
	*/

    pfSWITCH_update(9);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MAXIMUM, CurrentSpeedState);
    LONGS_EQUAL(10   ,CurrentAngle);

}

TEST(SpeedControl, fromMaxToMedWithMV)
{
    /*!
		  * @par Given : current speed Max
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be med with angle 90
	*/

    pfSWITCH_update(10);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MEDIUM, CurrentSpeedState);
    LONGS_EQUAL(90   ,CurrentAngle);

}


TEST(SpeedControl, fromMedToMinWithMV)
{
    /*!
		  * @par Given : current speed med
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be min with angle 140
	*/

    pfSWITCH_update(11);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MINIMUM, CurrentSpeedState);
    LONGS_EQUAL(140   ,CurrentAngle);

}


TEST(SpeedControl, noChangeWhenMVbuttonWhileMin)
{
    /*!
		  * @par Given : current speed med
		  * @par When  : pfSWITCH_update is called
		  * @par Then  : speed should be min with angle 140
	*/

    pfSWITCH_update(12);
    SpeedControl_update();  /* update speed with new values from switches */

    LONGS_EQUAL( MINIMUM, CurrentSpeedState);
    LONGS_EQUAL(140   ,CurrentAngle);

}
#endif /***++++++++++++++++++++++++++++++++++++++++++***/


TEST_GROUP_RUNNER(SpeedControl)
{
    RUN_TEST_CASE(SpeedControl, IntialValues);
    RUN_TEST_CASE(SpeedControl, TestOperationBetweenFakeSWAndSC);
    RUN_TEST_CASE(SpeedControl, TestLine1);

#if 0/***++++++++++++++++++++++++++++++++++++++++++***/
    RUN_TEST_CASE(SpeedControl, mediumSpeedIsDefualtAfterInit);
    RUN_TEST_CASE(SpeedControl, SwitchsStatesReceived);
    RUN_TEST_CASE(SpeedControl, defaultAngle90degree);
    RUN_TEST_CASE(SpeedControl, fromMedToMaxWithPV);
    RUN_TEST_CASE(SpeedControl, noChangeWhenPVbuttonWhileMax);
    RUN_TEST_CASE(SpeedControl, noChangeWhenPbuttonAndInvalidTimeWhileMax);
    RUN_TEST_CASE(SpeedControl, noChangeWhenAllRelesedWhileMax);
    RUN_TEST_CASE(SpeedControl, FromMaxToMedWhenPbuttonAndTime30);
    RUN_TEST_CASE(SpeedControl, FromMedToMinWhenPbuttonAndTime31);
    RUN_TEST_CASE(SpeedControl, noChangeWhenPbuttonAndValidTimeWhileMin);
    RUN_TEST_CASE(SpeedControl, fromMinToMedWithPV);
    RUN_TEST_CASE(SpeedControl, fromMedToMaxWithPV_2);
    RUN_TEST_CASE(SpeedControl, fromMaxToMedWithMV);
    RUN_TEST_CASE(SpeedControl, fromMedToMinWithMV);
    RUN_TEST_CASE(SpeedControl, noChangeWhenMVbuttonWhileMin);
#endif /***++++++++++++++++++++++++++++++++++++++++++***/

}



/* helper function */
void FileReadAndFakeSwitchArrange(void)
{
    //SpeedState = SpeedStateCpy;

    SWITCH_TEST_getTestData(&PV_State, &MV_State, &P_State, &TimePressed, &ExpectedAngle);
    FAKE_SW_setSwState(PV_switch, PV_State);
    FAKE_SW_setSwState(MV_switch, MV_State);
    FAKE_SW_setSwState(P_switch, P_State);
    Fake_SW_SetTimePressed(TimePressed);
    printf("\nPV: %d, MV: %d, P: %d, pTime: %d, ExAngle: %d\n", PV_State, MV_State, P_State, TimePressed, ExpectedAngle);
}


/* Helper function to reuse tests */
void SWITCH_TEST_getTestData(SWITCH_STATE_t* PV_initial_SW_state, SWITCH_STATE_t* MV_initial_SW_state,
                            SWITCH_STATE_t* P_initial_SW_state, unsigned char* TimePressed,
                            unsigned char* ExpectedAngle)
{
    FILE* f = fopen("SwitchesTestData.txt","r+");
    char str1[20];
    char str2[20];
    char str3[20];
    char loc_TimePressed[3];   /* readed as string because when tring to read string and decimal using fscan and a failure happened for no known reason */
    char loc_ExpectedAngle[3];   /* readed as string because when tring to read string and decimal using fscan and a failure happened for no known reason */
    //if(test_num < 1){ printf("Please notice that test cases starts from number 1 not 0\n");}


    if(f)
    {
        /* get test line*/
        char CharTestNum[2];                /* Max Test Lines is 99 */
        fscanf(f,"%s", &CharTestNum);
        TestLineNum = (char)atoi(CharTestNum);

        printf("\nNumber of Test lines %d\n", TestLineNum);

        /*just get the specific line corresponds to the intended test case */
        for(unsigned char i = 1; i <= TestLineCurrent; i++)
        {
            memset(str1,0,20);
            memset(str2,0,20);
            memset(str3,0,20);
            memset(loc_TimePressed,0,20);
            fscanf(f,"%s\t%s\t%s\t%s\t%s\t",       str1,        str2,          str3,
                                loc_TimePressed, loc_ExpectedAngle);
        }

        printf("\nPV: %s, MV: %s, P: %s, pTime: %s, ExAngle: %s\n", str1, str2, str3, loc_TimePressed, loc_ExpectedAngle);


        if(strcmp(str1,"SW_RELEASED") == 0)
        {
            *PV_initial_SW_state = SW_RELEASED;
        }
        else if (strcmp(str1,"SW_PREPRESSED") == 0)
        {
            *PV_initial_SW_state = SW_PREPRESSED;
        }
        else if(strcmp(str1,"SW_PRESSED") == 0)
        {
            *PV_initial_SW_state = SW_PRESSED;
        }
        else if(strcmp(str1,"SW_PRERELEASED") == 0)
        {
            *PV_initial_SW_state = SW_PRERELEASED;
        }
        else
        {
            printf("Incorrect test data for +V push button at test case line %d\n", TestLineCurrent);
            printf("$ The data we read is:>%s<\n", str1);
            return;
        }

        if(strcmp(str2,"SW_RELEASED") == 0)
        {
            *MV_initial_SW_state = SW_RELEASED;
        }
        else if (strcmp(str2,"SW_PREPRESSED") == 0)
        {
            *MV_initial_SW_state = SW_PREPRESSED;
        }
        else if(strcmp(str2,"SW_PRESSED") == 0)
        {
            *MV_initial_SW_state = SW_PRESSED;
        }
        else if(strcmp(str2,"SW_PRERELEASED") == 0)
        {
            *MV_initial_SW_state = SW_PRERELEASED;
        }
        else
        {
            printf("Incorrect test data for -V push button at test case line %d\n", TestLineCurrent);
            printf("$ The data we read is:>%s<\n", str2);
            return;
        }

        if(strcmp(str3,"SW_RELEASED") == 0)
        {
            *P_initial_SW_state = SW_RELEASED;
        }
        else if (strcmp(str3,"SW_PREPRESSED") == 0)
        {
            *P_initial_SW_state = SW_PREPRESSED;
        }
        else if(strcmp(str3,"SW_PRESSED") == 0)
        {
            *P_initial_SW_state = SW_PRESSED;
        }
        else if(strcmp(str3,"SW_PRERELEASED") == 0)
        {
            *P_initial_SW_state = SW_PRERELEASED;
        }
        else
        {
            printf("Incorrect test data for p push button at test case line %d\n", TestLineCurrent);
            printf("$ The data we read is:>%s<\n", str3);
            return;
        }

        *TimePressed = (char)atoi(loc_TimePressed);
        *ExpectedAngle = (char)atoi(loc_ExpectedAngle);

        fclose(f);

    }
    else
    {
        printf("Failed To open the file");
    }

}





#if 0
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

#endif
