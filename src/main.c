#ifndef F_CPU
#define F_CPU 16000000UL // se define que frecuencia se emplea: 16MHz
#endif
#ifndef __AVR_ATmega1284P__
#define __AVR_ATmega1284P__ // se define que microcontrolador se emplea
#endif

// distancias que definen el comportanmiento de rotacion y velocidad
#define MAX_DISTANCE 80
#define MID_DISTANCE 50
#define MIN_DISTANCE 10

// velocidades del motor
#define MAX_MOTOR_POWER 100
#define MID_MOTOR_POWER 100
#define MIN_MOTOR_POWER 100

#include "../lib/avr/io.h" // definiciones de los registros
#include "../include/HotWheels.h"
#include "../include/theDistance.h"

void found_wall_handler(); // callback de interrupción cuando se encuentra una pared

int main(void)
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
            axisLTurn(MID_MOTOR_POWER - 10);
        }
        else
        {
            axisRTurn(100);
        }

        _delay_ms(5);
    }

    return 0;
}