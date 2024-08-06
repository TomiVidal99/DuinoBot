#include "../include/defines.h"

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
#include "../lib/sensor-infrarojo/sensorInfrarrojo.h"
// #include "../lib/DuinoEyes/duinoEyes.h"
// #include "../lib/DuinoEyes/duinoStateMachine.h"

void isEnteringLine();

// definiciones para el uso de AVR_NECdecoder
char buffer[30] = {};
volatile NEC_data_t testNEC;
volatile uint8_t currentCommand = CMD_BUTTON_0;
uint8_t distance = 0;
uint8_t isFollowingLine = 0;
// eyeState_t currEyesState = {0, 0};
// eyeState_t prevEyesState = {0, 0};

typedef struct {
    uint8_t left;
    uint8_t right;
} lineStates_t;
lineStates_t currLineState = {0, 0};
lineStates_t prevLineState = {0, 0};

int main(void)
{
    // initDuinoEyes();
    onYourMarks(); // se inicializan los motores

    IR_adc_init(); // se inicializan los sensores IR para el seguidor de linea

    NECdecoderSetUp();
    IRDriver_Init(&DDRD, &PORTD, 2);
    sei();

    while (1)
    {
        // distance = quickDistance(); // se lee la distancia al proximo obstaculo

        // prevEyesState = currEyesState;
        // currEyesState = checkDuinoEyes(); // se leen los sensores IR
        prevLineState = currLineState;
        currLineState.left = IR_adc_detect(2) < DETECTION_THRESHOLD;
        currLineState.right = IR_adc_detect(3) < DETECTION_THRESHOLD;

        // if (IR_adc_detect(3) < DETECTION_THRESHOLD || IR_adc_detect(2) < DETECTION_THRESHOLD) {
        //     straightForward(100);
        //     _delay_ms(50);
        // } else {
        //     softStop();
        // }
        // _delay_ms(20);

        isEnteringLine();                 // actualizo si se está siguiendo una línea o no

        while (currentCommand == CMD_BUTTON_1)
        {
            _delay_ms(10);
        }

        isFollowingLine = 0;
        while (isFollowingLine > 0)
        {
            PORTB |= (1<<PB7);
            if (currLineState.left == 1 && currLineState.right == 0) {
                axisRTurn(100);
                _delay_ms(100);
            } else if (currLineState.right == 1 && currLineState.right == 0) {
                axisLTurn(100);
                _delay_ms(100);
            } else {
                straightForward(100);
                _delay_ms(300);
            }
            currLineState.left = IR_adc_detect(2) < DETECTION_THRESHOLD;
            currLineState.right = IR_adc_detect(3) < DETECTION_THRESHOLD;

            if (currLineState.left == 0 && currLineState.right == 0) {
                isFollowingLine = 0;
                break;
            }
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

void isEnteringLine()
{
    if (prevLineState.left == 0 && currLineState.left != 0) {
        isFollowingLine = 1;
        return;
    }
    if (prevLineState.right == 0 && currLineState.right != 0) {
        isFollowingLine = 1;
        return;
    }
    // isFollowingLine = (prevLineState.left == 0 && currLineState.left != 0) ||
    // (prevLineState.right == 0 && currLineState.right != 0);
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
