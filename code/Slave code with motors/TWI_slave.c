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
void motorAuto(float l, float r){




/*
	Motor1F = 100;
	Motor2F = 100;
	Motor1F = 100 * (1 + (l + l));
	Motor2F = 100 * (1 + (r + r));
//	Motor1F = 80;
//	Motor2F = 100;
	Motor1R = 0;
	Motor2R = 0;
	_delay_ms(1000);
	*/
	printf("left = %d ,  right = %d\n",Motor2F, Motor1F);
}
void dataFunction(unsigned char data){

	printf("DATA = %d\n", data);
	switch (data){
		case 1:
			if(Motor1F < 216 && Motor2F < 216){
				if (Motor1F == Motor2F){
					Motor1F += 40;
					Motor2F += 40;
			}
				else {
					Motor1F = 80;
					Motor2F = 80;
			}
			printf("FRAMAT\n");
			printf("%d\n%d\n", Motor1F, Motor2F);
		}	
			break;	
		case 2:
			Motor1F = 0;
			Motor2F = 0;
			Motor1R += 40;
			Motor2R += 40;
			printf("BAKAT\n");
			break;
		case 3:
			Motor1R = 0;
			Motor2R = 0;
			Motor1F = 80;
			Motor2F = 0;
			printf("HOGER\n");
			break;
		case 4:
			Motor1R = 0;
			Motor2R = 0;
			Motor1F = 0;
			Motor2F = 80;
			printf("VANSTER\n");
			break;
		case 5:
			Motor1F = 0;
			Motor2F = 0;
			Motor1R = 0;
			Motor2R = 0;
			printf("STOP\n");
			break;
		default:
			break;
	}
}

void pwmInit(){

	printf("PWM init");

	/* Setting pin 5, 11, 12 and 17 to output on ATMEGA328P */
	DDRD |= (1 << PORTD3)| (1 << PORTD5) | (1 << PORTD6);
	DDRB |= (1 << PORTB3);


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
