/*//////////////////////////////////////////////////////////

Motor Driver library code.

Contact author via email: balaguer.juan@icloud.com

Don't drink and drive!

//*/
////////////////////////////////////////////////////////

#include "../avr/io.h"
#include "../util/delay.h"
#include "HotWheels.h"

void onYourMarks(void)
{
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3); // set H-bridge pins
	DDRD |= (1 << PD4) | (1 << PD5);						   // as outputs.

	TCCR1A = 0b10100010; // clear OCPins on match set when timer is zero. Fast-PWM, 9-bit counter.
	TCCR1B = 0b00001100; // 256 clk prescaler. (~16ms per cycle).

	OCR1A = 0;
	OCR1B = 0;

	return;
}

void straightForward(int power)
{
	PORTB = (PORTB & 0xF0) | (1 << PB0) | (1 << PB3);

	OCR1A = (unsigned int)(511 * power) / 100; // receive power percentage and set wheels accordingly
	OCR1B = (unsigned int)(511 * power) / 100;

	return;
}

void straightBack(int power)
{
	PORTB = (PORTB & 0xF0) | (1 << PB1) | (1 << PB2);

	OCR1A = (unsigned int)(511 * power) / 100; // receive power percentage and set wheels accordingly
	OCR1B = (unsigned int)(511 * power) / 100;

	return;
}

void hardStop(void)
{
	PORTB = (PORTB & 0xF0) | (~(PORTB | 0xF0)); // invert the four pins that control the current flow (keep others the same)
	_delay_ms(100);
	OCR1A = 0x00;
	OCR1B = 0x00;

	return;
}

void softStop(void)
{
	OCR1A = 0x00;
	OCR1B = 0x00;

	return;
}

void axisRTurn(int power)
{
	PORTB = (PORTB & 0xF0) | (1 << PB1) | (1 << PB3);

	OCR1A = (unsigned int)(511 * power) / 100; // receive power percentage and set wheels accordingly
	OCR1B = (unsigned int)(511 * power) / 100;

	return;
}

void axisLTurn(int power)
{
	PORTB = (PORTB & 0xF0) | (1 << PB0) | (1 << PB2);

	OCR1A = (unsigned int)(511 * power) / 100; // receive power percentage and set wheels accordingly
	OCR1B = (unsigned int)(511 * power) / 100;

	return;
}
