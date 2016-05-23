/*
 * lineSensor.h
 *
 *  Created on: 17 maj 2016
 *      Author: marcus
 */

#ifndef LINESENSOR_H_
#define LINESENSOR_H_

# define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <compat/twi.h>

#include <stdio.h>

#include "uart.h"


#include "TWI_slave.h"


#define IN 0
#define OUT 1

#define NUM_SENSORS 8
#define TIMEOUT 2000

#define SENSORPOWER PORTD4
#define SENSORPOWERDR DDD4

#define SENSOR_REG1 'D'


// Define IR registers
#define IRREG_1 'D'
#define IRREG_2 'B'
#define IRREG_3 'B'
#define IRREG_4 'B'
#define IRREG_5 'C'
#define IRREG_6 'C'
#define IRREG_7 'C'
#define IRREG_8 'C'

#define IRPORT_1 PORTD2
#define IRPORT_2 PORTB2
#define IRPORT_3 PORTB4
#define IRPORT_4 PORTB5
#define IRPORT_5 PORTC0
#define IRPORT_6 PORTC1
#define IRPORT_7 PORTC2
#define IRPORT_8 PORTC3

const uint8_t sensorPorts[NUM_SENSORS] = {IRPORT_1, IRPORT_2, IRPORT_3, IRPORT_4, IRPORT_5, IRPORT_6, IRPORT_7, IRPORT_8};
const char sensorRegister[NUM_SENSORS] = {IRREG_1, IRREG_2, IRREG_3, IRREG_4, IRREG_5, IRREG_6, IRREG_7, IRREG_8};


#define ENGINE_LEFT 0
#define ENGINE_RIGHT 1

#define BLUE PORTD5
#define YELLOW PORTD3
#define RED PORTD4

#define YELLOW_DR DDD3
#define RED_DR DDD4
#define BLUE_DR DDD5

#define Kp 0.1    // proportional
#define Kd 0.01   // integral
#define Ki 0.005  // differential

// functions
float getPID( float cur_position, float new_position);
void setNewSpeed( float cur_position, float pid);
void powerOnSensors();
void powerOffSensors();
void readSensor( uint16_t sensorValues[] );
float calculateWeight( uint16_t sensorValues[] );
void engineControl( float avgSensor);
void turnOnLED( uint8_t led);
float PID(float cur_value,float req_value);

#endif /* LINESENSOR_H_ */
