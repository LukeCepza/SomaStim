// This device is the Master node 

// RF module libraries
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
RF24 radio(9, 10);                                // Arduino pin 9 for the CE pin and pin 10 for the CSN pin
uint8_t address[][6] = {"M2T", "M2A", "M2V"};     // Addresses for the pair of modules (Master to Touch | Master to Air | Master to Vibration)
byte payload;

// LCD libraries
#include <Wire.h>
#include "DFRobot_LCD.h"
DFRobot_LCD lcd(16, 2);     //16 characters and 2 lines of show


void setup() {
  lcd.init();             // initialize LCD

  Serial.begin(500000);   // baudrate of 500000

  // --- From RF24 library example: GettingStarted --- //
  // configure RF
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    lcd.setRGB(250, 0, 0);
    lcd.setCursor(0, 0);
    lcd.print("not responding! ");
    lcd.setCursor(0, 1);
    lcd.print("Radio hardware  ");
    delay(20);
    while (1) {} // hold in infinite loop
  }
  radio.setPALevel(RF24_PA_LOW);          // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 4 bytes
}


void loop() {
  delay(5);
  payload = SerialEvent();      // call main function SerialEvent
  radio.stopListening();        // put radio in TX mode

  // checks if 0xFA label arrived from serial, 
  // if so, then connection between OpenViBE and Arduino was succesful.
  if (payload == 0xFA) {  
    payload = 0;
    Serial.print("Hi, its Arduino!");
    lcd.setRGB(0, 250, 0);
    lcd.setCursor(0, 1);
    lcd.print("   Successful   ");
  }

  if (payload != 0) {
    unsigned long start_timer = micros();               // start the timer
    bool report = radio.write(&payload, sizeof(byte));  // transmit & save the report
    unsigned long end_timer = micros();                 // end the timer

    if (report) {
      // print if reception of label in sub-modules was succesful
      Serial.print(F("Transmission successful! "));     // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);            // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println( );                                // print payload sent
      delay(5);
      lcd.setRGB(0, 250, 0);
      lcd.setCursor(0, 1);
      lcd.print("   Successful   ");

    } else {
      // print if reception of label in sub- modules failed
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      lcd.setRGB(250, 0, 0);
      lcd.setCursor(0, 1);
      lcd.print("     Failed     ");
    }
  }
}

byte SerialEvent() {
  // function SerialEvent received the code/label from serial port (OpenViBE) and
  // sends them via RadioFrequency communication to the corresponding sub-module

  // Read codes/labels from Serial port
  byte ret = 0x00;
  String Fmar = "";
  while (Serial.available()) {
    char markers = Serial.read();
    Serial.println(markers);
    Fmar += markers;
    Serial.println(Fmar);
  }
  
  if (Fmar.indexOf("33036") >= 0) {           //Caress protocol -TENSE
    radio.openWritingPipe(address[0]);
    ret = 0x0D;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS TENSE 2N ");
  }
  else if (Fmar.indexOf("33037") >= 0) {
    radio.openWritingPipe(address[0]);
    ret = 0x0E;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS TENSE 3N ");
  }
  else if (Fmar.indexOf("33038") >= 0) {
    radio.openWritingPipe(address[0]);
    ret = 0x0F;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS TENSE 4N ");
  }
  else if (Fmar.indexOf("33039") >= 0) {
    radio.openWritingPipe(address[0]);
    ret = 0x10;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS TENSE 6N ");
  }
  
  else if (Fmar.indexOf("33024") >= 0) {      //Caress protocol -STIMULI
    //agregar el valor del canal
    radio.openWritingPipe(address[0]);
    ret = 0x01;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS 2N 33024 ");
  }
  else if (Fmar.indexOf("33025") >= 0) {
    radio.openWritingPipe(address[0]);
    ret = 0x02;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS 3N 33025 ");
  }
  else if (Fmar.indexOf("33026") >= 0) {
    radio.openWritingPipe(address[0]);
    ret = 0x03;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS 4N 33026 ");
  }
  else if (Fmar.indexOf("33027") >= 0) {
    radio.openWritingPipe(address[0]);
    ret = 0x04;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("CARESS 6N 33027 ");
  }

  else if (Fmar.indexOf("33028") >= 0) {        // Air protocol
    radio.openWritingPipe(address[1]);
    ret = 0x05;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire  1/4  33028");
  }
  else if (Fmar.indexOf("33029") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0x06;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire  2/4  33029");
  }
  else if (Fmar.indexOf("33030") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0x07;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire  3/4  33030");
  }
  else if (Fmar.indexOf("33031") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0x08;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire Full  33031");
  }
  else if (Fmar.indexOf("33043") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0x16;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("STOP STIM 33043 ");//AIR
  }

  else if (Fmar.indexOf("33032") >= 0) {        //Vibration protocol
    radio.openWritingPipe(address[2]);
    ret = 0x09;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L1  33032");
  }
  else if (Fmar.indexOf("33033") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0x0A;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L2  33033");
  }
  else if (Fmar.indexOf("33034") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0x0B;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L3  33034");
  }
  else if (Fmar.indexOf("33035") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0x0C;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L4  33035");
  }
  
  else if (Fmar.indexOf("32770") >= 0) {        //FINISH EXPERIMENT
    radio.openWritingPipe(address[2]);
    ret = 0xFF;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print(" EXPERIMENT DONE ");
  }
  
  if (Fmar.indexOf("33083") >= 0) {             //Serial Connection successfull
    ret = 0xFA;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Ping Received");
  }
  return ret;
}
