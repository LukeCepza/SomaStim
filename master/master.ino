// LCD
#include <Wire.h>
#include "DFRobot_LCD.h"
DFRobot_LCD lcd(16,2);  //16 characters and 2 lines of show

// This device is a TX node aka the master
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

//instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin
// Let these addresses be used for the pair

uint8_t address[][6] = {"M2T", "M2A", "M2V"};
//bool radioNumber = 2;
byte payload;  

void setup() {
  // LCD
  lcd.init();
  
  // put your setup code here, to run once:
  Serial.begin(500000);
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
    delay(200);
    while (1) {} // hold in infinite loop
  }
  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload)); // float datatype occupies 4 bytes
  // set the TX address of the RX node into the TX pipe
  //
  //radio.stopListening();
}


void loop() { 
  // put your main code here, to run repeatedly:
  delay(100);
  payload = SerialEvent();
  radio.stopListening();
  
  if (payload!=0){ 
    unsigned long start_timer = micros();                    // start the timer
    bool report = radio.write(&payload, sizeof(byte));       // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println( );                               // print payload sent
      delay(500);
      lcd.setRGB(0, 250, 0);
      lcd.setCursor(0, 1);
      lcd.print("Succesful       ");

    }else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      lcd.setRGB(250, 0, 0);
      lcd.setCursor(0, 1);
      lcd.print("Failed          ");
    }
  }
}

byte SerialEvent(){
  // put your main code here, to run repeatedly:
  byte ret = 0x00;
  String Fmar = "";
  while(Serial.available()){
    char markers = Serial.read();
    Fmar += markers;  
    }
  if(Fmar.indexOf("10001")>=0) {
    //agregar el valor del canal
    radio.openWritingPipe(address[0]);      
    ret = 0x10;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia - INICIO");
    }
  if(Fmar.indexOf("33024")>=0) {
    //agregar el valor del canal
    radio.openWritingPipe(address[0]);      
    ret = 0x01;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia 2N 33024");
    }
  else if(Fmar.indexOf("33025")>=0) {
    radio.openWritingPipe(address[0]);
    ret = 0x02;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia 3N 33025");
     }
  else if(Fmar.indexOf("33026")>=0) {
    radio.openWritingPipe(address[0]);
    ret = 0x03;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia 4N 33026");
    }
  else if(Fmar.indexOf("33027")>=0) {
    radio.openWritingPipe(address[0]);
    ret = 0x04;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia 6N 33027");
    }
  else if(Fmar.indexOf("33028")>=0) {
    radio.openWritingPipe(address[1]);
    ret = 0x05;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire  1/4  33028");
     }
  else if(Fmar.indexOf("33029")>=0) {
    radio.openWritingPipe(address[1]);
    ret = 0x06;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire  2/4  33029");
    }
  else if(Fmar.indexOf("33030")>=0) {
    radio.openWritingPipe(address[1]);
    ret = 0x07;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire  3/4  33030");
    }
  else if(Fmar.indexOf("33031")>=0) {
    radio.openWritingPipe(address[1]);
    ret = 0x08;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Aire Full  33031");
    }
  else if(Fmar.indexOf("33032")>=0) {
    radio.openWritingPipe(address[2]);
    ret = 0x09;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L1  33032");
    }
  else if(Fmar.indexOf("33033")>=0) {
    radio.openWritingPipe(address[2]);
    ret = 0x0A;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L2  33033");
    }     
  else if(Fmar.indexOf("33034")>=0) {
    radio.openWritingPipe(address[2]);
    ret = 0x0B;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L3  33034");
     }
  else if(Fmar.indexOf("33035")>=0) {
    radio.openWritingPipe(address[2]);
    ret = 0x0C;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Vibra  L4  33035");
    }
  else if(Fmar.indexOf("33036")>=0) {
    radio.openWritingPipe(address[0]);
    ret = 0x0D;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia TENSE 2N");
     }
  else if(Fmar.indexOf("33037")>=0) {
    radio.openWritingPipe(address[0]);
    ret = 0x0E;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia TENSE 3N");
     }
  else if(Fmar.indexOf("33038")>=0) {
    radio.openWritingPipe(address[0]);
    ret = 0x0F;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia TENSE 4N");
  }
  else if(Fmar.indexOf("33039")>=0) {
    radio.openWritingPipe(address[0]);
    ret = 0x10;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Caricia TENSE 6N");
     }

  if (ret!=0){
    digitalWrite(7,HIGH);
  }
  return ret;
}
