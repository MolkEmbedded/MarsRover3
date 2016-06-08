/*
 * lineSensor.c
 *
 *  Created on: 19 maj 2016
 *      Author: marcus
 */

#include "lineSensor.h"

// functions
/*
void powerOnSensors();
void powerOffSensors();
void readSensor( uint16_t sensorValues[] );
float calculateWeight( uint16_t sensorValues[] );
void engineControl( float avgSensor);
void turnOnLED( uint8_t led);
float PID(float cur_value,float req_value);
*/

// vars
uint16_t *lastValues;
float pGain = 200;   //Proportional Gain
float iGain =  0.2;  //Integral Gain
float dGain =  120;  //Differential Gain

int32_t eInteg = 0;  //Integral accumulator
int32_t ePrev  =0;      //Previous Error

const uint8_t sensorPorts[NUM_SENSORS] = {IRPORT_1, IRPORT_2, IRPORT_3, IRPORT_4, IRPORT_5, IRPORT_6, IRPORT_7, IRPORT_8};
const char sensorRegister[NUM_SENSORS] = {IRREG_1, IRREG_2, IRREG_3, IRREG_4, IRREG_5, IRREG_6, IRREG_7, IRREG_8};

void readLineSensors() {

	powerOnSensors();
	// variable for saving one reading from every sensor
	uint16_t sensorValues[NUM_SENSORS] = {0,0,0,0,0,0,0, 0};
	float avgSensors = 0.0;

	for( uint8_t i = 0; i < 50; i++){
		readSensor( sensorValues );

		//Take current sensor reading
		//return value is between 0 to 7
		//When the line is towards right of center then value tends to 8
		//When the line is towards left of center then value tends to 1
		//When line is in the exact center the the value is 4.5
		avgSensors = calculateWeight( sensorValues );
		setNewSpeed( avgSensors, getPID(avgSensors, 4.5));
//		engineControl( avgSensors);

		_delay_ms(300);
	}
	powerOffSensors();
}


/*
int main(){
	return 0;
}
*/
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

		case 'D':

			DDRD |= (OUT << sensorPorts[sensorNr]);		// set port as OUTPUT
			PORTD |= (1 << sensorPorts[sensorNr]);		// set port HIGH
			_delay_ms(10);								// let port be HIGH for 10 microseconds

			DDRD &= (IN << sensorPorts[sensorNr]);		// set port as INPUT

			// loop while INPUT is HIGH and count time
			while(time < TIMEOUT && ( PIND & _BV(sensorPorts[sensorNr])) ){
				_delay_us(10);
				time += 10;
			}

			break;

		case 'B':
			DDRB |= (OUT << sensorPorts[sensorNr]);		// set port as OUTPUT
			PORTB |= (1 << sensorPorts[sensorNr]);		// set port HIGH
			_delay_ms(10);								// let port be HIGH for 10 microseconds

			DDRB &= (IN << sensorPorts[sensorNr]);		// set port as INPUT

			// loop while INPUT is HIGH and count time
			while(time < TIMEOUT && ( PINB & _BV(sensorPorts[sensorNr])) ){
				_delay_us(10);
				time += 10;
			}

			break;

		case 'C':
				DDRC |= (OUT << sensorPorts[sensorNr]);		// set port as OUTPUT
				PORTC |= (1 << sensorPorts[sensorNr]);		// set port HIGH
				_delay_ms(10);								// let port be HIGH for 10 microseconds

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
	uint8_t compare = 0;
	long avgSensor = 0.0;
	float tmpAllSensors = 0;
	int sensorNr = 0;

	while(sensorNr < NUM_SENSORS){
		compare = (sensorValues[sensorNr] > sensorValues[compare] ? sensorNr : compare);
		printf("(%d) %d, ", sensorNr, sensorValues[sensorNr]);
		sensorNr ++;
	}



	printf("\n");
	// Calculate weight

	for( int i = 0; i < NUM_SENSORS; i ++){

		avgSensor += (float) sensorValues[i] * (i + 1);
		tmpAllSensors += sensorValues[i];

	}

	avgSensor = ( (tmpAllSensors > 0) ? (float) avgSensor / tmpAllSensors: 0xFF);

	return avgSensor;

} // end calculateWeight




float getPID( float cur_position, float new_position) {

  float pid = 0.0;
  float error = new_position - cur_position;

  pid = (Kp * error) + (Ki * eInteg) + (Kd * (error - ePrev));

  eInteg += error;
  ePrev = error;


  return pid;

}

void setNewSpeed( float cur_position, float pid) {
  float changeLeft = 0, changeRight = 0;

  if (pid > 0) {
    // Turn left
    changeLeft = (pid / 2);
    changeRight = -(pid / 2);

  }

  if (pid <= 0) {
    // turn right
    changeLeft = -(pid / 2);
    changeRight = (pid / 2);
  }
  //printf("%f ", cur_position);
  //printFloat("Left", changeLeft);
  //printFloat("Right", changeRight);
  printf("\n");

  // printf("position: %f, Left (%f), Right (%f)\n", cur_position, changeLeft, changeRight);
}
/*
 * turnOnLED
 * @param uint8_t led
 */
void turnOnLED( uint8_t led) {
	switch (led) {

		case BLUE:
			PORTD |= (1 << BLUE);
			PORTD &= ~(1 << RED) & ~(1 << YELLOW);
			break;

		case RED:
			PORTD |= (1 << RED);
			PORTD &= ~(1 << BLUE) & ~(1 << YELLOW);
			break;

		case YELLOW:
			PORTD |= (1 << YELLOW);
			PORTD &= ~(1 << RED) & ~(1 << BLUE);
			break;
		default:
			PORTC &= (0 << BLUE) & (0 << RED) & (0 << YELLOW);
		}
	_delay_ms(100);
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
