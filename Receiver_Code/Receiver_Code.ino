#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int triggerPin = 3;
char message[32] = "";
RF24 receiver(5, 6);
const byte address[6] = "00004";

void setup() {
  Serial.begin(9600);
  
  pinMode(triggerPin, OUTPUT);
  
  receiver.begin();
  receiver.openReadingPipe(0, address);
  receiver.setPALevel(RF24_PA_MIN);
  receiver.startListening();
}

void loop() {
  if (receiver.available()) {
    receiver.read(&text, sizeof(text));
    if(text[0] == '0'){
      digitalWrite(triggerPin, 255);
      Serial.println("Opening door");
    } else{
      digitalWrite(triggerPin, 0);
      Serial.println("Closing door");
    }
  }
}
