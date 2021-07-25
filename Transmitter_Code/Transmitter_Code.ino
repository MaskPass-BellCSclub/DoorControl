#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radioModule(7, 8); // CE, CSN
const byte address[6] = "00004";

const byte sizeOfBuffer = 40;
char inputFromPC[sizeOfBuffer];
const char startMarker = '<';
const char endMarker = '>';
byte numbOfBytesReceived = 0;
boolean readInProgress = false;
boolean newMessageReceived = false;

char triggerChar = '7';

unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;


void setup() {
  Serial.begin(9600);

  radioModule.begin();
  radioModule.openWritingPipe(address);
  radioModule.setPALevel(RF24_PA_MIN);
  radioModule.stopListening();
  
  Serial.println("<Arduino is ready>");
}


void loop() {
  curMillis = millis();
  getDataFromPC();
  replyToPC();
}


void getDataFromPC() {
    
  if(Serial.available() > 0) {

    char currentChar = Serial.read();
      
    if (currentChar == endMarker) {
      readInProgress = false;
      newMessageReceived = true;
      inputFromPC[numbOfBytesReceived] = 0;
      sendCommand();
    }
    
    if(readInProgress) {
      inputFromPC[numbOfBytesReceived] = currentChar;
      numbOfBytesReceived ++;
      if (numbOfBytesReceived == sizeOfBuffer) {
        numbOfBytesReceived = sizeOfBuffer - 1;
      }
    }

    if (currentChar == startMarker) { 
      numbOfBytesReceived = 0; 
      readInProgress = true;
    }
  }
}

 
void sendCommand() {
  triggerChar = inputFromPC[0];
  char text[32] = "7";
  text[0] = triggerChar;

  radioModule.write(&text, sizeof(text));
}


void replyToPC() {
  
  if (newMessageReceived) {
    newMessageReceived = false;
    Serial.print("<Received Command: ");
    Serial.print(inputFromPC);
    Serial.println(">");
  }
  
}
