#define F_CPU 16000000UL // se define que frecuencia se emplea: 16MHz

// distancias que definen el comportanmiento de rotacion y velocidad
#define MAX_DISTANCE 80
#define MID_DISTANCE 50
#define MIN_DISTANCE 10

// velocidades del motor
#define MAX_MOTOR_POWER 90
#define MID_MOTOR_POWER 30
#define MIN_MOTOR_POWER 10

// se define que se va a emplear el micro controlador ATmega328P
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h> // definiciones de los registros
#include <include/HotWheels.h>
#include <include/theDistance.h>

void found_wall_handler(); // callback de interrupción cuando se encuentra una pared

void main(void)
{
    int distance = 0;

    onYourMarks(); // se inicializan los motores

    while (1)
    {

        distance = quickDistance();

        if (distance > MAX_DISTANCE)
        {
            straightForward(MAX_MOTOR_POWER);
        }
        else if (distance < MID_DISTANCE && distance > MIN_DISTANCE)
        {
            straightForward(MID_MOTOR_POWER);
            axisLTurn(10);
            _delay_ms(30);
        }
        else
        {
            straightForward(MIN_MOTOR_POWER);
            axisRTurn(30);
            _delay_ms(50);
        }

        _delay_ms(200);
    }
}