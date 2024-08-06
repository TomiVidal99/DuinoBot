#ifndef F_CPU
#define F_CPU 16000000UL // se define que frecuencia se emplea: 16MHz
#endif
#ifndef __AVR_ATmega1284P__
#define __AVR_ATmega1284P__ // se define que microcontrolador se emplea
#endif

// distancias que definen el comportanmiento de rotacion y velocidad
#define MAX_DISTANCE 60
#define MID_DISTANCE 30
#define MIN_DISTANCE 10

// velocidades del motor
#define MAX_MOTOR_POWER 100
#define MID_MOTOR_POWER 100
#define MIN_MOTOR_POWER 100

// valor que dispara cuando se detecta una línea
#define DETECTION_THRESHOLD 150