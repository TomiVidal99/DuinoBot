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

// valor que dispara cuando se detecta una línea
#define DETECTION_THRESHOLD 150

#include <stdio.h>
#include <stdint.h>

// definiciones de los registros
#include "../lib/avr/io.h"

#include "../lib/HotWheels/HotWheels.h"

#include "../lib/theDistance/theDistance.h"

// dependencias para AVR_NECdecoder
#include "../lib/avr-ir-necdecoder/AVR_NECdecoder.h"
#include "../lib/util/delay.h"
#include "../lib/avr/interrupt.h"

// dependencias para el seguidor de linea DuinoEye
#include "../lib/DuinoEyes/duinoEyes.h"

void isEnteringLine();

// definiciones para el uso de AVR_NECdecoder
char buffer[30] = {};
volatile NEC_data_t testNEC;
volatile uint8_t currentCommand = CMD_BUTTON_0;
uint8_t distance = 0;
uint8_t isFollowingLine = 0;
eyeState_t currEyesState = {0, 0};
eyeState_t prevEyesState = {0, 0};

int main(void)
{
    initDuinoEyes();
    onYourMarks(); // se inicializan los motores

    NECdecoderSetUp();
    IRDriver_Init(&DDRD, &PORTD, 2);
    sei();

    while (1)
    {

        distance = quickDistance(); // se lee la distancia al proximo obstaculo
        prevEyesState = currEyesState;
        currEyesState = checkDuinoEyes(); // se leen los sensores IR
        isEnteringLine();                 // actualizo si se está siguiendo una línea o no

        while (currentCommand == CMD_BUTTON_1)
        {
            _delay_ms(10);
        }

        while (isFollowingLine > 0)
        {
            softStop();
        }

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

/* Se retorna 0 si está 'entrando' a una línea. 0 caso contrario */
void isEnteringLine()
{
    if (currEyesState.isLeftEyeDetecting > DETECTION_THRESHOLD &&
        prevEyesState.isLeftEyeDetecting < DETECTION_THRESHOLD)
    {
        isFollowingLine = 0;
        return;
    }
    if (currEyesState.isRightEyeDetecting > DETECTION_THRESHOLD &&
        prevEyesState.isRightEyeDetecting < DETECTION_THRESHOLD)
    {
        isFollowingLine = 0;
        return;
    }
}

ISR(INT0_vect)
{
    NECdecoder(&testNEC);
}

ISR(TIMER1_OVF_vect)
{
    currentCommand = testNEC.preciseData.command;
    go2sleepDecoder();
}