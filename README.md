# SpeedControl using TDD concept.
 This project is an implementation of a simple speed control module for vacuum cleaner developed using the Test-Driven Development (TDD) concept.


# Design notes:

## System Static Architecture

![Static Architecture](./images/staticArch.jpg)


## System Block Diagram 

![Block Diagram](./images/blockDiagram.jpg)

## State Machine for Speed Control Module

![statemachine](./images/statemachine.jpg)


## Sequence diagram for setting the callback.
in the real and fake motor, so as to let the SpeedControl module set the callback in both the Real and Fake Motor module without knowing the existance of the fake motor module:

![sequance](./images/sequance.jpg)


## APIs for modules
* Switch

    * SWITCH_init
    * SWITCH_update
    * SWITCH_GetPressedSwitch
* SpeedControl module
    * SpeedControl_init
    * SpeedControl_update
    * SpeedControl_GetSpeedState
    *  SpeedControl_GetAngle
* Motor
    * MOTOR_init
    * MOTOR_update
    * MOTOR_SetSharedAngleGetter

* OS
    * YO_eInit
    * YO_vCreateTask
    * YO_vStartScheduler

#  Timing Analysis
All analysis assume that there is no blocking points in the system.

## Approach 1

![Timing1](./images/timing_1.jpg)

Average CPU load = (3)/15= 20%

Max sleep time = 4ms

Adv: Faster

### Schedulability Check

![sched](./images/sched.jpg)


## Approach 2

![Timing1](./images/timing2.jpg)

Average CPU load = (4)/20= 20%

Max sleep time = 9 ms

Adv: longer life time (! maybe)


### Schedulability Check

![sched](./images/sched2.jpg)


# Testing Notes:

## 1-state coverage


![1statecover](./images/1statecover.jpg)


## EP for time

![EPfortime](./images/epTime.jpg)


## EP for switches state

![EPSwitches](./images/EPSwitches.jpg)


## EP composing with invalid

![EP composing with invalid](./images/EPComposingInvalid.jpg)


## BVA for time

![BVA for time](./images/BVATime.jpg)


## EP for switch names
![EP for switch names](./images/EPSwitch.jpg)

# Result:


![Result](./images/rslt.jpg)



IMPORTANT NOTE:
I`M TOTALLY NOT SURE ABOUT ANYTHING WRITTEN ABOVE,
THEY MAYBE RIGHT OR WRONG!
