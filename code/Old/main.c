/*
 * GccApplication2.c
 *
 * Created: 2016-04-20 14:13:13
 * Author : hasse
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"


#define BLINK_DELAY_MS 1000

int main (void)
{
	UART_Init(9600);
	/* set pin 5 of PORTB for output*/
	DDRC |= _BV(DDC0);
	
	while(1) {
		/* set pin 5 high to turn led on */
		PORTC |= _BV(PORTC0);
		_delay_ms(BLINK_DELAY_MS);

		UART_Send('1');
		
		/* set pin 5 low to turn led off */
		PORTC &= ~_BV(PORTC0);
		_delay_ms(BLINK_DELAY_MS);
	}
}

