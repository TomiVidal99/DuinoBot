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

/** \file duinoStateMachine.c */

/* =======================================================================
 * [INCLUDES]
 * =======================================================================
 */

#include "duinoStateMachine.h"
#include "duinoEyes.h"

/* =======================================================================
 * [PUBLIC INTERFACE FUNCTIONS DEFINITION]
 * =======================================================================
 */

event_t
duinoSMCheckEvent ()
{
    eyeState_t eyes;
    eyes = checkDuinoEyes ();

    if (eyes.isLeftEyeDetecting == FALSE && eyes.isRightEyeDetecting == FALSE)
        return EVENT_BOTH_DOWN;
    else if (eyes.isLeftEyeDetecting == FALSE)
        return EVENT_LEFT_DOWN;
    else if (eyes.isRightEyeDetecting == FALSE)
        return EVENT_RIGHT_DOWN;
    else
        return EVENT_BOTH_UP;
}

void
smHandleIdleState (sm_t *sm, event_t event)
{
    switch (event)
        {
        case EVENT_BOTH_DOWN:
            sm->currentState = STATE_IDLE;
            break;
        case EVENT_BOTH_UP:
            sm->currentState = STATE_ON_LINE;
            break;
        default:
            break;
        }
}

void
smHandleOnlineState (sm_t *sm, event_t event)
{
    switch (event)
        {
        case EVENT_BOTH_DOWN:
            sm->currentState = STATE_REVERSE;
            break;
        case EVENT_LEFT_DOWN:
            sm->currentState = STATE_TURN_LEFT;
            break;
        case EVENT_RIGHT_DOWN:
            sm->currentState = STATE_TURN_RIGHT;
            break;
        default:
            break;
        }
}

void
smHandleTurnLeftState (sm_t *sm, event_t event)
{
    /* Quiero dejar de doblar únicamente cuando detecte linea con ambos sensores */
    if (event == EVENT_BOTH_UP)
        sm->currentState = STATE_ON_LINE;
    else
        sm->currentState = STATE_TURN_LEFT;
}

void
smHandleTurnRightState (sm_t *sm, event_t event)
{
    /* Quiero dejar de doblar únicamente cuando detecte linea con ambos sensores */
    if (event == EVENT_BOTH_UP)
        sm->currentState = STATE_ON_LINE;
    else
        sm->currentState = STATE_TURN_RIGHT;
}

void
smHandleReverseState (sm_t *sm, event_t event)
{
    /* Quiero dejar de ir marcha atras únicamente cuando detecte linea con ambos sensores */
    if (event == EVENT_BOTH_UP)
        sm->currentState = STATE_ON_LINE;
    else
        sm->currentState = STATE_REVERSE;
}

void
smHandleEvent (sm_t *sm, event_t event)
{
    switch (sm->currentState)
        {
        case STATE_IDLE:
            smHandleIdleState (sm, event);
            break;
        case STATE_ON_LINE:
            smHandleOnlineState (sm, event);
            break;
        case STATE_TURN_LEFT:
            smHandleTurnLeftState (sm, event);
            break;
        case STATE_TURN_RIGHT:
            smHandleTurnRightState (sm, event);
            break;
        case STATE_REVERSE:
            smHandleReverseState (sm, event);
            break;
        default:
            break;
        }
}
