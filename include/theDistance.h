/*//////////////////////////////////////////////////////////

HC-SR04 sensor library code.

Contact author via email: balaguer.juan@icloud.com

Don't drink and drive!

//*/////////////////////////////////////////////////////////

#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"


int quickDistance(void);	//blocking function. Returns the distance in centimeters in an integer format.

float fineDistance(void);	//blocking function. Returns the distance in centimeters in a floating point format.

//bool closerThan(int);

//bool closerThan(float);