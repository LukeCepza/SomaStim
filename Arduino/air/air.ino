// This device is the air node 
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin
uint8_t address[][6] = {"M2T", "M2A", "M2V"};// Let these addresses be used for the pair
byte payload = 0x00;  

uint8_t fz[4] = {63,127,191,255};    //levels of intensity for the motors

void setup() {
  pinMode(2, OUTPUT);     // set pin 2 as output (anological output)
  pinMode(3, OUTPUT);     // set pin 3 as output (anological output)
  pinMode(4, OUTPUT);     // set pin 4 as output (anological output)

  Serial.begin(500000);     // baudrate of 500000

  // --- From RF24 library example: GettingStarted --- //
  // configure RF
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    delay(200);
    while (1) {} // hold in infinite loop
  }
  radio.setPALevel(RF24_PA_LOW);          // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 2 bytes
  radio.openReadingPipe(1, address[1]);   // pipe 2 "M2A" - Master to Air
  radio.startListening();                 // put radio in RX mode
}

void loop() {
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&payload, bytes);            // fetch payload from FIFO
      SerialEventWrite(payload);
    }
}

// function SerialEventWrite read the label/code (or payload) sent from Master module
// and writes their correspond level of intensity
void SerialEventWrite(byte rec){
  // checks if label is the one for marking the threshold 
  //else, then ths is false, so the STOP label received will affect the stimuli actions
    if(rec==0x05) {           // label/code 33024
      analogWrite(2,255);  
      analogWrite(3,255);   //overshoot to start motor
      analogWrite(3,255);  delay(5); //overshoot to start motor

      analogWrite(2,fz[0]);
      analogWrite(3,fz[0]);
      analogWrite(4,fz[0]);

     }
    if(rec==0x06) {           // label/code 33025
      analogWrite(2,255);  
      analogWrite(3,255);   //overshoot to start motor
      analogWrite(3,255);  delay(5); //overshoot to start motor
      analogWrite(2, fz[1]);    // 2/4 of intensity
      analogWrite(3, fz[1]);    // 2/4 of intensity
      analogWrite(4, fz[1]);    // 2/4 of intensity

     }
    if(rec==0x07) {           // label/code 33026
      analogWrite(2, fz[2]);    // 2/4 of intensity
      analogWrite(3, fz[2]);    // 2/4 of intensity
      analogWrite(4, fz[2]);    // 2/4 of intensity


     }  
    if(rec==0x08){            // label/code 33027
      analogWrite(2, fz[3]);    // 2/4 of intensity
      analogWrite(3, fz[3]);    // 2/4 of intensity
      analogWrite(4, fz[3]);    // 2/4 of intensity
     }
    if (rec == 0x16){     // STOP STIMULI LABEL
      analogWrite (2,0);
      analogWrite (3,0);
      analogWrite (4,0);

     }
  }