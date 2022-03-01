// Add libraries for nRF24L01 module
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
////////////Haptic Drivers
#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 
#include <Wire.h> //I2C library 

RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"M2T", "M2A", "M2V"};
byte payload = 0x00;

// Variables
byte payload = 0x00;
int level;
int motor = 0;
// Create instance of Haptic motor
SFE_HMD_DRV2605L HMD; //Create haptic motor driver object 

uint8_t rtpn[] = {0x99, 0xB3, 0xC6, 0xE6};    //levels of intensity for the motors

////////////////Multiplexor ////////////
// Retreieved from https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout/wiring-and-test
#define TCAADDR 0x70

void tcaselect(uint8_t i) { 
  if (i > 7) return; 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() 
{
  HMD.begin();
  HMD.Mode(5);                    // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0b00110110);    // 1: LRA Mode ; 011: 4x Braking factor; 01: Medium (default) loop gain ; 10 : 15x (default)
  HMD.Library(1);                 // 1-5 & 7 for ERM motors, 6 for LRA motors 

  Serial.begin(500000);
  Wire.begin();

// From RF24 library example: GettingStarted*****
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
// Initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("Radio hardware is not responding!!"));
    delay(200);
    while (1) {}    // hold in infinite loop
  }
  
// Set the PA Level low to try preventing power supply related problems
// because these examples are likely run with nodes in close proximity to
// each other.
radio.setPALevel(RF24_PA_LOW);          // RF24_PA_MAX is default.
// save on transmission time by setting the radio to only transmit the
// number of bytes we need to transmit a float
radio.setPayloadSize(sizeof(payload));
// set the RX address of the TX node into a RX pipe
radio.openReadingPipe(1, address[2]);   // pipe 3 "M2V" - Master 2 Vibration
radio.startListening();                 // put radio in RX mode
//****From RF24 library example: GettingStarted*****
}

void loop() {
  uint8_t pipe;
  if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
    radio.read(&payload, bytes);            // fetch payload from FIFO
    
     SerialEventWrite(payload);
  } // end if of received payload    
}

void SerialEventWrite(byte rec){
  tcaselect(motor); // select latest motor
  if(rec==0x09) {           // label/code 33032
    HMD.RTP(rtpn[lvl]);
    delay(3000);
  }
  if(rec==0x0A) {           // label/code 33033
    HMD.RTP(rtpn[lvl+1]);
    delay(3000);
  }
  if(rec==0x0B) {           // label/code 33034
    HMD.RTP(rtpn[lvl+2]);
    delay(3000);
  }  
  if(rec==0x0C){            // label/code 33035
    HMD.RTP(rtpn[lvl+3]);
    delay(3000);
  }  
  HMD.RTP(0); //turn off stimulation indefinetely
  if (motor < 3){ //increase by one the motor, change to other
    motor++;
  }
  else{ //if all three motors were used, restart from zero
    motor = 0;
  }
}

