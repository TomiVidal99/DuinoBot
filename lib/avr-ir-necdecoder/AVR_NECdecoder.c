/*

 * AVR-IR-NECdecoder.c
 
 * Autor : Jeronimo Gomez Tantarelli
 
 * Contacto: j.gomeztantarelli@gmail.com
 
 */ 

#include "AVR_NECdecoder.h"
#include "../UART/uart.h"

#include <stdio.h>

char buffer[33];
uint8_t flag_NEC_dataIsReady=0;
uint8_t flag_NEC_transmissionError=0;
uint8_t pulseCounter=0;
uint8_t bitsRecieved=0;
uint8_t bit;
uint32_t timeEvent;

NECdataStruct_t NECdata;
NEC_data_t myNECdata;

/*NECdecoderSetUp:
	Inicializacion de INT0 e interrupcion por flancos decrecientes
*/

void NECdecoderSetUp (){	
EIMSK	|= (1<< INT0);
EICRA	|= (1<< ISC01)| (0 << ISC00);
}

/*setUpTIMER1:
	Inicializacion de TCNT1, prescaler (64) y interrupcion por overFlow
*/

void setUpTIMER1(){	
TCCR1B	|= (1<< CS11)|(1<<CS10);	/*MODIFICAR PRESCALER EN AVR_NECdecoder.h SI SE MODIFICA*/
TIMSK1	|= (1<< TOIE1);				
TCNT1	= 0X00;
}

/*shutDownTIMER1:
	TCNT1 desactivado
*/
void shutDownTIMER1(){
TCCR1B	|= (0<<CS10)|(0<<CS11)|(0<<CS12);
}

/*NECdecoder:
	Devuelve por parametro una variable de tipo NEC_data_t de definicion propia.
	Puede accederse como myNECdata.rawData (32 bits)
	o como myNECdata.preciseData.tipoDeDato (8 bits)
*/
void NECdecoder(NEC_data_t *myNECdata)
{
	pulseCounter++;										//Permite diferenciar pulsos de inicio y de datos
	timeEvent	= TCNT1*CLK_PERIOD ;					//TimeEvent: tiempo entre flancos de bajada
	
	
	if(pulseCounter==1)
		setUpTIMER1();		
	else
		TCNT1	= 0;		
	
	if (pulseCounter==2)	
		checkIfNEC(timeEvent, &flag_NEC_dataIsReady);

	if (pulseCounter>2)
	{
		if (flag_NEC_dataIsReady)
		{			
			if (pulseCounter>2 &&  pulseCounter<35)		//COMIENZO DE LA TRANSMISION DE 32 BITS
			{
				bit = decodeBits(timeEvent);
				bitsRecieved++;
				define_myNECdata(myNECdata, bitsRecieved, bit);
			} 
			if (bitsRecieved==32 && pulseCounter==35)
			{
				//check4errors(*myNECdata, &flag_NEC_transmissionError);
				//print_rawData_pretty(*myNECdata);
				//print_preciseData_pretty(*myNECdata);
			}
			
			if (pulseCounter> 36 && timeEvent>11000 && timeEvent<12000)
			{
				//print_repeatCodeWarning();
			}
			
		}		/*end if(flag_NEC_dataIsReady)*/
		else	/*if(flag_NEC_dataIsReady==FALSE)*/
		{
			go2sleepDecoder();
		}
	}
}
/*getNECaddress:
	Devuelve address (16 bits) decodificado
*/
uint16_t getNECaddress(){
	NECdecoder(&myNECdata);
	return (myNECdata.preciseData.adressH << 8 )|(myNECdata.preciseData.adressL);
}
/*getNECcommand:
	Devuelve comando (8 bits) decodificado
*/
uint8_t getNECcommand(){
	NECdecoder(&myNECdata);
	return myNECdata.preciseData.command;
}
/*getNECrawData:
	Devuelve 32 bits decodificados
*/
uint32_t getNECrawData(){
	NECdecoder(&myNECdata);
	return myNECdata.rawData;
}


/*CheckIfNEC: 
*	Activa la flag de dataIsReady
	si el timpo entre flancos coincide con el del protocolo 
	
*	Desactiva la flag en caso contrario	

*	Argumento timeEvent no es requerido, se mantiene por claridad*/

void checkIfNEC(uint32_t timeEvent, uint8_t *flag_NEC_dataIsReady){

	if (timeEvent>13000 && timeEvent<14000)
		*flag_NEC_dataIsReady	= TRUE;
	else
		*flag_NEC_dataIsReady	= FALSE;
}


/*decodeBits:
*	Decodifica valor de un bit segun protocolo:
*	'0': 1125 us de tiempo entre flancos descendientes
*	'1': 2250 us "										"
*	timeEvent [us]										

*	Argumento no es requerido pero se mantiene por claridad*/

uint8_t decodeBits(uint32_t timeEvent){					
return (timeEvent < 1750) ? (uint8_t)0 : (uint8_t)1;
}

/*setBit:
*	Ubica el bit decodificado en su respectivo indice para variables de 8 bits

*	Argumentos 'bit' e 'indice' no es requeridos, se mantienen por claridad*/

void setBit(uint8_t *myRegister, uint8_t bit, uint8_t indice){
	if (bit)
		*myRegister|= (1 << indice);			
	if (!bit)	
		*myRegister&= ~(1<<indice);				
}

/*setBitIndex:
*	Define argumento 'indice' para funcion setBit(x,y, indice)
	setBitIndex(1)	= 7;
	setBitIndex(2)	= 6;
	...
	setBitIndex(8)	= 0;
	setBitIndex(9)	= 7;
	...
	setBitIndex(16)	= 0;
	
*	1<= bitsReceived<=32 	
	
*	Argumento 'bitsReceived' no es requerido, se mantiene por claridad*/	

uint8_t setBitIndex(uint8_t bitsRecieved){
	if (bitsRecieved>=1 && bitsRecieved<=32)
	return (8 - (bitsRecieved % 8)) % 8;
}

/*define_myNECdata:
	Almacena el mensaje IR recbido en myNECdata.rawData
*/

void define_myNECdata(NEC_data_t *myNECdata, uint8_t bitsRecieved, uint8_t bit){
	uint8_t bitIndex = setBitIndex(bitsRecieved);
	if (bitsRecieved<=8)
		setBit(&myNECdata->preciseData.adressH, bit, bitIndex);
			
	if (bitsRecieved>8 && bitsRecieved<=16)
		setBit(&myNECdata->preciseData.adressL, bit, bitIndex);
			
	if (bitsRecieved>16 && bitsRecieved<=24)
		setBit(&myNECdata->preciseData.command, bit, bitIndex);
		
	if (bitsRecieved>24 && bitsRecieved<=32)
		setBit(&myNECdata->preciseData.invertedCommand, bit, bitIndex);
}

/*print_NEC_or_eNEC:
	Imprime por UART tipo de protocolo (NEC o Extended NEC)
*/

void print_NEC_or_eNEC(NEC_data_t myNECdata){
	
	if ( ((myNECdata.preciseData.adressL) & (myNECdata.preciseData.adressH)) == 0)	//verifico si addressL = ~(addressH)
		USART_putstring("NEC protocol. \n");
	else
		USART_putstring("Extended NEC protocol. \n");
}

/*print_rawData:
	Imprime por UART 32 bits de datos en hexadecimal
*/
void print_rawData(NEC_data_t myNECdata){
	
	uint8_t *rawBytes = (uint8_t*)&myNECdata.rawData;
	sprintf(buffer, "%02X%02X%02X%02X", rawBytes[0], rawBytes[1], rawBytes[2], rawBytes[3]);
	USART_putstring(buffer);
	USART_putstring("\n");
}

/*print_rawData_pretty:
	Imprime por UART 32 bits de datos en hexadecimal
+	Imprime tipo de protocolo NEC
*/

void print_rawData_pretty(NEC_data_t myNECdata){
	
	print_NEC_or_eNEC(myNECdata);
	USART_putstring("Raw data: ");
	print_rawData(myNECdata);
}

/*print_preciseData:
	Imprime por separado direccion y comando
*/

void print_preciseData(NEC_data_t myNECdata){
		USART_putstring("addrs: ");
		sprintf(buffer, "%02X", myNECdata.preciseData.adressH);
		USART_putstring(buffer);
		sprintf(buffer, "%02X", myNECdata.preciseData.adressL);
		USART_putstring(buffer);
		USART_putstring(" \n");
		USART_putstring("cmd: ");
		sprintf(buffer, "%02X", myNECdata.preciseData.command);
		USART_putstring(buffer);
		sprintf(buffer, "%02X", myNECdata.preciseData.invertedCommand);
		USART_putstring(buffer);
		USART_putstring(" \n");

}

/*print_preciseData_pretty:
	Imprime por separado direccion y comando
+	Imprime tipo de protocolo NEC
*/

void print_preciseData_pretty(NEC_data_t myNECdata){
	print_NEC_or_eNEC(myNECdata);
	print_preciseData(myNECdata);
}


/*print_repeatCodeWarning:
	Imprime por UART codigo de advertencia de pulsacion de tecla mantenida
*/
void print_repeatCodeWarning(){
	USART_putstring("Codigo repetido. Pulsacion mantenida \n");
}


/*check4errors:
	Activa flag de error de transmision
+	Imprime por UART si detecta que no se cumple protocolo en bits de comando
*/

void check4errors(NEC_data_t myNECdata, uint8_t *flag_NEC_transmissionError){
		if ( ((myNECdata.preciseData.command) & (myNECdata.preciseData.invertedCommand)) != 0)
		{
			*flag_NEC_transmissionError=TRUE;
			USART_putstring("Error en la transmision/recepcion");
		}
}

/*restartFlags:
	flag = FALSE (para todas las flags)
*/
void restartFlags(uint8_t *flag_NEC_dataIsReady, uint8_t *flag_NEC_transmissionError){
	*flag_NEC_dataIsReady = FALSE;
	*flag_NEC_transmissionError = FALSE;
}

/*restartCounters:
	contadores = 0 (para variables globales que cuentan)
*/

void restartCounters(uint8_t *pulseCounter, uint8_t *bitsRecieved){
	*pulseCounter=0;
	*bitsRecieved=0;
}

/*go2sleepDecoder:
	Reinicio flags, contadores 
	(usado en fin de la decodificacion)
*/

void go2sleepDecoder(){
	restartFlags(&flag_NEC_dataIsReady, &flag_NEC_transmissionError);
	restartCounters(&pulseCounter, &bitsRecieved);
}
