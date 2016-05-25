#define F_CPU 16000000UL
#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include <compat/twi.h>
#include "uart.h"
#include "TWI_slave.h"

ISR(TWI_vect);

unsigned char recv_data;


int main(void)
{
	
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	
	pwmInit();
	TWI_InitSlave(); // Function to initialize slave
	sei();

	while(1){
		printf("loop\n");
		while (!(TWCR & (1<<TWINT)));

	}
}

ISR(TWI_vect)
{
	switch (TWSR){
		case 0x60:
		TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT)|(1<<TWIE);
		break;
		case 0x80:
		recv_data = TWDR;
		dataFunction(recv_data);
		TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE)| ~(1<<TWEA);
		break;
		case 0x88:
		TWCR =  (1<<TWEN)|(1<<TWINT)|(1<<TWEA)|(1<<TWIE);
		default:
		break;
	}
}

