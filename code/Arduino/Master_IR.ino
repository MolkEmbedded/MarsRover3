#include <Wire.h>
#include <IRremote.h>
#include <IRremoteInt.h>
//Master

#define slave_device_address 0x40

int led = 13;
int RECV_PIN = 11;
int x, y;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup(){
  Wire.begin(); // Start I2C BUS as Master
  irrecv.enableIRIn(); // Start the receiver
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if (irrecv.decode(&results)){
    Serial.println(results.value, HEX);
    x = results.value;
    irrecv.resume(); // Receive the next value
  }


switch(x){

//Framåt
case 0x4658:
  y = 1;
  Serial.println("Vad som hels1t");
  Wire.beginTransmission(slave_device_address);
  Serial.println("Vad som hels1t23");
  Wire.write(1);
  Serial.println("Vad som123 hels1t23");
  Wire.endTransmission();
  Serial.println("Vad som helst");
  x = 0;
  break;

//Bakåt
case 0x4659:
  y = 2;
  Wire.beginTransmission(slave_device_address);
  Wire.write(2);
  Wire.endTransmission();
  x = 0;
  break;
  
//Höger
  case 0x465B:
  y = 3;
  Wire.beginTransmission(slave_device_address);
  Wire.write(3);
  Wire.endTransmission();
  x = 0;
  break;
 
//Vänster  
  case 0x1465A:
  y = 4;
  Wire.beginTransmission(slave_device_address);
  Wire.write(4);
  Wire.endTransmission();
  x = 0;
  break;
  
//Emergency STOP
  case 0x14692:
  y = 5;
  Wire.beginTransmission(slave_device_address);
  Wire.write(5);
  Wire.endTransmission();
  x = 0;
  break;
}
}


