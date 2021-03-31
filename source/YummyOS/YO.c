#include "YO.h"



ERROR_t YO_eInit(void)
{

    long loc_TimeOut = 10000;   /* A time out counter to terminate the proccess if an error occurred*/

    SWITCH_init();

    while(  SpeedControl_init() != E_OK || 
            MOTOR_init() != E_OK        || 
            SWITCH_init() != E_OK ) /* block tell finish or timeout */
    {   
        /* calling SpeedControl_init() first will return PENDING waiting for MOTOR_init() */
        /* MOTOR_init() in addition to the  normal initialization,
           it sets a pointer to function used in 
           SpeedControl_init() */

           SpeedControl_init();     /* We supposed to call MOTOR_init() first but as a resualt to the implemented mechanism here, it is safe! otherwise the SpeedControl_init will try to access a NULL pointer*/
           MOTOR_init();
        if( SpeedControl_init() == E_NOK)   /* if initializing SpeedControl failed */
        {
            printf("\nError initializing the SpeedControl module\n");
            return E_NOK;
        }
        else if (MOTOR_init() == E_NOK)     /* if initializing Motor failed */
        {
            printf("\nError initializing the Motor module\n");
            return E_NOK;
        }
        else    /*  if noting failed (PENDING or E_OK) */
        {
            loc_TimeOut--;
            if(!loc_TimeOut)    /* if the counter reaches Zero. hence, timeout occurred */
            {
                return E_NOK;
            }
        }

    }   /*  end While */    /* exit if All is E_OK */


    return E_OK;

}





/**
 * @brief 
 * 
 * @param pTask     a pointer to the task 
 * @param MinimamStackSize      the minimum stack size based on the task implementation
 * @param priority      the priority of the task (the lower the number the higher the priority )
 * @param periodcity_ms     the period to recall the task.
 * @param InitialDelay  to determine the FIRST call time.
 */
void YO_vCreateTask(    void (*  pTask)(void),
                        const char MinimamStackSize, 
                        const char priority,
                        const char periodcity_ms,
                        const char InitialDelay)
{
            /*********************************************
            *       HI YummyOs is currently died ^_*     *
            *********************************************/
}

/**
 * @brief periodiclly call the task according to its timing 
 *        specifications and if no task is supposed to run,
 *        it run the LOW_POWER task
 * 
 */
void YO_vStartScheduler(void)
{
            /*********************************************
            *       HI YummyOs is currently died ^_*     *
            *********************************************/
            
            /*      it supposed to periodiclly call the task according 
                    to its timing specifications and if no task is supposed
                    to run,it run the LOW_POWER task
            */
}

