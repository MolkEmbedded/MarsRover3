#define F_CPU 16000000UL
#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include <compat/twi.h>
#include "TWI_And_Motors.h"


// Function to initialize slave
void TWI_InitSlave(void){

	// Set slave address to TWAR
	TWAR=0x40;
	TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWIE)|(1<<TWINT);	
}
void motorAuto(float pid){

	if (pid > 100) pid = 100;
	if (pid < -100) pid = -100;
	if (pid < 0) {
		LeftF = leftSpeed;
		RightF = rightSpeed + pid;
	} 
	else if (pid > 0) {
		LeftF = leftSpeed - pid;
		RightF = rightSpeed;
	}
	 else {
		LeftF = leftSpeed;
		RightF = rightSpeed;
	}
}
void motorIR(unsigned char data){

	switch (data){
		case 1:
			if(RightF < 216 && LeftF < 216){
				if (RightF == LeftF){
					RightF += 40;
					LeftF += 40;
			}
				else {
					RightF = 80;
					LeftF = 80;
			}
		}	
			break;	
		case 2:
			RightF = 0;
			LeftF = 0;
			RightR += 40;
			LeftR += 40;
			break;
		case 3:
			RightR = 0;
			LeftR = 0;
			LeftF = 115;
			RightF = 0;
			break;
		case 4:
			RightR = 0;
			LeftR = 0;
			LeftF = 0;
			RightF = 115;
			break;
		case 5:
			RightF = 0;
			LeftF = 0;
			RightR = 0;
			LeftR = 0;
			break;
		default:
			break;
	}
}

void motorDance(){
		LeftF = 200;
		LeftR = 0;
		RightF = 0;
		RightR = 200;
		_delay_ms(2000);
		LeftF = 0;
		LeftR = 200;
		RightF = 200;
		RightR = 0;
		_delay_ms(2000);
		LeftF = 0;
		LeftR = 0;
		RightF = 0;
		RightR = 0;
		_delay_ms(2500);
		for(; ; );
	}

void pwmInit(){

	/* Setting pin 5, 11, 12 and 17 to output on ATMEGA328P */
	DDRD |= (1 << PORTD3)| (1 << PORTD5) | (1 << PORTD6);
	DDRB |= (1 << PORTB3);

	/* Waveform Generation Mode 3 - Fast PWM */
	TCCR0A |= _BV(WGM01) | _BV(WGM00);
	TCCR2A |= _BV(WGM21) | _BV(WGM20);

	/* Clear OC0B/OC2B on Compare Match, set OC0B at BOTTOM */
	TCCR0A |= _BV(COM0A1) | _BV(COM0B1);
	TCCR2A |= _BV(COM2A1) | _BV(COM2B1); 

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
