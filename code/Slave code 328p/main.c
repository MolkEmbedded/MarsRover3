/*
 * main.c
 *
 *  Created on: 17 maj 2016
 *      Author: marcus
 */


#include "lineSensor.h"
#include "uart.h"
#include "TWI_slave.h"
#include "sonar.h"


int distance_in_cm = 0;



ISR(TWI_vect);
unsigned char recv_data;
unsigned char data;
unsigned char autoMode = 0;
unsigned char manMode = 0;
unsigned char start = 0;


int main() {
	
	pwmInit();
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	/*
	DDRC &= ~(1<<PORT4); // Set SDA to input
	DDRC &= ~(1<<PORT5); // Set SCL to input

	PORTC |= (1<<PC4); // Enable pull-up
	PORTC |= (1<<PC5); // Enable pull-up

	*/
	_delay_ms(100);
	TWI_InitSlave(); // Function to initialize slave
	sei();
	printf("start\n");
	while(1){



		while(manMode){
		printf("HeJ!!!!!\n");
		while (!(TWCR & (1<<TWINT)));
		}
		while(autoMode){
		if(start){
			printf("Mello");
			_delay_ms(3000);
			start = 0;
			}
		float p = 0;
		p = readLineSensors();
	

			 distance_in_cm=read_sonar();

			 _delay_ms(10);

			// printf("%d\n",distance_in_cm);


			 if (distance_in_cm > 10 || distance_in_cm < 0 ) {
					motorAuto(p);
					}else{
					RightF = 0;
					LeftF = 0;
					}
		}
	}

	printf("end\n");
	return 0;
}


ISR(TWI_vect)
{printf("0x%02X\n  ",TWSR);
	switch (TWSR){
		case 0x60:
		TWCR = /*(1<<TWEA)|*/(1<<TWEN)|(1<<TWINT)|(1<<TWIE);
		
		break;
		case 0x80:
		recv_data = TWDR;
		if (recv_data == 7){
			manMode = 1;
			autoMode = 0;
			dataFunction(5);
			}
		else if (recv_data == 8){
			manMode = 0;
			autoMode = 1;
			start = 1;
			}
		else {
			dataFunction(recv_data);
			}
		TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWIE);
		break;
		case 0x88:
		recv_data = TWDR;
			if (recv_data == 7){
				manMode = 1;
				autoMode = 0;
				dataFunction(5);
			}
			else if (recv_data == 8){
				manMode = 0;
				autoMode = 1;
				start = 1;
				_delay_ms(50);
			}
			else {
				dataFunction(recv_data);
			}
		TWCR =  (1<<TWEN)|(1<<TWINT)|(1<<TWEA)|(1<<TWIE);
		break;
		default:
		break;
	}
}


