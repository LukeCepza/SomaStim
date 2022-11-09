// Add libraries for nRF24L01 module
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

////////////Haptic Drivers
#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 
#include <Wire.h> //I2C library 
 
//instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"M2T", "M2A", "M2V"};
// bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
byte payload = 0x00;  

// Variables
// byte payload = 0x00;
int lvl   =0;
int motor =5;
// Create instance of Haptic motor
SFE_HMD_DRV2605L HMD; //Create haptic motor driver object 

uint8_t rtpn[4] = {63,127,191,255};    //levels of intensity for the motors
int ln          = 4;
byte incr       = 0x05;     //intensidad incrementa de 5 and 5
  //intensidad comienza en 0
byte max              = 0xFF;
int tshold,x, numths  = 0;
uint8_t thold         = 0x00;
bool ths              = false;

void setup() {
  HMD.begin();
  Serial.begin(500000);
  
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  HMD.begin();
  HMD.Mode(0x05);            // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0x36);     // 1: LRA Mode ; 011: 4x Braking factor; 01: Medium (default) loop gain ; 10 : 15x (default)
  HMD.Library(2);            // 1-5 & 7 for ERM motors, 6 for LRA motors
  delay(10);         

// From RF24 library example: GettingStarted***** 
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
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_max is default.
  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 2 bytes
  // set the TX address of the RX node into the TX pipe
  // radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
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
  if (ths && thold < max) {
    HMD.RTP(thold);
    thold = thold+incr;
    Serial.println(thold);
    delay(1000);  }
}

void SerialEventWrite(byte rec){
  // checks if label is the one for marking the threshold (0xF0 - es solo un ejemplo) o is no
  // the label of STOP (0xFF -ejemplo)
  
  if(rec==0x11){
    ths = true;
      //funciรณn abajo
    Serial.println("Thrs Ready");
    digitalWrite (5,HIGH);
  }
  if (ths== true){
    if (rec == 0x12){
      HMD.RTP(0x00);
      numths++;
      Serial.println("Thr FINAL");
      digitalWrite (5,LOW);
      x = x+thold;
      thold = 0;
      ths = false; 
     
      if (numths == 3){
        tshold = x/3; //promedio de umbral de las 3 pruebas
        Serial.println("Thr PROMEDIO");
        Serial.println(tshold);

        Serial.println("Se imprime _y_, que es el numero que representa la intensidad");
        Serial.println("_y_ debe ser el 1/4,1/2 y 3/4 del valor entre el intervalo de tshold y 0xFF: ");

        int pz=(0xFF-tshold)/(ln-1);
        Serial.println(pz);

        Serial.println(" ");

        rtpn[0]=tshold;
        for (int i=0;i<(ln-1);i++){
          rtpn[i+1]= rtpn[i] + pz;
          Serial.println(rtpn[i]);
        }
        Serial.println(" ");
        for (int j=0;j<ln;j++){
          Serial.println(rtpn[j]);
        }
      }
    }
  } else {
     if(rec==0x09) {           // label/code 33032
       Serial.println("label 33032");
       Serial.println(motor);
       Serial.println("Intensidad: ");
       Serial.println(rtpn[0]);
       HMD.RTP(rtpn[(lvl)]);
       digitalWrite (motor,HIGH);
      }
     if(rec==0x0A) {           // label/code 33033
       Serial.println("label 33033");
       Serial.println(motor);
       Serial.println("Intensidad: ");
       Serial.println(rtpn[1]);
       digitalWrite (motor,HIGH);       
       HMD.RTP(rtpn[(lvl+1)]);

      }
     if(rec==0x0B) {           // label/code 33034
       Serial.println("label 33034");
       Serial.println(motor);
       Serial.println("Intensidad: ");
       Serial.println(rtpn[2]);
       HMD.RTP(rtpn[(lvl+2)]);
       digitalWrite (motor,HIGH);
      } 
     if(rec==0x0C){            // label/code 33035
       Serial.println("label 33035");
       Serial.println(motor);
       Serial.println("Intensidad: ");
       Serial.println(rtpn[3]);
       HMD.RTP(rtpn[(lvl+3)]);
       digitalWrite (motor,HIGH);
      }
     if(rec ==0x13){
       HMD.RTP(0x00);
       Serial.println("STOP");
       digitalWrite (motor,LOW);

      if (motor < 7){ //increase by one the motor, change to other
        motor++;
        delay(10);
       }
       else { //if all three motors were used, restart from zero
         motor = 5;
       }
      }
     
  } // else
} // serialeventwrite
