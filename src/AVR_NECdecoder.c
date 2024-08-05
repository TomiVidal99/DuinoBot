/*
 * AVR-IR-NECdecoder.c
 * Author : Jeronimo Gomez Tantarelli
 */

#include "../include/AVR_NECdecoder.h"
#include "../include/uart.h"
#include <stdio.h>

char buffer2[32];
uint8_t flag_NEC_dataIsReady;
uint8_t flag_NEC_transmissionError;
uint8_t flag_NEC_transmissionComplete;
uint8_t pulseCounter = 0;
uint8_t bitIndex = 0;
uint8_t bitsRecieved = 0;
volatile uint8_t bit;
volatile uint32_t timeEvent;

NECdata_t NECdata;
NEC_allData_t myNECdata;

void setUpINT0()
{
	/*interrupcion INT0 activada en flanco descendiente*/
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC01) | (0 << ISC00);
}

void setUpTIMER1()
{
	TCCR1B |= (1 << CS11) | (1 << CS10); /*Inicializacion + prescaler 64 ~ 4us por ciclo de reloj*/
	TCNT1 = 0X00;
	TIMSK1 |= (1 << TOIE1); /*Interrupcion por OverFlow activada*/
}

void shutDownTIMER1()
{
	TCCR1B |= (0 << CS10) | (0 << CS11) | (0 << CS12);
}

uint32_t NECdecoder()
{
	cli();			// Desactivo interrupciones
	pulseCounter++; // Permite diferenciar pulsos de inicio y de datos

	timeEvent = (uint32_t)TCNT1 * CLK_PERIOD; // TimeEvent: tiempo entre flancos de bajada

	if (pulseCounter == 1)
	{
		setUpTIMER1(); // Inicializo TIMER1 en el primer flanco descendiente

		// USART_putstring("Timer iniciado \n");
	}
	else
	{
		TCNT1 = 0; // Reinicio contador en cada flanco para calcular tiempos e/ interrupciones
	}

	if (pulseCounter == 2) // DETECCION DE FLANCOS DE INICIO DE PROTOCOLO NEC
	{
		if (timeEvent > 13000 && timeEvent < 14000)
		{
			flag_NEC_dataIsReady = TRUE;
			// USART_putstring("NECisReady :D \n");
		}
		else
		{
			flag_NEC_dataIsReady = FALSE;
			// USART_putstring("NECerror :( \n");
		}
	} // if (pulseCounter==2)

	if (pulseCounter > 2 && pulseCounter < 35 && flag_NEC_dataIsReady) // COMIENZO DE LA TRANSMISION DE 32 BITS
	{
		bit = decodeBits(timeEvent);
		bitsRecieved++;
		bitIndex = setBitIndex(bitsRecieved);
		if (bitsRecieved >= 1 && bitsRecieved <= 16) // Defino Adress
		{
			if (bitsRecieved <= 8)
				setBit(&myNECdata.preciseData.adressH, bit, bitIndex);
			else
				setBit(&myNECdata.preciseData.adressL, bit, bitIndex);
		}
		else if (16 <= bitsRecieved && bitsRecieved <= 32) // 15 < bitIndex < 32
		{
			if (bitsRecieved <= 24)
				setBit(&myNECdata.preciseData.command, bit, bitIndex);
			else
				setBit(&myNECdata.preciseData.invertedCommand, bit, bitIndex);
		}
	} // bitTransmission - if
	if (bitsRecieved == 32)
	{
		verifyNEC(myNECdata);
		check4errors(myNECdata, &flag_NEC_transmissionError);
		sendRawData(myNECdata);
		sendPreciseData(myNECdata);
	}
	sei(); // Habilito interrupciones
	return myNECdata.rawData;
}

uint8_t decodeBits(uint32_t timeEvent)
{
	if (timeEvent < 1750) // Referencia en [us] para diferenciar bits '0' y '1'
		return (uint8_t)0;
	else
		return (uint8_t)1;
}

void setBit(uint8_t *NECdata, uint8_t bit, uint8_t indice)
{
	if (bit == 1)
	{
		// set bit '1' en indice correspondiente
		*NECdata |= (1 << indice); // NECdata | (00010...0), bit '1' determinado por INDICE
	}
	else //(bit==0)
	{
		// set bit '0' en indice correspondiente
		*NECdata &= ~(1 << indice); // NECdata & (11101..1), bit '0' determinado por INDICE
	}
}

uint8_t setBitIndex(uint8_t bitsRecieved)
{
	//  debe cumplirse que 0<= bitIndex <=15

	if (bitsRecieved >= 1 && bitsRecieved <= 8)
		return (uint8_t)(8 - bitsRecieved);

	if (bitsRecieved > 8 && bitsRecieved <= 16)
		return (uint8_t)(16 - bitsRecieved);

	if (bitsRecieved > 16 && bitsRecieved <= 24)
		return (uint8_t)(24 - bitsRecieved);

	if (bitsRecieved > 24 && bitsRecieved <= 32)
		return (uint8_t)(32 - bitsRecieved);

	return 0;
}

void verifyNEC(NEC_allData_t myNECdata)
{
	if (((myNECdata.preciseData.adressL) & (myNECdata.preciseData.adressH)) == 0) // verifico si addressL = ~(addressH)
		USART_putstring("NEC protocol. \n");
	else
		USART_putstring("Extended NEC protocol. \n");
}

void sendRawData(NEC_allData_t myNECdata)
{
	uint8_t *rawBytes = (uint8_t *)&myNECdata.rawData;
	sprintf(buffer2, "%02X%02X%02X%02X", rawBytes[0], rawBytes[1], rawBytes[2], rawBytes[3]);
	USART_putstring("Raw data: ");
	USART_putstring(buffer2);
	USART_putstring("\n");
}

void sendPreciseData(NEC_allData_t myNECdata)
{
	verifyNEC(myNECdata);
	USART_putstring("Address: ");
	sprintf(buffer2, "%02X", myNECdata.preciseData.adressH);
	USART_putstring(buffer2);
	sprintf(buffer2, "%02X", myNECdata.preciseData.adressL);
	USART_putstring(buffer2);
	USART_putstring(" \n");
	USART_putstring("Command: ");
	sprintf(buffer2, "%02X", myNECdata.preciseData.command);
	USART_putstring(buffer2);
	sprintf(buffer2, "%02X", myNECdata.preciseData.invertedCommand);
	USART_putstring(buffer2);
	USART_putstring(" \n");
}

void check4errors(NEC_allData_t myNECdata, uint8_t *flag_NEC_transmissionError)
{
	if (((myNECdata.preciseData.command) & (myNECdata.preciseData.invertedCommand)) != 0)
	{
		*flag_NEC_transmissionError = TRUE;
		// USART_putstring("Error en la transmision/recepcion");
	}
}

// TODO: hacer enum de los botones
//		agregar comentarios al codigo
//		hacer funcion para resetear valores
//		hacer funcion para boton repetido
//		simplificar setBitIndex
