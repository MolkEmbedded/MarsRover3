#define F_CPU 16000000UL
#include <stdio.h>
#include<avr/io.h>
#include<util/delay.h>
#include<inttypes.h>
#include<avr/interrupt.h>
#include <compat/twi.h>
#include "uart.h"

void TWI_start(void);
void TWI_repeated_start(void);
void TWI_init_master(void);
void TWI_write_address(unsigned char);
void TWI_read_address(unsigned char);
void TWI_write_data(unsigned char);
void TWI_read_data(void);
void TWI_stop(void);
ISR(TWI_vect);

unsigned char address=0x40, read=1, write=0;
unsigned char data, write_data=0x01, recv_data;
unsigned int val = 0;
char mesg[2] = "OK";

int main(void)
{
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	
		DDRC &= ~_BV(DDC4);
		DDRC &= ~_BV(DDC5);

		PORTC |= _BV(PORTC4);
		PORTC |= _BV(PORTC5);
	TWI_init_master();   // Function to initialize TWI
	sei();
		
		
		// Function to send start condition
		TWI_start();

		for ( ; ; );
}

void TWI_init_master(void) // Function to initialize master
{
TWBR = 0x0B;                                  // Set bit rate register. Defined in header file.
TWDR = 0xFF;                                      // Default content = SDA released.
TWCR = (1<<TWEN)|(1<<TWIE);   

}

void TWI_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
	while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
}


void TWI_write_address(unsigned char data)
{
	TWDR=data;  // Address and write instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8)!= 0x18);  // Check for the acknowledgement
	
}

void TWI_read_address(unsigned char data)
{
	TWDR=data;  // Address and read instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
	while((TWSR & 0xF8)!= 0x40);  // Check for the acknowledgement
}



void TWI_read_data(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknowledgement
	recv_data=TWDR;
	PORTB=recv_data;
}

void TWI_stop(void)
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}

ISR(TWI_vect)
{
	switch (TWSR)
	{
		case 0x08:
			data = 0x40;
			TWDR=data;  
			printf("%s", mesg);
			TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
			break;
		case 0x18:
			data = 0x01;
			TWDR = data;
			TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
			break;
		case 0x28:
		
		if ( val < 21){
			val++;
		printf("%d \n", val);
			if (data == 0x01){
			_delay_ms(1000);
				data = 0x02;
				}
			else{
				data = 0x01;
				_delay_ms(1000);
				}
			TWDR=data;  // put data in TWDR
			TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
			break;
			}
			else {
			TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
			break;
			}
		default:
			break;

	}
}