#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 11;
int val;
int led = 13;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    val = results.value;
    Serial.println(val);
    irrecv.resume(); // Receive the next value
  }
  
 if (val == 18010){ 
  digitalWrite(led,HIGH);
  }
    
 if (val == 18119){ 
  digitalWrite(led,LOW);
  }
  
  
  delay(100);
}
