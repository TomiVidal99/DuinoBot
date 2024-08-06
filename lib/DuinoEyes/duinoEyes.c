/* **************************************************************************
 *
 *        Título: Duino Eyes: Sensores infrarojos del DuinoBot
 *           Año: 2024
 *
 * Desarrollador: Ignacio Brittez
 *
 *      e-mail: ignacio_brittez@outlook.com
 *
 * ************************************************************************** */

/** \file duinoEyes.c */

/* =======================================================================
 * [INCLUDES]
 * =======================================================================
 */

#include "duinoEyes.h"
#include "IRDriver.h"
#include <stdint.h>

/* =======================================================================
 * [PUBLIC INTERFACE FUNCTIONS DEFINITION]
 * =======================================================================
 */

void initDuinoEyes(void)
{
    IRDriver_Init(&DDRA, &PORTA, 2);
    IRDriver_Init(&DDRA, &PORTA, 3);
}

eyeState_t
checkDuinoEyes(void)
{
    eyeState_t eyes;
    eyes.isLeftEyeDetecting = IRDriver_IsHigh(&PORTA, 2);
    eyes.isRightEyeDetecting = IRDriver_IsHigh(&PORTA, 3);
    return eyes;
}
