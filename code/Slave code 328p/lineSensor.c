/*
 * lineSensor.c
 *
 *  Created on: 19 maj 2016
 *      Author: marcus
 */
#include "TWI_slave.h"
#include "lineSensor.h"

int count = 0;
float error = 0;

uint8_t compare = 2;
const uint8_t sensorPorts[NUM_SENSORS] = {IRPORT_1, IRPORT_2, IRPORT_3, IRPORT_4,/* IRPORT_5, IRPORT_6, IRPORT_7,*/ IRPORT_8};
const char sensorRegister[NUM_SENSORS] = {IRREG_1, IRREG_2, IRREG_3, IRREG_4,/* IRREG_5, IRREG_6, IRREG_7,*/ IRREG_8};
const int sensorWeight[NUM_SENSORS] = {-127, -100, 0, 100, 127};


// vars
uint16_t *lastValues;

int32_t eInteg = 0;  //Integral accumulator
int32_t ePrev  =0;      //Previous Error



float readLineSensors() {


	// variable for saving one reading from every sensor
	uint16_t sensorValues[NUM_SENSORS] = {0,0,0,0,0};

	readSensor( sensorValues );
		
	uint8_t compare = 0;
	int sensorNr = 0;
	
	while(sensorNr < NUM_SENSORS){
		compare = (sensorValues[sensorNr] > sensorValues[compare] ? sensorNr : compare);
		//printf("(%d) %d, ", sensorNr, sensorValues[sensorNr]);
		sensorNr ++;
	}
	
	if (sensorValues[0] > 1500 && sensorValues[1] > 1500 && sensorValues[2] > 1500 && sensorValues[3] > 1500 && sensorValues[4] > 1500){
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

		//Take current sensor reading
		//return value is between 0 to 7
		//When the line is towards right of center then value tends to 8
		//When the line is towards left of center then value tends to 1
		//When line is in the exact center the the value is 4.5
		return calculateWeight( sensorValues );

}



/*
 * readSensor
 * @param uint8_16 sensorValues[]
 */
void readSensor( uint16_t sensorValues[] ) {

	/*
	1. Turn on IR LEDs (optional).
	2. Set the I/O line to an output and drive it high.
	3. Allow at least 10 μs for the sensor output to rise.
	4. Make the I/O line an input (high impedance).
	5. Measure the time for the voltage to decay by waiting for the I/O line to go low.
	6. Turn off IR LEDs (optional).
	*/
	// read values from every IRsensor
	for( uint8_t sensorNr = 0; sensorNr < NUM_SENSORS; sensorNr ++){

		// variable for saving past time when sensors go LOW
		uint16_t time = 0;

		// check which register the sensor is connected to
		// and perform action...
		switch( sensorRegister[sensorNr]){
		//printf("Switch\n");
		case 'D':

			DDRD |= (OUT << sensorPorts[sensorNr]);		// set port as OUTPUT
			PORTD |= (OUT << sensorPorts[sensorNr]);		// set port HIGH
			_delay_us(10);								// let port be HIGH for 10 microseconds

			DDRD &= ~(1 << PORTD2);		
			//DDRD &= ~(OUT << sensorPorts[sensorNr]);		// set port as INPUT

			// loop while INPUT is HIGH and count time
			while(time < TIMEOUT && ( PIND & _BV(sensorPorts[sensorNr])) ){
				_delay_us(10);
				time += 10;
			}

			break;

		case 'C':
				DDRC |= (OUT << sensorPorts[sensorNr]);		// set port as OUTPUT
				PORTC |= (1 << sensorPorts[sensorNr]);		// set port HIGH
				_delay_us(10);								// let port be HIGH for 10 microseconds

				DDRC &= (IN << sensorPorts[sensorNr]);		// set port as INPUT

				// loop while INPUT is HIGH and count time
				while(time < TIMEOUT && ( PINC & _BV(sensorPorts[sensorNr])) ){
					_delay_us(10);
					time += 10;
				}

			break;

		} // end switch
	// save past time for sensors to get LOW
	sensorValues[sensorNr ] = time;
	} // end for



}


/*
 * calculate
 * @param uint16_t sensorValues[]
 */
float calculateWeight( uint16_t sensorValues[] ) {

	// get first highest value
	long avgSensor = 0.0;
	float tmpAllSensors = 0;
	int sensorNr = 0;
	
	while(sensorNr < NUM_SENSORS){
		if(sensorValues[sensorNr] > 1500)
		compare = (sensorValues[sensorNr] > sensorValues[compare] ? sensorNr : compare);
		//printf("(%d) %d, ", sensorNr, sensorValues[sensorNr]);
		sensorNr ++;
	}
	// Calculate weight

	for( int i = 0; i < NUM_SENSORS; i ++){

		avgSensor += (float) sensorValues[i] * sensorWeight[i];
		tmpAllSensors += sensorValues[i];

	}
	avgSensor = ( (tmpAllSensors > 0) ? (float) avgSensor / tmpAllSensors: 0xFF);

	return getPID(sensorWeight[compare], 0);

} // end calculateWeight




float getPID( float cur_position, float new_position) {

  float pid = 0.0;

	error = new_position - cur_position;

  pid = (Kp * error) /*+ (Ki * eInteg) */+ (Kd * (cur_position - ePrev));

  
  printf("%d\n", (int) pid);
	eInteg += error;
	for (int i = 0; i < 5; i++)
		ePrev = cur_position;


  return pid;

}


/*
 * powerOnSensors
 */
void powerOnSensors() {
	PORTB |= (1 << SENSORPOWER);
}

/*
 * powerOffSensors
 */
void powerOffSensors() {
	PORTB &= ~(1 << SENSORPOWER);
}
