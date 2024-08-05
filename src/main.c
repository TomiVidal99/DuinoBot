#ifndef F_CPU
#define F_CPU 16000000UL // se define que frecuencia se emplea: 16MHz
#endif
#ifndef __AVR_ATmega1284P__
#define __AVR_ATmega1284P__ // se define que microcontrolador se emplea
#endif

// distancias que definen el comportanmiento de rotacion y velocidad
#define MAX_DISTANCE 60
#define MID_DISTANCE 30
#define MIN_DISTANCE 10

// velocidades del motor
#define MAX_MOTOR_POWER 100
#define MID_MOTOR_POWER 100
#define MIN_MOTOR_POWER 100

#include "../lib/avr/io.h" // definiciones de los registros
#include "../include/HotWheels.h"
#include "../include/theDistance.h"

int main(void)
{
    uint8_t distance = 0;

    onYourMarks(); // se inicializan los motores

    while (1)
    {

        distance = quickDistance(); // se lee la distancia al proximo obstaculo

        if (distance > MAX_DISTANCE)
        {
            straightForward(MAX_MOTOR_POWER);
        }
        else if (distance < MID_DISTANCE && distance > MIN_DISTANCE)
        {
            straightForward(MID_MOTOR_POWER);
        }
        else if (distance < MIN_DISTANCE)
        {

            straightBack(MID_MOTOR_POWER);
            _delay_ms(2000);

            softStop();

            axisLTurn(MID_MOTOR_POWER);
            _delay_ms(1500);
            softStop();
        }

        _delay_ms(50);
    }

    return 0;
}