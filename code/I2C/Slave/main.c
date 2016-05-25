#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include <compat/twi.h>
#include "UART.h"

void TWI_init_slave(void);
void TWI_write_slave(void);
void data_Function(unsigned char);
ISR(TWI_vect);

unsigned char recv_data;


int main(void)
{
	
	UART_Init(9600);
	
	DDRC |= _BV(DDC7);
	
	DDRB=0xff;
	TWI_init_slave(); // Function to initialize slave
	sei();
	UART_Send('0');
	while(1){
	while (!(TWCR & (1<<TWINT)));

	}
}

// Function to initialize slave
void TWI_init_slave(void)
{
	// Fill slave address to TWAR
	TWAR=0x40;
	TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWIE)|(1<<TWINT);
}

void data_Function(unsigned char data){
	switch (data)
	{
		case 0x01:
			PORTC |= _BV(PORTC7);
			break;
		default:
			PORTC &= ~_BV(PORTC7);
			break;
	}
}

void TWI_write_slave(void) // Function to write data
{
	// Fill TWDR register with the data to be sent
	TWDR= recv_data;
	// Enable TWI, Clear TWI interrupt flag
	TWCR= (1<<TWEN)|(1<<TWINT);
	// Wait for the acknowledgement
	while((TWSR & 0xF8) != 0xC0);
}

ISR(TWI_vect)
{
	switch (TWSR)
{
		case 0x60:
			UART_Send(0x60);
			TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT)|(1<<TWIE);
			break;
		case 0x80:
			UART_Send(0x80);
			recv_data = TWDR;
			UART_Send(recv_data);
			data_Function(recv_data);
			TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT)|(1<<TWIE);
			break;
		default:
			UART_Send(TWSR);
			break;
	}
}