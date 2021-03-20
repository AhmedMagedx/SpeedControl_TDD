
#include "../Fake_SwitchDriver/FakeSwitch.h"
#include "../unity/unity_fixture.h"
#include <string.h>

TEST_GROUP(FakeSwitch);

TEST_SETUP(FakeSwitch)
{
    FAKE_SW_init();
    //SpeedControl_init();
}

TEST_TEAR_DOWN(FakeSwitch)
{
    FAKE_SW_destroy();
}


/*----------------------- Test Cases ---------------------*/

#if 0
/* Helper function to reuse tests */
void SWITCH_TEST_getTestData(SWITCH_STATE_t* PV_initial_SW_state, SWITCH_STATE_t* MV_initial_SW_state,
                                SWITCH_STATE_t* P_initial_SW_state, unsigned char* TimePressed,
                                unsigned char test_num);
#endif

#if 0
/* reading these values from the FakeSwitch.c */
extern SWITCH_STATE_t PV_State;
extern SWITCH_STATE_t MV_State;
extern SWITCH_STATE_t P_State;
extern unsigned char TimePressed;
#endif



/**
 * @brief Construct a new TEST object
 *      Test that the switch is in released state after initialization
 */
TEST(FakeSwitch, switchesAreReleasedAfterInit)
{
    /*!
		  * @par Given : Switch State is unknown
		  * @par When  : Init is called
		  * @par Then  : Switch state is SW_RELEASED
	*/

    SWITCH_NAME_t buttonPressedName ;
    buttonPressedName = SWITCH_GetPressedSwitch();
    /* Note that init is called in TEST_SETUP() */
    LONGS_EQUAL(NONE, buttonPressedName);   /* PVswitch */

}


/**
 * @brief Construct a new TEST object
 *      Test that the set switch state is working
 */
TEST(FakeSwitch, switchSetState)
{
    /*!
		  * @par Given : Switch State is relesed
		  * @par When  : setState is called
		  * @par Then  : Switch state is SW_PREPRESSED
	*/
    FAKE_SW_Arrange(SW_PREPRESSED, SW_PRESSED, SW_RELEASED, 0);

    SWITCH_NAME_t buttonPressedName ;
    buttonPressedName = SWITCH_GetPressedSwitch();

    LONGS_EQUAL(MV_switch, buttonPressedName);

}


/**
 * @brief Construct a new TEST object
 *      test reading th first test case from the first line of the file
 */
TEST(FakeSwitch, PriorityCheckPVAndMV)
{
    /*!
        * @par Given : Text File with a valid test data is exist in the same directory
        * @par When  : SWITCH_TEST_getTestData is called
        * @par Then  : local variables must equal the first line date
	*/

    /* Arrange */
    FAKE_SW_Arrange(SW_PRESSED, SW_PRESSED, SW_RELEASED, 0);

    /* Act */
    SWITCH_NAME_t buttonPressedName ;
    buttonPressedName = SWITCH_GetPressedSwitch();

    /* Assert */
    LONGS_EQUAL(MV_switch, buttonPressedName);
}

/**
 * @brief Construct a new TEST object
 *      test reading th first test case from the first line of the file
 */
TEST(FakeSwitch, PriorityCheckMVAndP)
{
    /*!
        * @par Given : Text File with a valid test data is exist in the same directory
        * @par When  : SWITCH_TEST_getTestData is called
        * @par Then  : local variables must equal the first line date
	*/

    /* Arrange */
    FAKE_SW_Arrange(SW_RELEASED, SW_PRESSED, SW_PRESSED, 30);

    /* Act */
    SWITCH_NAME_t buttonPressedName ;
    buttonPressedName = SWITCH_GetPressedSwitch();

    /* Assert */
    LONGS_EQUAL(P_switch, buttonPressedName);
}

/**
 * @brief Construct a new TEST object
 *      test reading th first test case from the first line of the file
 */
TEST(FakeSwitch, PriorityCheckPVAndP)
{
    /*!
        * @par Given : Text File with a valid test data is exist in the same directory
        * @par When  : SWITCH_TEST_getTestData is called
        * @par Then  : local variables must equal the first line date
	*/

    /* Arrange */
    FAKE_SW_Arrange(SW_PRESSED, SW_RELEASED, SW_PRESSED, 30);

    /* Act */
    SWITCH_NAME_t buttonPressedName ;
    buttonPressedName = SWITCH_GetPressedSwitch();

    /* Assert */
    LONGS_EQUAL(P_switch, buttonPressedName);
}

/**
 * @brief Construct a new TEST object
 *      test reading th first test case from the first line of the file
 */
TEST(FakeSwitch, PriorityCheckAllPressed)
{
    /*!
        * @par Given : Text File with a valid test data is exist in the same directory
        * @par When  : SWITCH_TEST_getTestData is called
        * @par Then  : local variables must equal the first line date
	*/

    /* Arrange */
    FAKE_SW_Arrange(SW_PRESSED, SW_PRESSED, SW_PRESSED, 30);

    /* Act */
    SWITCH_NAME_t buttonPressedName ;
    buttonPressedName = SWITCH_GetPressedSwitch();

    /* Assert */
    LONGS_EQUAL(P_switch, buttonPressedName);
}





TEST_GROUP_RUNNER(FakeSwitch)
{
    RUN_TEST_CASE(FakeSwitch, switchesAreReleasedAfterInit);
    RUN_TEST_CASE(FakeSwitch, switchSetState);
    RUN_TEST_CASE(FakeSwitch, PriorityCheckPVAndMV);
    RUN_TEST_CASE(FakeSwitch, PriorityCheckMVAndP);
    RUN_TEST_CASE(FakeSwitch, PriorityCheckPVAndP);
    RUN_TEST_CASE(FakeSwitch, PriorityCheckAllPressed);

}



void FAKE_SW_Arrange(SWITCH_STATE_t pvstate, SWITCH_STATE_t mvstate, SWITCH_STATE_t pstate, unsigned char time)
{
    FAKE_SW_setSwState(PV_switch, pvstate);
    FAKE_SW_setSwState(MV_switch, mvstate);
    FAKE_SW_setSwState(P_switch, pstate);
    Fake_SW_SetTimePressed(time);
}
