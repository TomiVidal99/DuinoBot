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

/** \file IRDriver.c */

/* =======================================================================
 * [INCLUDES]
 * =======================================================================
 */

#include "IRDriver.h"
#include <stdint.h>

/* =======================================================================
 * [MACROS]
 * =======================================================================
 */

#define TRUE 1
#define FALSE 0

/* =======================================================================
 * [PUBLIC INTERFACE FUNCTIONS DEFINITION]
 * =======================================================================
 */

void
IRDriver_Init (volatile uint8_t *ddr, volatile uint8_t *port, uint8_t pin)
{
    *ddr &= ~MASK (pin); /* Configura el pin como entrada */
    *port |= MASK (pin); /* Inicializo el puerto en alto para evitar estado de alta impedancia  */
}

uint8_t
IRDriver_IsHigh (volatile uint8_t *port, uint8_t pin)
{
    if ((*port & MASK (pin)) == MASK (pin))
        return TRUE;
    return FALSE;
}

uint8_t
IRDriver_IsLow (volatile uint8_t *port, uint8_t pin)
{
    return !IRDriver_IsHigh (port, pin);
}
