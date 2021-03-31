#ifndef _DO_H_
#define    _DO_H_
#include "../common/std_types.h"

void YO_vCreateTask(    void (*  pTask)(void),  
                        const char MinimamStackSize,
                        const char priority,    
                        const char periodcity_ms,  
                        const char InitialDelay);

void YO_vStartScheduler(void);

/* prototype */
ERROR_t YO_eInit(void);


#endif