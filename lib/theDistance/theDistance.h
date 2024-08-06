/*//////////////////////////////////////////////////////////

HC-SR04 sensor library code.

Contact author via email: balaguer.juan@icloud.com

Don't drink and drive!

/*/
/////////////////////////////////////////////////////////

#include "../avr/io.h"
#ifndef F_CPU
#warning "Missing F_CPU definition"
#endif
#include "../util/delay.h"

uint8_t quickDistance(void); // blocking function. Returns the distance in centimeters in an integer format.

float fineDistance(void); // blocking function. Returns the distance in centimeters in a floating point format.

// bool closerThan(int);

// bool closerThan(float);