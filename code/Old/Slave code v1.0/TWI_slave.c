#define F_CPU 16000000UL
#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include <compat/twi.h>
#include "TWI_slave.h"


// Function to initialize slave
void TWI_InitSlave(void){
	// Fill slave address to TWAR
	TWAR=0x40;
	TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWIE)|(1<<TWINT);
}

void dataFunction(unsigned char data){

	printf("DATA = %d\n", data);
	switch (data){
		case 1:
			if(OCR0A < 216 && OCR2A < 216){
				if (OCR0A == OCR2A){
					OCR0A += 40;
					OCR2A += 40;
			}
				else {
					OCR0A = 80;
					OCR2A = 80;
			}
			printf("FRAMAT\n");
			printf("%d\n%d\n", OCR0A, OCR2A);
		}	
			break;	
		case 2:
			OCR0A = 0;
			OCR2A = 0;
			OCR0B += 40;
			OCR2B += 40;
			printf("BAKAT\n");
			break;
		case 3:
			OCR0B = 0;
			OCR2B = 0;
			OCR0A = 80;
			OCR2A = 0;
			printf("HOGER\n");
			break;
		case 4:
			OCR0B = 0;
			OCR2B = 0;
			OCR0A = 0;
			OCR2A = 80;
			printf("VANSTER\n");
			break;
		case 5:
			OCR0A = 0;
			OCR2A = 0;
			OCR0B = 0;
			OCR2B = 0;
			printf("STOP\n");
			break;
		default:
			break;
	}
}

void pwmInit(){

	/* Setting pin 5, 11, 12 and 17 to output on ATMEGA328P */
	DDRD |= _BV(PORTD3)| _BV(PORTD5) | _BV(PORTD6);
	DDRB |= _BV(PORTB3);


	/* Waveform Generation Mode 3 - Fast PWM */
	TCCR0A |= _BV(WGM01) | _BV(WGM00);
	TCCR2A |= _BV(WGM21) | _BV(WGM20);

	/* Clear OC0B/OC2B on Compare Match, set OC0B at BOTTOM */
	TCCR0A |= _BV(COM0A1) | _BV(COM0B1);
	TCCR2A |= _BV(COM2A1) | _BV(COM2B1); //

	/* Setting all motors to 0/OFF on startup */
	OCR0A = 0;
	OCR0B = 0;
	OCR2A = 0;
	OCR2B = 0;

	/* Setting timer to 0 at startup */
	TCNT0 = 0;
	TCNT2 = 0;
	
	/* Setting prescaler to clk/64 */
	TCCR0B |= _BV(CS00)| _BV(CS01);
	TCCR2B |= _BV(CS20)| _BV(CS21);
}
