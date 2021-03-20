#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED
#include "../common/std_types.h"

#ifndef NULL
    #define NULL 0
#endif /* NULL */


//void MOTOR_init(unsigned char (* pGetSharedAngle)(void)) = MOTOR_init_real;
ERROR_t MOTOR_init(void);
//void (* MOTOR_SetSharedAngleGetter)(unsigned char (* pGetSharedAngle)(void));

void MOTOR_update(void);




#endif
