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




#define IN 0
#define OUT 1

#define NUM_SENSORS 5
#define TIMEOUT 2000

#define SENSORPOWER PORTD4
#define SENSORPOWERDR DDD4

#define SENSOR_REG1 'D'


// Define IR registers
#define IRREG_8 'D'
//#define IRREG_7 'B'
//#define IRREG_6 'B'
//#define IRREG_5 'B'
#define IRREG_4 'C'
#define IRREG_3 'C'
#define IRREG_2 'C'
#define IRREG_1 'C'

#define IRPORT_8 PORTD2 
//#define IRPORT_7 PORTB2 
//#define IRPORT_6 PORTB4
//#define IRPORT_5 PORTB5
#define IRPORT_4 PORTC0
#define IRPORT_3 PORTC1
#define IRPORT_2 PORTC2
#define IRPORT_1 PORTC3


#define ENGINE_LEFT 0
#define ENGINE_RIGHT 1

#define Kp 0.4    // proportional
#define Kd 0.2   // integral
#define Ki 0.005  // differential



// functions
void readLineSensors();
float getPID( float cur_position, float new_position);
void setNewSpeed(float pid);
void powerOnSensors();
void powerOffSensors();
void readSensor( uint16_t sensorValues[] );
float calculateWeight( uint16_t sensorValues[] );
void engineControl( float avgSensor);
void turnOnLED( uint8_t led);
float PID(float cur_value,float req_value);
void motorAuto(float, float);




#endif /* LINESENSOR_H_ */
