// include libraries for RF communication
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
//instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"M2T", "M2A", "M2V"};
byte payload = 0x00;  
int fan = 3;

void setup() {
  pinMode(fan, OUTPUT);     // set pin 3 as output (anological output)
                            // pin 3 works as PWM in arudino NANO
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
  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);          // RF24_PA_MAX is default.
  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 2 bytes
  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[1]);   // pipe 2 "M2A" - Master to Air
  radio.startListening();                 // put radio in RX mode
  // --- From RF24 library example: GettingStarted --- //
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
// and writes their correspond level of intensity as an analogical output on pin 3
// intensities vary: full (255), 3/4 (191), 1/2 (128) and 1/4 (63)
void SerialEventWrite(byte rec){
  if(rec==0x05) {           // label/code 33024
   analogWrite (fan,63);    // 255/4*1
   delay (3000);            // fan actived for 3 seconds
   analogWrite (fan,0);     // turn the fan off
   }
  if(rec==0x06) {           // label/code 33025
   analogWrite (fan,128);   // 255/4*2
   delay (3000);
   analogWrite (fan,0);
   }
  if(rec==0x07) {           // label/code 33026
   analogWrite (fan,191);   // 255/4*3
   delay (3000);
   analogWrite (fan,0);
   }  
  if(rec==0x08){            // label/code 33027
   analogWrite (fan,255);   // full
   delay (3000);
   analogWrite (fan,0);
   }  
}
