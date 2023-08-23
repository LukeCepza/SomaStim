// This device is the vibration node 

// RF module libraries
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
RF24 radio(9, 10);                                // Arduino pin 9 for the CE pin and pin 10 for the CSN pin
uint8_t address[][6] = {"M2T", "M2A", "M2V"};     // Addresses for the pair of modules (Master to Touch | Master to Air | Master to Vibration)
byte payload = 0x00; 

// Haptic Drivers libraries
#include <Sparkfun_DRV2605L.h>        //SparkFun Haptic Motor Driver Library 
#include <Wire.h>                     //I2C library 
SFE_HMD_DRV2605L HMD;                 //Create haptic motor driver object 

uint8_t rtpn[4] = {24,32,64,128};     //haptic motores intensity levels


void setup() {
  pinMode(2, OUTPUT);       // set pin 2 as output (digital output)
  pinMode(3, OUTPUT);       // set pin 3 as output (digital output)
  pinMode(4, OUTPUT);       // set pin 4 as output (digital output)

  Serial.begin(500000);     // baudrate of 500000
  
  // initialize haptic drivers (configuration in DRV2605 datasheet)
  HMD.begin();
  HMD.Mode(0x05);               // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0b00110110);  // 1: LRA Mode ; 011: 4x Braking factor; 01: Medium (default) loop gain ; 10 : 15x (default)  
  HMD.Library(2);               // 1-5 & 7 for ERM motors, 6 for LRA motors
  delay(10);         

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
  radio.setPALevel(RF24_PA_LOW);          // RF24_PA_max is default.
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 2 bytes
  radio.openReadingPipe(1, address[2]);   // pipe 3 "M2V" - Master to Vibration 
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


void SerialEventWrite(byte rec){
  // function SerialEventWrite reads the label/code (or payload) received from Master module
  // and writes their correspond level of intensity into the output pins (haptic drivers and motors)
  
  if(rec==0x09) {   // label/code 33032
    digitalWrite (2,HIGH);    //activate haptic drivers with digital output
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);             // overshoot to start motor
    delay(5);
    HMD.RTP(rtpn[(0)]);       // 1/4 of intensity
  }
  if(rec==0x0A) {   // label/code 33033
    digitalWrite (2,HIGH);    //activate haptic drivers with digital output
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);             // overshoot to start motor
    delay(5);
    HMD.RTP(rtpn[(1)]);       // 2/4 of intensity
  }
  if(rec==0x0B) {   // label/code 33034
    digitalWrite (2,HIGH);    //activate haptic drivers with digital output
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);             // overshoot to start motor
    delay(5);
    HMD.RTP(rtpn[(2)]);       // 3/4 of intensity
  } 
  if(rec==0x0C){    // label/code 33035
    digitalWrite (2,HIGH);    //activate haptic drivers with digital output
    digitalWrite (3,HIGH);
    digitalWrite (4,HIGH);
    HMD.RTP(100);             // overshoot to start motor
    delay(5);
    HMD.RTP(rtpn[(3)]);       // 4/4 of intensity
  }
  
  if(rec ==0x13){
    HMD.RTP(0x00);            //turning off haptic motors
    Serial.println("STOP");
    digitalWrite (2,LOW);     //deactivate haptic drivers
    digitalWrite (3,LOW);
    digitalWrite (4,LOW);
  }
}
