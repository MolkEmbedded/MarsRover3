/*!
 * **********************************************************************************************
 * \file test_sonar.c
 * \brief Testing sonar program using HC-SR04 Ultrasonic Sensor
 *
 * \author      :   Praveen Kumar
 * \date        :   Mar 24, 2014
 * Copyright(c) :   Praveen Kumar - www.veerobot.com
 * Description  :   Testing sonar program using HC-SR04 Ultrasonic Sensor
 *
 * LICENSE      :   Redistribution and use in source and/or binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain this copyright notice, list of conditions and disclaimer.
 * - Redistributions in binary form must reproduce this copyright notice, list of conditions and disclaimer in
 *      documentation and/or other materials provided with the distribution.
 *
 * DISCLAIMER   :   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" WITHOUT ANY
 * KIND OF WARRANTIES. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE
 *
 * Usage        :   Ultrasonic Sensor HC-SR04 module is tested with following configuration:
 *                  Board:  Draco - AVR development board
 *                  Pins:   Trigger Pin :   PD0
 *                          Echo Pin    :   PD1
 *                          Crystal     :   16MHz External
 *                          Fuse Bits   :   l:EE, h:D9, E:07
 *                                  Microcontroller :   ATmega328P
 *                                  Timer used  :   16 bit timer1
 * **********************************************************************************************
 */

#define F_CPU 8000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "sonar.h"
 
int main(){
    int distance_in_cm=0;
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;
 
    while(1)
    {
        distance_in_cm=read_sonar();
        if (distance_in_cm == TRIG_ERROR)
        {
            puts("Error!");
            _delay_ms(DELAY_BETWEEN_TESTS/2);
            puts("");
            _delay_ms(DELAY_BETWEEN_TESTS/2);
 
        }
        else if (distance_in_cm == ECHO_ERROR)
        {
            puts("Echo error!");
            _delay_ms(DELAY_BETWEEN_TESTS);
            puts("");
        }
        else
        {

			printf("Distance(cm): %d",distance_in_cm);

            _delay_ms(DELAY_BETWEEN_TESTS);
            puts("");
        }
    }
    return 0;
}