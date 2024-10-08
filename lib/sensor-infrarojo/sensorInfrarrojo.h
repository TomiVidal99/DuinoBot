/**
 * @file sensor_infrarrojo.c
 * @brief Driver para interactuar con Sensores Infrarrojos
 * @author Bruno Benítez
 * @date 2024-7-24 año/mes/día
 * @version v0.2024724
 */
/*Driver: sensor infrarrojo*/

//#ifndef SENSOR_INFRARROJO_H
//#define SENSOR_INFRARROJO_H

#include "../avr/io.h"
#define MASK(x) (1 << x)
//#include <avr/sfr_defs.h>

/**
 * @brief Configuramos el puerto utilizado por el sensor infrarrojo
 * 
 * @param ddr Data Direction Register a configurar
 * @param ddr_bit Bit del ddr a configurar
 * @param port Puerto a configurar
 * @param port_bit Bit del port a
 */
void IR_init(volatile uint8_t *ddr, uint8_t ddr_bit, volatile uint8_t *port, uint8_t port_bit);

/**
 * @brief Lee el estado del sensor
 * 
 * @param port Es el puerto de entrada donde está el sensor
 * @param port_bit Es el bit a leer
 * 
 * @return uint8_t 1 si detecta luz, 0 en caso contrario
 */
uint8_t IR_fast_detect(volatile uint8_t *pin, uint16_t pin_bit);

/**
 * @brief Inicialización del modo ADC
 * 
 * @note Se justifica a la izquierda. Por esto se desprecia los 2(dos) bits menos significativos de ADC
 * @note Por esto se desprecia los 2(dos) bits menos significativos de ADC.
 */
void IR_adc_init();

/**
 * @brief Lectura de la intensidad de luz recibida por el sensor, utilizando el modo ADC: conversión simple. 
 * 
 * @param adc_x Canal utilizado por el sensor, del ADC0 al ADC7.
 * @return uint8_t ADCH contiene los 8bits más significativos de la conversión (porque justificamos a la izquierda)
 */
uint16_t IR_adc_detect(uint8_t canal);

uint8_t IR_adc_detecth(uint8_t canal);

/**
 * @brief Lectura del estado del sensor utilizando el modo ADC: conversión simple. 
 * El valor de retorno depende de porcentaje_umbral
 * 
 * @param adc_x Canal utilizado por el sensor, del ADC0 al ADC7.
 * @param porcentaje_umbral porcentaje de VCC para comparar con el resultado de la conversión
 * @return uint8_t 1 si la conversión es mayor o igual al umbral elegido, 0 en caso contrario
 */
uint8_t IR_slow_detect(uint8_t canal, uint8_t porcentaje_umbral);

uint16_t calibracion_sensor(uint8_t canal, uint16_t cantidad_iteraciones);

//#endif