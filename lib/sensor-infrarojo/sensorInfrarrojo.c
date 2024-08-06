/**
 * @file sensor_infrarrojo.c
 * @brief Driver para interactuar con Sensores Infrarrojos
 * @author Bruno Benítez
 * @date 2024-7-24 año/mes/día
 * @version v0.2024724
 */
/*Driver: sensor infrarrojo*/

#include "../avr/io.h"
#include "sensorInfrarrojo.h"
#include "../avr/sfr_defs.h"

#define LEER_PIN(x,b)   ((x) & 1<<(b))
#define MASK(x) (1 << x)


/**
 * @brief Configuramos el puerto utilizado por el sensor infrarrojo
 * 
 * @param ddr Data Direction Register a configurar
 * @param ddr_bit Bit del ddr a configurar
 * @param port Puerto a configurar
 * @param port_bit Bit del port a
 */

void IR_init(volatile uint8_t *ddr, uint8_t ddr_bit, volatile uint8_t *port, uint8_t port_bit) /*_MMIO_BYTE*/
{
    /*Configuración del puerto como entrada*/
    *ddr &= ~MASK(ddr_bit);
    *port &= ~MASK(port_bit); 
}

/**
 * @brief Lee el estado del sensor
 * 
 * @param port Es el puerto de entrada donde está el sensor
 * @param port_bit Es el bit a leer
 * 
 * @return uint8_t 1 si detecta luz, 0 en caso contrario
 */
uint8_t IR_fast_detect(volatile uint8_t *pin, uint16_t pin_bit)
{
    //if(bit_is_set(*pin, MASK(pin_bit))!=0)/*Verificamos el estado del sensor*/
      //  return 1;/*Detección de luz*/
    
    //return 0;/*No se detecta luz*/

    if(LEER_PIN(*pin,pin_bit) == (1<<pin_bit))
		return 1;
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
    //Y si le sumamos una magia +?  


/**
 * @brief Inicialización del modo ADC
 * 
 * @note Se justifica a la izquierda.
 * @note Por esto se desprecia los 2(dos) bits menos significativos de ADC.
 */
void IR_adc_init()
{
	ADCSRA = MASK(ADEN) /* Habilitamos el modo ADC*/
			 | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); /* Divisor de frecuencia de 128*/
	ADMUX = (0<<REFS1) | (1<<REFS0) /*Tensión de referencia: AVCC*/
			 | (0<<ADLAR) /*Justificado a la derecha*/
			 | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);
																								 
	/* Perform Dummy Conversion to complete ADC init */
	ADCSRA |= (1<<ADSC);

	/* wait for conversion to complete */
	while ((ADCSRA & (1<<ADSC)) != 0);                      
}

/**
 * @brief Lectura de la intensidad de luz recibida por el sensor, utilizando el modo ADC: conversión simple. 
 * 
 * @param uint8t canal Canal utilizado por el sensor, del ADC0 al ADC7.
 * @return uint8_t ADCH contiene los 8bits más significativos de la conversión (porque justificamos a la izquierda)
 */
uint16_t IR_adc_detect(uint8_t canal)
{
	canal &= ((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
	ADMUX = (ADMUX & 0xF0) | canal;
    ADCSRA |= (1<<ADSC);/*Seteamos ADSC (ADC Start Conversion) para iniciar la conversión*/

    while ((ADCSRA & (1<<ADSC)) != 0);/*Esperamos que la conversion este lista*/
                                        /*Normalmente la conversión toma 13 ciclos*/

    return ADC;
}

uint16_t calibracion_sensor(uint8_t canal, uint16_t cantidad_iteraciones)/* Calculo el valor umbral para el color negro*/
{
	uint16_t i = 0;
	uint16_t lectura_adc[cantidad_iteraciones];
	uint16_t lectura_max = 0;
	uint16_t lectura_min = 0;
	
	for (i=0; i < cantidad_iteraciones; i++)
	{
		lectura_adc[i] = IR_adc_detect(canal);
	}
	for (i = 0; i < cantidad_iteraciones; i++)
	{
		if(lectura_adc[i] > lectura_max){
			lectura_max = lectura_adc[i];
		}
		else if(lectura_adc[i] < lectura_min){
			lectura_min = lectura_adc[i];
		}
	}
	uint16_t umbral = (lectura_max + lectura_min)/2;
	return umbral;
}


//IR_analog_comparator_intit
/*Si quisieramos utilizar el comparador analógico del ATMEGA, necesitamos poner la tensión umbral en la pata negativa*/
/*Pero el ATMEGA no tiene DAC*/
/*Si se quisiera, se podría utilizar el PWM y filtrarlo para conseguir la tensión umbral
 ---> requiere añadir hardware, la librería perdería su gracia*/