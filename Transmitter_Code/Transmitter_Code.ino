#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// CE, CSN pins for the NRF24L01 module
RF24 radioModule(7, 8);
const byte address[6] = "00004";

//declarations for the communication with the GUI over serial
const byte sizeOfBuffer = 40;
char inputFromPC[sizeOfBuffer];
const char startMarker = '<';
const char endMarker = '>';
byte numbOfBytesReceived = 0;
boolean readInProgress = false;
boolean newMessageReceived = false;
unsigned long curMillis;
unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;

//char container for open/close command input
char triggerChar = '7';

void setup() {
  Serial.begin(9600);
  
  //setup the NRF24L01 module in transmit mode
  radioModule.begin();
  radioModule.openWritingPipe(address);
  radioModule.setPALevel(RF24_PA_MIN);
  radioModule.stopListening();
  
  //send arduino is ready message to pc
  Serial.println("<Arduino is ready>");
}


void loop() {
  //get incoming commands from pc and reply with confirmation message
  curMillis = millis();
  getDataFromPC();
  replyToPC();
}


void getDataFromPC() {
   
  //checks if there is a message available to be read
  if(Serial.available() > 0) {

    char currentChar = Serial.read();
     
    //if the char that's been read is the end marker, the message has ended and program moves on the sendCommand() function
    if (currentChar == endMarker) {
      readInProgress = false;
      newMessageReceived = true;
      inputFromPC[numbOfBytesReceived] = 0;
      sendCommand();
    }
    
    //if the char that's been read is not the end marker and the read is in progress, read the incomming command
    if(readInProgress) {
      inputFromPC[numbOfBytesReceived] = currentChar;
      numbOfBytesReceived ++;
      if (numbOfBytesReceived == sizeOfBuffer) {
        numbOfBytesReceived = sizeOfBuffer - 1;
      }
    }
    
    //if the char that's been read is the start marker, turns readInProgress to true
    if (currentChar == startMarker) { 
      numbOfBytesReceived = 0; 
      readInProgress = true;
    }
  }
}

 
void sendCommand() {
  //takes the input from pc and changes it to a form that can be sent over the NRF24L01 module
  triggerChar = inputFromPC[0];
  char text[32] = "7";
  text[0] = triggerChar;
  
  //sends the input command over the NRF23L01 module
  radioModule.write(&text, sizeof(text));
}


void replyToPC() {
  
  //sends the confirmation message back to the pc
  if (newMessageReceived) {
    newMessageReceived = false;
    Serial.print("<Received Command: ");
    Serial.print(inputFromPC);
    Serial.println(">");
  }
  
}
