/*
 * main.c
 *
 *  Created on: 17 maj 2016
 *      Author: marcus
 */

#include "lineSensor.h"


// vars


ISR(TWI_vect);
unsigned char recv_data;


int main() {

	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;

	//pwmInit();
	TWI_InitSlave(); // Function to initialize slave
	sei();

	printf("start\n");
	powerOnSensors();

	while(0){
		printf("loop\n");
		while (!(TWCR & (1<<TWINT)));
	}
	// variable for saving one reading from every sensor
	uint16_t sensorValues[NUM_SENSORS] = {0,0,0,0,0,0,0, 0};
	float avgSensors = 0.0;

	for( uint8_t i = 0; i < 50; i++){
	//	readSensor( sensorValues );

		//Take current sensor reading
		//return value is between 0 to 7
		//When the line is towards right of center then value tends to 8
		//When the line is towards left of center then value tends to 1
		//When line is in the exact center the the value is 4.5
	//	avgSensors = calculate( sensorValues );
	//	setNewSpeed( avgSensors, getPID(avgSensors, 4.5));


		_delay_ms(300);
	}




		powerOffSensors();


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


