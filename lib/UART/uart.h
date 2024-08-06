#ifndef UART_H
#define UART_H

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 115200
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL)) + 1) - 1) // modo normal


// Declaración de funciones
void USART_init(void);
unsigned char USART_receive(void);
void USART_send(unsigned char data);
void USART_putstring(char *StringPtr);

#endif // !uart
