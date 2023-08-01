// LCD
#include <Wire.h>
#include "DFRobot_LCD.h"
DFRobot_LCD lcd(16, 2); //16 characters and 2 lines of show

// This device is a TX node aka the master
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

//instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin
// Let these addresses be used for the pair
uint8_t address[][6] = {"M2T", "M2A", "M2V"};
byte payload;

char sos[26][5] = {
"33024",//"Caress 2N 24",
"33025",//"Caress 3N 25",
"33026",//"Caress 4N 26",
"33027",//"Caress 6N 27",
"33036",//"Caress T. 2N 36",
"33037",//"Caress T. 3N 37",
"33038",//"Caress T. 4N 38",
"33039",//"Caress T. 6N 39",
"33028",//"Air 1/4 28",
"33029",//"Air 2/4 29",
"33030",//"Air 3/4 30",
"33031",//"Air Full 31",
"33032",//"Vib L1 33032",
"33033",//"Vib L2 33033",
"33034",//"Vib L3 33034",
"33035",//"Vib L4 33035",
"33064",//"S. Thres. Vib",
"33061",//"Thres. rec. Vib",
"33042",//"STOP VIB (REG.)",
"33041",//"VAL. FLAG VIB",
"33065",//"START THRES. AIR",
"33062",//"Thres. rec. Air ",
"33043",//"STOP AIR (REG",
"33044",//"VAL. FLAG AIR",
"32770"//"Finished"
};

const byte ret[25] = {
0x01,
0x02,
0x03,
0x04,
0x0D,
0x0E,
0x0F,
0x10,
0x05,
0x06,
0x07,
0x08,
0x09,
0x0A,
0x0B,
0x0C,
0x11,
0x12,
0x13,
0x18,
0x14,
0x15,
0x16,
0x17,
0xFF};
//
//const int addresses[25] = {0,
//0,
//0,
//0,
//0,
//0,
//0,
//0,
//1,
//1,
//1,
//1,
//2,
//2,
//2,
//2,
//2,
//2,
//2,
//2,
//1,
//1,
//1,
//1,
//0};

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
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  payload = SerialEvent();
  radio.stopListening();
  if (payload == 0xFA) {
    payload = 0;
    Serial.print("Hi, its Arduino!");
    lcd.setRGB(0, 250, 0);
    lcd.setCursor(0, 1);
    lcd.print("   Successful   ");
  }

  if (payload != 0) {
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
      lcd.print("   Successful   ");

    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      lcd.setRGB(250, 0, 0);
      lcd.setCursor(0, 1);
      lcd.print("     Failed     ");
    }
  }
}

byte SerialEvent() {
  // put your main code here, to run repeatedly:
  byte ret = 0x00;
  String Fmar = "";
  while (Serial.available()) {
    char markers = Serial.read();
    Serial.println(markers);
    Fmar += markers;
    Serial.println(Fmar);

  }
  //Caress protocol -TENSE
  if (Fmar.indexOf("33036") >= 0) {
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
  //Caress protocol -STIMULI
  else if (Fmar.indexOf("33024") >= 0) {
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

  // Air protocol
  else if (Fmar.indexOf("33028") >= 0) {
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
  else if (Fmar.indexOf("33065") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0x14;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("START THRESHOLD "); //AIR
  }
  else if (Fmar.indexOf("33062") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0x15;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print(" THRESHOLD SENT ");//AIR
  }
  else if (Fmar.indexOf("33043") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0x16;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("STOP STIMUL 33043");//AIR
  }
  else if (Fmar.indexOf("33044") >= 0) {
    radio.openWritingPipe(address[1]);
    ret = 0X17;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("VALIDATION 33044");  //AIR
  }

  //vibration protocol
  else if (Fmar.indexOf("33032") >= 0) {
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
  else if (Fmar.indexOf("33064") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0x11;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("START THRESHOLD ");
  }
  else if (Fmar.indexOf("33061") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0x12;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print(" THRESHOLD SENT ");
  }
  else if (Fmar.indexOf("33042") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0x13;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("STOP STIMUL 33042");
  }
  else if (Fmar.indexOf("33041") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0X18;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("VALIDATION 33041");  //VIB
  }
  //FINISH EXPERIMENT
  else if (Fmar.indexOf("32770") >= 0) {
    radio.openWritingPipe(address[2]);
    ret = 0xFF;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print(" EXPERIMENT DONE ");
  }
  //Serial Connection successfull
  if (Fmar.indexOf("33083") >= 0) {
    ret = 0xFA;
    lcd.setRGB(0, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("Ping Received");
  }
  return ret;
}
