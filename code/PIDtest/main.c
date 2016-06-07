//
//  main.c
//  PIDtest
//
//  Created by Marcus Johansson on 2016-05-19.
//  Copyright © 2016 Marcus Johansson. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

float getPID( float cur_position, float new_position);
float getPD( float cur_position, float new_position);
float getPID2( float cur_position, float new_position);
void setNewSpeed( float cur_position, float pid);
void printFloat( char str[], float nr);
void printAvgValues( uint16_t sensorValues[], uint8_t length );


#define MAXCHANGE 255
#define TIMEOUT 2000
#define NUM_SENSORS 5

#define Kp 0.07   //Proportional Gain
#define Ki 0.3  //Integral Gain
#define Kd 0.3  //Differential Gain

//Define parameter
#define epsilon 0.01
#define dt 0.01 //100ms loop time
#define MAX 4 //For Current Saturation
#define MIN -4

//http://tipswell.com/pid-control-algorithm-using-c-language.html 2/5

//#define Kd 0.004
//#define Kp 0.008 //250    // proportional
                  //#define Kd 0.01   // integral
                  //#define Ki 0.005  // differential

//530, 1050, 2000, 1060, 620

//#define Ki 0.08

#define CENTER 0

int delay = 10;
int lastValue = 0;

int32_t eInteg = 0;  //Integral accumulator
int32_t ePrev  =0;      //Previous Error
int32_t eInteg2 = 0;  //Integral accumulator
int32_t ePrev2  =0;      //Previous Error

int sensorWeightIndex[NUM_SENSORS] = {-127, -64, 0, 64, 127};
//uint8_t sensorWeightIndex[NUM_SENSORS] = {1, 2, 3,4, 5};


int main(int argc, const char * argv[]) {
  
  

  
  //  (0) 760, (1) 590, (2) 470, (3) 720, (4) 2000, (5) 2000, (6) 790, (7) 840
  //  0) 430, (1) 430, (2) 370, (3) 850, (4) 2000, (5) 480, (6) 380, (7) 470,
  //  2000, (1) 2000, (2) 350, (3) 350, (4) 380, (5) 390, (6) 2000, (7) 2000
  /*
   (0) 100, (1) 120, (2) 160, (3) 170, (4) 200, (5) 240, (6) 510, (7) 2000, compare: 7
   (0) 2000, (1) 430, (2) 330, (3) 300, (4) 320, (5) 360, (6) 370, (7) 620, compare: 0
   */
  
  // variable for saving one reading from every sensor
  /*uint16_t sensorValues[8] = {2000,2000, 350, 350,380,390,300,300};
  uint16_t sensorValues2[8] = {2000,2000, 2000, 2000,2000,2000,2000,2000};
  uint16_t sensorValues3[8] = {760,590, 470, 720,2000,2000,790,480};
  uint16_t sensorValues4[8] = {100,120, 160, 170,200,240,510,2000};
   //530, 1050, 2000, 1060, 620
  */
  uint16_t sensorValues6[5] = {40,60, 90, 40,2000};
  uint16_t sensorValues[5] = {2000,2000, 90, 60,40};
   uint16_t sensorValues2[5] = {2000,2000, 2000, 2000,2000};
  uint16_t sensorValues3[5] = {70,50, 2000, 50,40};
  uint16_t sensorValues4[5] = {2000,220, 112, 240,160};
  uint16_t sensorValues5[5] = {530,1050, 2000, 1060,620};
  
  
   printAvgValues(sensorValues, NUM_SENSORS);
  printAvgValues(sensorValues3, NUM_SENSORS);
  printAvgValues(sensorValues2, NUM_SENSORS);
  printAvgValues(sensorValues4, NUM_SENSORS);
  printAvgValues(sensorValues5, NUM_SENSORS);
    printAvgValues(sensorValues6, NUM_SENSORS);
  // Calculate weight

  
    return 0;
}


float getPID2( float cur_position, float new_position) {
  
  float pid = 0.0;
  float error = new_position - cur_position;
  
  pid = (Kp * error) + (Ki * eInteg2) + (Kd * (error - ePrev2));
  
  eInteg2 += error;
  ePrev2 = error;
  
  //  printf("PiD (%f) cur_position: %f, new_position: %f\n", pid, cur_position, new_position);
  return pid;
  
}
float getPID( float cur_position, float new_position) {
  
  float pid = 0.0;
  float error = new_position - cur_position;

  pid = (Kp * error) + (Ki * eInteg) + (Kd * (error - ePrev));

  eInteg += error;
  ePrev = error;
  
  //  printf("PiD (%f) cur_position: %f, new_position: %f\n", pid, cur_position, new_position);
  return pid;

}
float getPD( float cur_position, float new_position) {
  
  float pid = 0.0;
  float error = new_position - cur_position;
  
  pid = (Kp * error) + (Kd * (error - ePrev));
  
  eInteg += error;
  ePrev = error;
  
  //  printf("PD (%f) cur_position: %f, new_position: %f\n", pid, cur_position, new_position);
  
  return pid;
  
}

void setNewSpeed( float cur_position, float pid) {
  
  float changeLeft = 0, changeRight = 0;
  
  //if(pid > 100) pid = 100;
  // if (pid< -100) pid = -100;
  if (pid > 0) {
    printf("turn left: %f\n", -pid);
  } else  if (pid < 0) {
    printf("turn right: %f\n", pid);
  } else {
    printf("no turn: %f\n", pid);
  }
  
  //  printf("currentPosition: %f\n", cur_position);
  
  /*
  if (pid > 0) {
    // Turn left
    changeLeft = (pid / 2);
    changeRight = -(pid / 2);
    printf("pid1\n");
  }
  
  if (pid <= 0) {
    // turn right
    changeLeft = (pid / 2);
    changeRight = -(pid / 2);
    printf("pid2\n");
  }
  printf("%f ", cur_position);
  printFloat("Left", changeLeft);
  printFloat("Right", changeRight);
  printFloat("PID", pid);
  printf("\n");
  */
  // printf("position: %f, Left (%f), Right (%f)\n", cur_position, changeLeft, changeRight);
}


void printFloat( char str[], float nr){
  
  char *num = (char *) malloc(sizeof(nr));
  sprintf(num,"%g", nr);
  
  printf("%s: %s ",str, num);
  
  free(num);
}


void printAvgValues( uint16_t sensorValues[], uint8_t length ) {
  
  float avg = 0;
  int tmpValues = 0;
  
  int sensorNr = 0;
  uint8_t compare = 0;
  
  
  if (sensorValues[0] == 2000 && sensorValues[1] == 2000 && sensorValues[2] == 2000 && sensorValues[5] < 2000) {
    // turn quick left
    printf("Quick left\n");
    
  } else if(sensorValues[2] == 2000 && sensorValues[3] == 2000 && sensorValues[4] == 2000  && sensorValues[0] < 2000) {
    // turn quick right
    printf("Quick right\n");
    
  } else {
    
    
    while(sensorNr < NUM_SENSORS){
      compare = ((sensorValues[sensorNr] > 1500 && sensorValues[sensorNr] > sensorValues[compare]) ? sensorNr : compare);
      printf("(%d) %d, ", sensorNr, sensorValues[sensorNr]);
      tmpValues += sensorValues[sensorNr];
      sensorNr ++;
    }
    
    if (tmpValues == (TIMEOUT * NUM_SENSORS)) {
      printf("\nstopp\n");
    }
    
  }
 
  
  printf("\n");
  sensorNr = 0;
  while(sensorNr < NUM_SENSORS){
    //  compare = (sensorValues[sensorNr] > sensorValues[compare] ? sensorNr : compare);
    printf("(%d) %d, ", sensorNr, sensorWeightIndex[sensorNr]);
    sensorNr ++;
  }

  printf("\ncompare: %d\n", compare);
  for (int i = 0; i < length; i++) {
    
    avg += (float) sensorValues[i] * sensorWeightIndex[i];
    tmpValues += sensorValues[i];
    
  }
  avg = ( tmpValues > 0 ? (float) avg / tmpValues: 0xFF);


   printf("avg: %f,tmpValues %d\n", avg, tmpValues);
  // setNewSpeed(avg, getPID(sensorWeightIndex[compare], CENTER));
   setNewSpeed(avg, getPD( sensorWeightIndex[compare], CENTER));
  //  setNewSpeed(avg, getPID(avg, CENTER));
  lastValue = tmpValues;
  printf("\n\n");
}