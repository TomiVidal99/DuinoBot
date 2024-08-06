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

/** \file duinoEyes.h */

#ifndef DUINOEYES_H
#define DUINOEYES_H

/* =======================================================================
 * [INCLUDES]
 * =======================================================================
 */

#include <stdint.h>

#ifdef TEST
#include "stub_iom1284p.h"
#else
#include "../avr/io.h"
#endif // TEST

/* =======================================================================
 * [MACROS]
 * =======================================================================
 */

#define MASK(x) (1 << (x))

/* =======================================================================
 * [TYPEDEFS DEFINITIONS]
 * =======================================================================
 */

typedef struct
{
    uint8_t isLeftEyeDetecting;
    uint8_t isRightEyeDetecting;
} eyeState_t;

/* =======================================================================
 * [PUBLIC INTERFACE PROTOTYPES]
 * =======================================================================
 */

/*
 * @brief Inicialización de ambos sensores IR (ojos) del duinoBot
 */
void initDuinoEyes(void);

/*
 * @brief Lectura de los ojos del duinoBot
 * @return Estructura con estado de de los pines de ambos ojos.
 */
eyeState_t checkDuinoEyes(void);

#endif // DUINOEYES_H
