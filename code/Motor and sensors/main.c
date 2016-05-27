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



int main() {
	
	pwmInit();
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;

	_delay_ms(100);
	TWI_InitSlave(); // Function to initialize slave
	//sei();

	printf("start1\n");
	//powerOnSensors();
	printf("start2\n");
	while(0){
		printf("loop\n");
		while (!(TWCR & (1<<TWINT)));
	}
	while(1){
	//printf("start3\n");
	readLineSensors();
	//motorAuto(0.1, 0.1);
	/*
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
		 } */
	}

	printf("end\n");
	return 0;
}



/*
 * calculate
 * @param uint16_t sensorValues[]
 */
/*
float calculate( uint16_t sensorValues[] ) {

	// get first highest value
	uint8_t compare = 0;
	float avgSensor = 0.0;
	float tmpAllSensors = 0;
int sensorNr = 0;

while(sensorNr < NUM_SENSORS){
	compare = (sensorValues[sensorNr] > sensorValues[compare] ? sensorNr : compare);
	printf("(%d) %d, ", sensorNr, sensorValues[sensorNr]);
	sensorNr ++;
}



printf("\n");


	//
	//avgSensor = (float) sensorValues[0] * 1 + sensorValues[1] * 2 + sensorValues[2] * 3 + sensorValues[3] * 4 + sensorValues[4] * 5 + sensorValues[5] * 6 + sensorValues[6] *7 + sensorValues[7] *8;
	// Calculate weight

	for( int i = 0; i < NUM_SENSORS; i ++){

		avgSensor += (float) sensorValues[i] * (i + 1);
		tmpAllSensors += sensorValues[i];

	}

	avgSensor = ( (tmpAllSensors > 0) ? (float) avgSensor / tmpAllSensors: 0xFF);


//	printf("avgSensor: %f\n", avgSensor);
	return avgSensor;

}
*/

/*
 *
 */
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
		break;
		default:
		break;
	}
}


