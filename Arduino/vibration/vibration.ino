// This device is the vibration node 
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin
uint8_t address[][6] = {"M2T", "M2A", "M2V"}; // Let these addresses be used for the pair
byte payload = 0x00;  

//////////// Haptic Drivers
#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 
#include <Wire.h> //I2C library 
SFE_HMD_DRV2605L HMD; //Create haptic motor driver object 
uint8_t rtpn[4] = {24,32,64,128};    //levels of intensity for the motors

void setup() {
  Serial.begin(500000);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  HMD.begin();
  HMD.Mode(0x05);            // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0b00110110);     // 1: LRA Mode ; 011: 4x Braking factor; 01: Medium (default) loop gain ; 10 : 15x (default)  
  HMD.Library(2);            // 1-5 & 7 for ERM motors, 6 for LRA motors
  delay(10);         

  while (!Serial) {
  }
// initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    delay(200);
    while (1) {} // hold in infinite loop
  }
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_max is default.
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 2 bytes
  radio.openReadingPipe(1, address[2]); // using pipe 1}
  radio.startListening(); // put radio in RX mode
}

void loop() {
  uint8_t pipe;

  if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
   uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
   radio.read(&payload, bytes);            // fetch payload from FIFO
   SerialEventWrite(payload);
  }
}

void SerialEventWrite(byte rec){
  if(rec==0x09) {           // label/code 33032
    digitalWrite (2,HIGH);
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);
    delay(5);
    HMD.RTP(rtpn[(0)]);
  }
  if(rec==0x0A) {           // label/code 33033
    digitalWrite (2,HIGH);
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);
    delay(5);
    HMD.RTP(rtpn[(1)]);

  }
  if(rec==0x0B) {           // label/code 33034
    digitalWrite (2,HIGH);
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);
    delay(5);
    HMD.RTP(rtpn[(2)]);


  } 
  if(rec==0x0C){            // label/code 33035
    digitalWrite (2,HIGH);
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);
    HMD.RTP(rtpn[(3)]);
  }
  
  if(rec ==0x13){
    HMD.RTP(0x00);
    Serial.println("STOP");
    digitalWrite (2,LOW);
    digitalWrite (3,LOW);
    digitalWrite (4,LOW);

  }
} 

