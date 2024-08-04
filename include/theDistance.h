/*//////////////////////////////////////////////////////////

HC-SR04 sensor library code.

Contact author via email: balaguer.juan@icloud.com

Don't drink and drive!

/*/
/////////////////////////////////////////////////////////

#include "../lib/avr/io.h"
#ifndef F_CPU
#warning "Missing F_CPU definition"
#endif
#include "../lib/util/delay.h"

int quickDistance(void); // blocking function. Returns the distance in centimeters in an integer format.

float fineDistance(void); // blocking function. Returns the distance in centimeters in a floating point format.

// bool closerThan(int);

// bool closerThan(float);