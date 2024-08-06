/* **************************************************************************
 *
 *        Título: Maquina de estados sigue linea duinoBot.
 *           Año: 2024
 *
 * Desarrollador: Ignacio Brittez
 *
 *      e-mail: ignacio_brittez@outlook.com
 *
 * ************************************************************************** */

/** \file duinoStateMachine.h */

#ifndef DUINOSTATEMACHINE_H
#define DUINOSTATEMACHINE_H

/* =======================================================================
 * [MACROS]
 * =======================================================================
 */

#define TRUE 1
#define FALSE 0

/* =======================================================================
 * [TYPEDEF DEFINITIONS]
 * =======================================================================
 */

typedef enum
{
    STATE_IDLE,
    STATE_ON_LINE,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_REVERSE,
} state_t;

typedef enum
{
    EVENT_BOTH_DOWN,
    EVENT_LEFT_DOWN,
    EVENT_RIGHT_DOWN,
    EVENT_BOTH_UP
} event_t;

typedef struct
{
    state_t currentState;
} sm_t;

/* =======================================================================
 * [PUBLIC INTERFACE PROTOTYPES]
 * =======================================================================
 */


/*
 * @brief Lee el estado de los ojos para verificar cual es el evento actual.
 * @warning INICIALIZAR LOS OJOS DEL DUINO ANTES.
 * @return evento actual.
 */
event_t duinoSMCheckEvent ();

/*
 * @brief Maneja el estado STATE_IDLE.
 * @param sm: maquina de estados bajo ejecucción.
 * @param event: evento entrante
 */
void smHandleIdleState (sm_t *sm, event_t event);

/*
 * @brief Maneja el estado STATE_ONLINE.
 * @param sm: maquina de estados bajo ejecucción.
 * @param event: evento entrante
 */
void smHandleOnlineState (sm_t *sm, event_t event);

/*
 * @brief Maneja el estado STATE_TURN_LEFT.
 * @param sm: maquina de estados bajo ejecucción.
 * @param event: evento entrante
 */
void smHandleTurnLeftState (sm_t *sm, event_t event);

/*
 * @brief Maneja el estado STATE_TURN_RIGHT.
 * @param sm: maquina de estados bajo ejecucción.
 * @param event: evento entrante
 */
void smHandleTurnRightState (sm_t *sm, event_t event);

/*
 * @brief Maneja el estado STATE_REVERSE.
 * @param sm: maquina de estados bajo ejecucción.
 * @param event: evento entrante
 */
void smHandleReverseState (sm_t *sm, event_t event);

/*
 * @brief Desencadena el manejo de cada estado a partir del evento entrante.
 * @param sm: maquina de estados bajo ejecucción.
 * @param event: evento entrante
 */
void smHandleEvent (sm_t *sm, event_t event);

#endif // DUINOSTATEMACHINE_H
