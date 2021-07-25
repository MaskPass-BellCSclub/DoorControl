#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Pin that controls the NPN transistor
int triggerPin = 3;
char message[32] = "";

//CE and CSN pins of the nrf24l01 module
RF24 receiver(5, 6);
//address for the radio module
const byte address[6] = "00004";

void setup() {
  Serial.begin(9600);
  
  //sets the NPN control pin to ouput
  pinMode(triggerPin, OUTPUT);
  
  //starts up the NRF24L01 module in receiver mode
  receiver.begin();
  receiver.openReadingPipe(0, address);
  receiver.setPALevel(RF24_PA_MIN);
  receiver.startListening();
}

void loop() {
  //checks to see if a radio message is available on the specified address
  if (receiver.available()) {
    
    //reads in the message
    receiver.read(&text, sizeof(text));
    
    //checks the received message and if it's 0 opens the door, otherwise closes the door
    if(text[0] == '0'){
      digitalWrite(triggerPin, 255);
      Serial.println("Opening door");
    } else{
      digitalWrite(triggerPin, 0);
      Serial.println("Closing door");
    }
  }
}
