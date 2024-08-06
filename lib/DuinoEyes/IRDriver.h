/* **************************************************************************
 *
 *        Título: Driver sensor infrarojo CNY70
 *           Año: 2024
 *
 * Desarrollador: Ignacio Brittez
 *
 *      e-mail: ignacio_brittez@outlook.com
 *
 * ************************************************************************** */

/** \file IRDRIVER.h */

#ifndef IRDRIVER_H
#define IRDRIVER_H

/* =======================================================================
 * [INCLUDES]
 * =======================================================================
 */

#include <stdint.h>

/* =======================================================================
 * [MACROS]
 * =======================================================================
 */

#define MASK(x) (1 << (x))

/* =======================================================================
 * [PUBLIC INTERFACE PROTOTYPES]
 * =======================================================================
 */


/*
 * @brief Inicialización del sensor.
 * @param ddr Puntero al ddr a utilizar.
 * @param port Puntero al port a utilizar.
 * @param pin Numero de pin a utilizar.
 */
void IRDriver_Init (volatile uint8_t *ddr, volatile uint8_t *port, uint8_t pin);

/**
 * @brief Comprueba que el pin se encuentre en alto.
 * @param port Puntero al port bajo uso.
 * @param pin Número de pin utilizado.
 * @return FALSE=0, TRUE=1.
 */
uint8_t IRDriver_IsHigh (volatile uint8_t *port, uint8_t pin);

/**
 * @brief Comprueba que el pin se encuentre en bajo.
 * @param port Puntero al port bajo uso.
 * @param pin Número de pin utilizado.
 * @return FALSE=0, TRUE=1.
 */
uint8_t IRDriver_IsLow (volatile uint8_t *port, uint8_t pin);

#endif // IRDRIVER_H
