/*//////////////////////////////////////////////////////////

Motor Driver library code.

Contact author via email: balaguer.juan@icloud.com

Don't drink and drive!

//*/
////////////////////////////////////////////////////////

#include "../lib/avr/io.h"
#ifndef F_CPU
#warning "Missing F_CPU definition"
#endif
#include "../lib/util/delay.h"

void onYourMarks(void); // setting up the timers. write to initialize, before invoking any function.

void straightForward(int); // invoke once to set forward. argument is a porcentage.

void straightBack(int); // invoke once to set backwards. argument is a porcentage.

void hardStop(void); // invoke once to break abruptly.

void softStop(void); // invoke once to stop putting active power onto the motors.	Let the robot move with its innertia

void axisRTurn(int power); // invoke once to set spinning right. argument is a porcentage.

void axisLTurn(int power); // invoke once to set spinning left. argument is a porcentage.