/*
 * AVR-IR-NECdecoder.h
 * Author : Jeronimo Gomez Tantarelli
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef AVR_NECdecoder_H
#define AVR_NECdecoder_H

#include "../lib/avr/io.h"
#include "../lib/avr/interrupt.h"
#include "../include/IRDriver.h"
#include "../lib/util/delay.h"

#define PRESCALER 64
#define CLK_PERIOD ((1000000) * PRESCALER / (uint32_t)F_CPU) // periodo medido en us

#define TRUE 1
#define FALSE 0

typedef struct
{
	uint8_t adressH;
	uint8_t adressL;
	uint8_t command;
	uint8_t invertedCommand;
} NECdata_t;

typedef union
{
	NECdata_t preciseData;
	uint32_t rawData;
} NEC_allData_t;

void setUpINT0();
void setUpTIMER1();
void shutDownTIMER1();
uint32_t NECdecoder();
uint8_t decodeBits();
void setBit(uint8_t *NECdata, uint8_t bit, uint8_t indice);
uint8_t setBitIndex(uint8_t bitsRecieved);
void verifyNEC(NEC_allData_t myNECdata);
void sendRawData(NEC_allData_t myNECdata);
void sendPreciseData(NEC_allData_t myNECdata);
void check4errors(NEC_allData_t myNECdata, uint8_t *flag_NEC_transmissionError);
#endif // AVR_NECdecoder_H