//  -------------- Descomentar cuando se pruebe con RF --------------
// Add libraries for nRF24L01 module
//#include <SPI.h>
//#include "printf.h"
//#include "RF24.h"
//
//RF24 radio(9, 10);    // hardware conexion between RF module and Arduino pins
//                      // using pin 9 for the CE pin, and pin 10 for the CSN pin
//
//// Let these addresses be used for the pair
//// "address[]" -- > channel in which message is sent
//// for this module, "address[1] = M2A (Master to Air)" is used
//uint8_t address[][6] = {"M2T", "M2A", "M2V"};
//  -------------- Descomentar cuando se pruebe con RF --------------

////////////Haptic Driver
#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 
#include <Wire.h> //I2C library 


// Variables
byte payload = 0x00;
int level;
byte motor;

SFE_HMD_DRV2605L HMD; //Create haptic motor driver object 

//int motor = 2;
//uint8_t rtp_index = 0;
uint8_t rtpn[] = {0x99, 0xB3, 0xC6, 0xE6};    //levels of intensity for the motors


////////////////Multiplexor
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
  Serial.begin(500000);
  HMD.Mode(5);                    // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0b00110110);    // 1: LRA Mode ; 011: 4x Braking factor; 01: Medium (default) loop gain ; 10 : 15x (default)
  HMD.Library(1);                 // 1-5 & 7 for ERM motors, 6 for LRA motors 
  Wire.begin();

//  -------------- Descomentar cuando se pruebe con RF --------------
//  //-------------------Radio------------------
//  //****From RF24 library example: GettingStarted*****
//  while (!Serial) {
//    // some boards need to wait to ensure access to serial over USB
//  }
//  
//  // initialize the transceiver on the SPI bus
//  if (!radio.begin()) {
//    Serial.println(F("Radio hardware is not responding!!"));
//    delay(200);
//    while (1) {}    // hold in infinite loop
//  }
//  
//  // Set the PA Level low to try preventing power supply related problems
//  // because these examples are likely run with nodes in close proximity to
//  // each other.
//  radio.setPALevel(RF24_PA_LOW);          // RF24_PA_MAX is default.
//  // save on transmission time by setting the radio to only transmit the
//  // number of bytes we need to transmit a float
//  radio.setPayloadSize(sizeof(payload));
//  // set the RX address of the TX node into a RX pipe
//  radio.openReadingPipe(1, address[2]);   // pipe 3 "M2V" - Master 2 Vibration
//  radio.startListening();                 // put radio in RX mode
//  //****From RF24 library example: GettingStarted*****
//  -------------- Descomentar cuando se pruebe con RF --------------
}

void loop() {
//  --------------- Descomentar cuando se pruebe con RF --------------
//  uint8_t pipe;
//  if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
//    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
//    radio.read(&payload, bytes);            // fetch payload from FIFO
//    
//     SerialEventWrite(payload);
//  } // end if of received payload
//  --------------- Descomentar cuando se pruebe con RF --------------
  
  payload = SerialEvent();    // escribe y lee el mensaje en el SerialMonitor
  
  SerialEventWrite(payload);  // recibe la etiqueta recibida
}


// ------ Función para escribir la etiqueta en el serial monitor --------
byte SerialEvent(){
  byte ret = 0x00;                // variable set for the hex number
  String Fmar = "";               // variable type string to keep the message read from the serial
  while(Serial.available()){      // while there's a message in the serial... 
    char markers = Serial.read(); // read every character and write it on "markers"
    Fmar += markers;              // save the character read into "Fmar"
  }
    
  if(Fmar.indexOf("33032")>=0) {        // if label/code is on Fmar, procede
    //radio.openWritingPipe(address[0]);  // configure RF to communicate to the correspond channel for the label/code
    ret = 0x09;
  }
  else if(Fmar.indexOf("33033")>=0) {
    //radio.openWritingPipe(address[0]);  // RF
    ret = 0x0A;
  }
  else if(Fmar.indexOf("33034")>=0) {
    //radio.openWritingPipe(address[0]);  // RF
    ret = 0x0B;
  }
  else if(Fmar.indexOf("33035")>=0) {
    //radio.openWritingPipe(address[0]);  // RF
    ret = 0x0C;
  }
  return ret;
}
// ------ Función para escribir la etiqueta en el serial monitor --------


void SerialEventWrite(byte pL){
  int lvl=intensity(pL);
  motor=0x00;
    while (motor<0x03){
      Serial.print("Motor: ");
      Serial.println(motor);
      tcaselect(motor);
      Serial.print("Se activa con valor: ");
      Serial.println(rtpn[lvl]);
      HMD.RTP(rtpn[lvl]);
      delay(3000);
      Serial.println("Se apaga");
      HMD.RTP(0x00);
      delay(5000);
      //delay(25000);
      motor++;
      delay(2000);
    }
}


int intensity(byte code){
  switch (code){
      case 0x09:
        Serial.println("INTENSIDAD 1 - 0.15");
        level=0;
        return level;
        break;               
      case 0x0A:
        Serial.println("INTENSIDAD 2 - 0.3");
        level=1;
        return level;
        break;
      case 0x0B:
        Serial.println("INTENSIDAD 3 - 0.55");
        level=2;
        return level;
        break;
      case 0x0C:
        Serial.println("INTENSIDAD 4 - 0.7");
        level=3;
        return level;
        break;
     }  // end switch
}
