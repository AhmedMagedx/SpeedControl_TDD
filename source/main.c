/**
 * @file main.c
 * @author Ahmed Maged (ah.maged125@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-02-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdio.h>
#include <stdlib.h>

/* Include Modules */
#include "SwitchDriver/Switch.h"
#include "MotorDriver/Motor.h"
#include "SpeedControlModule/SpeedControlModule.h"


/* Include Unity */
#include "../test/unity/unity_fixture.h"
#define MAKE_UNITY_VERBOSE	argc = 2; argv[1] = "-v"


/* Test Groups Runner */
 void RunAllTests(void)
{
    RUN_TEST_GROUP(FakeSwitch);
    RUN_TEST_GROUP(SC_GetSpeedState);
    RUN_TEST_GROUP(SC_GetSharedAngle);
    RUN_TEST_GROUP(FakeMotor);   /* This group must be after SC_GetSharedAngle group group otherwise the last will fail*/
    RUN_TEST_GROUP(SpeedControlInit);
    RUN_TEST_GROUP(SpeedControlUpdate);
    RUN_TEST_GROUP(InputTestCases);
}

/**
 * @brief the main! (nothing more to say)
 * 
 * @param argc  number of arguments to pass
 * @param argv   array of arguments to pass
 * @return  0 if ok, -1 if error hapenned
 */
int main(int argc, char * argv[])
{

    /* Manipulate argc & argv to make unity verbose*/
    /* You can use CodeBlocks Command Line inputs to do the same */
    MAKE_UNITY_VERBOSE;

    /* Call Unity Main */
    UnityMain(argc, argv, RunAllTests);


    printf("App is running..");
    SWITCH_init();
    MOTOR_init();
    SpeedControl_init();


    while(1)
    {
        /* No stubs or fakes called here */
        SWITCH_update();
        SpeedControl_update();
        MOTOR_update();
    }

    return 0;
}

