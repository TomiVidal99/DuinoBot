/*//////////////////////////////////////////////////////////

HC-SR04 sensor library code.

Contact author via email: balaguer.juan@icloud.com

Don't drink and drive!

//*/
////////////////////////////////////////////////////////

#include "../lib/avr/io.h"
#include "../lib/util/delay.h"

int quickDistance(void)
{
	TCCR3A = 0x00;
	unsigned int t = 0; // clear mem space

	TCCR3B = 0x00;			// stop the count
	TCNT3 = 0x00;			// clear the count
	TIFR3 = (1 << TOV3);	// clear overflow flag
	PORTB &= (~(1 << PB5)); // clear the ICPin3

	DDRB |= (1 << PINB5); // write mode
	_delay_us(1);

	PORTB |= (1 << PINB5); // sent trigg
	_delay_us(10);
	DDRB &= 0xDF;		 // start hearing
	TCCR3B = 0b10000011; // start the count

	while (ICR3 == 0)
	{
		if ((TIFR3 & (1 << TOV3)) != 0)
		{ // if the counter overflows
			t = 0xFFFF;
			break; // stop.
		}
	}

	TCCR3B = 0x00; // stop the count

	t |= ICR3;

	return (((t) / 15) - 7);
}

float fineDistance(void)
{
	TCCR3A = 0x00;
	unsigned int t = 0; // clear mem space

	TCCR3B = 0x00;			// stop the count
	TCNT3 = 0x00;			// clear the count
	TIFR3 = (1 << TOV3);	// clear overflow flag
	PORTB &= (~(1 << PB5)); // clear the ICPin3

	DDRB |= (1 << PINB5); // write mode
	_delay_us(1);

	PORTB |= (1 << PINB5); // sent trigg
	_delay_us(10);
	DDRB &= 0xDF;		 // start hearing
	TCCR3B = 0b10000011; // start the count

	while (ICR3 == 0)
	{
		if ((TIFR3 & (1 << TOV3)) != 0)
		{ // if the counter overflows
			t = 0xFFFF;
			break; // stop.
		}
	}

	TCCR3B = 0x00; // stop the count

	t |= ICR3;

	return ((((float)t) * 0.0694) - 7.29);
}

/*
bool closerThan(int){
	//quickDistance but only checks relative distance
}

bool closerThan(float){
	//fineDistance but only checks relative distance
}//*/