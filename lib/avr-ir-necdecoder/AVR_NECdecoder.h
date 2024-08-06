/*

 * AVR-IR-NECdecoder.h
 
 * Autor : Jeronimo Gomez Tantarelli
 
 * Contacto: j.gomeztantarelli@gmail.com
 
 */ 


/***************
	[INCLUDE]
***************/
#ifndef AVR_NECdecoder_H
#define AVR_NECdecoder_H
#include "../IRDriver/IRDriver.h"
#include "../UART/uart.h"
#include "myRemotes.h"
#include <stdio.h>
#include "../avr/io.h"
#include "../avr/interrupt.h"
#include "../util/delay.h"

/****************
	[DEFINE]
****************/
#define F_CPU 16000000UL
#define PRESCALER 64					/*MODIFICAR SI SE MODIFICA setUpTIMER1*/
#define CLK_PERIOD ((1000000)*PRESCALER/ (uint32_t) F_CPU ) /* [us] */
#define TRUE 1
#define FALSE 0

typedef struct{
	uint8_t adressH;
	uint8_t adressL;
	uint8_t command;
	uint8_t invertedCommand;
}NECdataStruct_t;

typedef union{
	NECdataStruct_t	preciseData;
	uint32_t	rawData;
}NEC_data_t;



/*****************************
[FUNCIONES DE SETUP/SHUTDOWN]
******************************/
void NECdecoderSetUp();
void setUpTIMER1();
void shutDownTIMER1();


/****************************
[FUNCIONES QUE DECODIFICAN]
*****************************/
void NECdecoder(NEC_data_t *myNECdata);
uint32_t getNECrawData();
uint16_t getNECaddress();
uint8_t getNECcommand();

/********************************************
[FUNCIONES DE IMPRESION DE DATOS Y DEBUGGEO]
*********************************************/
void print_NEC_or_eNEC(NEC_data_t myNECdata);
void print_rawData(NEC_data_t myNECdata);
void print_rawData_pretty(NEC_data_t myNECdata);
void print_preciseData(NEC_data_t myNECdata);
void print_preciseData_pretty(NEC_data_t myNECdata);
void print_repeatCodeWarning();
void check4errors(NEC_data_t myNECdata, uint8_t *flag_NEC_transmissionError);


///////////////////////////////////////////////////////////////////////////////////////
//				[FUNCIONES DE INTERFAZ PRIVADA]										//
//////////////////////////////////////////////////////////////////////////////////////

/************************************
[FUNCIONES DE DEFINICION  DE DATOS]
*************************************/
static void checkIfNEC(uint32_t timeEvent, uint8_t *flag_NEC_dataIsReady);
static void setBit(uint8_t *myRegister, uint8_t bit, uint8_t indice);
static uint8_t setBitIndex(uint8_t bitsRecieved);
static uint8_t decodeBits(uint32_t timeEvent);
static void define_myNECdata(NEC_data_t *myNECdata, uint8_t bitsRecieved, uint8_t bit);


/***************************************
[FUNCIONES DE REINICIO DE DECODIFICADOR]
****************************************/
static void restartCounters(uint8_t *pulseCounter, uint8_t *bitsRecieved);
static void restartFlags(uint8_t *flag_NEC_dataIsReady, uint8_t *flag_NEC_transmissionError);
void go2sleepDecoder();




#endif // AVR_NECdecoder_H