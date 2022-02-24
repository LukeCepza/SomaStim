// This device is a RX node aka the Caricias
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
//instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin
// Let these addresses be used for the pair
uint8_t address[][6] = {"M2T", "M2A", "M2V"};
// bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
byte payload = 0x00;  
int vent = 3;

void setup() {
  pinMode(vent, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(500000);
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
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 2 bytes
  // set the TX address of the RX node into the TX pipe
  // radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[1]); // using pipe 1}
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

// Modulación de velocidad de ventilador
void SerialEventWrite(byte rec){
  if(rec==0x01) {
   analogWrite (vent,63); // 255/4*1
   delay (3000);
   analogWrite (vent,0);
   }
  if(rec==0x02) {
   analogWrite (vent,128); // 255/4*2
   delay (3000);
   analogWrite (vent,0);
   }
  if(rec==0x03) {
   analogWrite (vent,191); // 255/4*3
   delay (3000);
   analogWrite (vent,0);
   }  
  if(rec==0x04){
   analogWrite (vent,254);
   delay (3000);
   analogWrite (vent,0);
   }  
}