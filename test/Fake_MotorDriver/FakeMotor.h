#ifndef FAKE_MOTOR_H_INCLUDED
#define FAKE_MOTOR_H_INCLUDED






//extern unsigned char SharedAngle;  /* Shared global variable between SpeedControl module and the MotorDriver module*/

//void MOTOR_init(unsigned char (* pGetSharedAngle)(void));
void FAKE_MOTOR_init(void);
void FAKE_MOTOR_update(void);
extern void (* MOTOR_SetSharedAngleGetter)(unsigned char (* pGetSharedAngle)(void));

unsigned char FAKE_MOTOR_GetAngle(void);


void FAKE_MOTOR_destroy(void);




#endif  /* End of file*/




