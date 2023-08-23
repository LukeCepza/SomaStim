// This device is the Carees node

// RF module libraries
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
RF24 radio(9, 10);                                // Arduino pin 9 for the CE pin and pin 10 for the CSN pin
uint8_t address[][6] = {"M2T", "M2A", "M2V"};     // Addresses for the pair of modules (Master to Touch | Master to Air | Master to Vibration)
byte payload = 0x00;

// Servo-motors libraries
//Libraries used: https://github.com/zcshiner/Dynamixel_Serial/blob/master/Dynamixel%20Institution%20V2.pdf
#include <Dynamixel_Serial.h>     // Library needed to control Dynamixal servo
#define SERVO_01 0x01             // ID of which we will set Dynamixel too
#define SERVO_02 0x02             // ID of which we will set Dynamixel too
#define SERVO_ControlPin 0x02     // Control pin of buffer chip, NOTE: this does not matter becasue we are not using a half to full contorl buffer.
#define SERVO_SET_Baudrate 500000 // Baud rate speed which the Dynamixel will be set too (57600)
#define CW_LIMIT_ANGLE 0x001      // lowest clockwise angle is 1, as when set to 0 it set servo to wheel mode
#define CCW_LIMIT_ANGLE 0x3FF     // Highest anit-clockwise angle is 0XFFF, as when set to 0 it set servo to wheel mode

float ang_01_Io, ang_r, ang_l;
int ang_01, ang_02;
byte pos = 0;
boolean newDataReady = 0;
int serialPrintInterval = 0;    // increase value to slow down serial print activity
double elTime;                  // compute time elapsed from previous computation


void setup(){
  Serial.begin(SERVO_SET_Baudrate);   // baudrate equal to one in servo-motors (500000)

  // --- Initialize Servo-motores --- //
  Dynamixel.begin(SERVO_SET_Baudrate);  delay(1); // We now need to set Ardiuno to the new Baudrate speed
  Dynamixel.setDirectionPin(SERVO_ControlPin);  delay(1);
  Dynamixel.setMode(SERVO_01, SERVO, CW_LIMIT_ANGLE, CCW_LIMIT_ANGLE);  delay(1); // set mode to SERVO and set angle limits 
  Dynamixel.setMode(SERVO_02, SERVO, CW_LIMIT_ANGLE, CCW_LIMIT_ANGLE);  delay(1);
  zeropos();  // set motors to initial position
  
  // --- From RF24 library example: GettingStarted --- //
  // configure RF
  while (!Serial){
    // some boards need to wait to ensure access to serial over USB
  }
  // initialize the transceiver on the SPI bus
  if (!radio.begin()){
    Serial.println(F("radio hardware is not responding!!"));
    delay(200);
    while (1){} // hold in infinite loop
  }
  radio.setPALevel(RF24_PA_LOW);         // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 2 bytes
  radio.openReadingPipe(1, address[0]);  // pipe 1 "M2T" - Master to Touch
  radio.startListening();                // put radio in RX mode
}


void loop(){
  uint8_t pipe;
  if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
    radio.read(&payload, bytes);            // fetch payload from FIFO 
    SerialEventWrite(payload);
  }
}


void SerialEventWrite(byte rec){
  // function SerialEventWrite reads the label/code (or payload) received from Master module
  // and writes their corresponding motor velcity
    if (rec == 0x01){
      stim(60,1); 
    }
    else if (rec == 0x02){
      stim(90,1);
    }
    else if (rec == 0x03){
      stim(120,1);
    }
    else if (rec == 0x04){
      stim(180,1);
    }
}


void stim(int vel, int t){
  // function stim calculates the turning angle in 1 second for the servo-motor
  // according to the velocity of the stimuli
    ang_r = int(ang_01_Io + vel * t * 2.27);
    ang_l = int(ang_01_Io - vel * t * 2.27);
    if (ang_r > 1000){          // If the next angle is larger than possible,
      stim_angle(ang_l,vel);   
    } else {                      // Change direction.
      stim_angle(ang_r,vel);
    }
}


void stim_angle(int ang, int vel){
  // function stim_angle uses library function Dynamixel.servo to activate servo-motors with the corresponding
  // velocity for moving the calculated angle
    Dynamixel.servo(SERVO_01, ang, vel);    delay(1);   //move motors to desired angle at specific velocity
    Dynamixel.servo(SERVO_02, ang, vel);    delay(1);
    ang_01_Io = ang;
}


void zeropos(){
  // function zeropos initializes servo-motors in "home" position
    ang_01_Io = 1023;
    Dynamixel.servo(SERVO_01, ang_01_Io, 0x150);  delay(1);   //move motors to initial position
    Dynamixel.servo(SERVO_02, ang_01_Io, 0x150);  delay(1);
}
