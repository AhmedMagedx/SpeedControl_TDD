#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED


//void MOTOR_init(unsigned char (* pGetSharedAngle)(void)) = MOTOR_init_real;
void MOTOR_init(void);
void (* MOTOR_SetSharedAngleGetter)(unsigned char (* pGetSharedAngle)(void));

void MOTOR_update(void);

unsigned char (*SC_GetSharedAngleCallBack)(void);  /* Turned into pointer so we can make it point to another fake function */
void MOTOR_GetAngle(void);



#endif