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

int incr = 5;     //intensidad incrementa de 5 and 5
//intensidad comienza en 0
byte max=0xFF;
int tshold,pz =0;
//int pz = 255;
int x, numths = 0;
uint8_t thold =0x00;
bool ths = false;
//int numths = 0;
int ln = 4;       // nÃºmero de partes a divir
uint8_t fz[4] = {63,127,191,255};    //levels of intensity for the motors


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
  pz=(0xFF-tshold)/(ln-1);

}

void loop() {
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&payload, bytes);            // fetch payload from FIFO
      SerialEventWrite(payload);
    }
    if (ths && thold < max) {
      analogWrite(fan,thold);
      thold = thold+incr;
      Serial.println(thold);
      delay(1000);  
    }
}

// function SerialEventWrite read the label/code (or payload) sent from Master module
// and writes their correspond level of intensity as an analogical output on pin 3
// intensities vary: full (255), 3/4, 1/2 and 1/4
void SerialEventWrite(byte rec){
  // checks if label is the one for marking the threshold (0xF0 - es solo un ejemplo) o is no
  // the label of STOP (0xFF -ejemplo)
  if(rec==0x14){
    ths = true;
      //funciÃ³n abajo
    Serial.println("Thrs Ready");
  }

  if (ths == true){
  //if ths is true, then the STOP label received will affect the threshold action
    if (rec == 0x15){
      analogWrite(fan,0);
      numths++;
      Serial.println("Thr FINAL");
      x = x+thold;
      thold = 0;
      ths = false;

      if (numths == 3){
        tshold = x/3; //promedio de umbral de las 3 pruebas
        Serial.println("Thr PROMEDIO");
        Serial.println(tshold);

        Serial.println("Se imprime _y_, que es el numero que representa la intensidad");
        Serial.println("_y_ debe ser el 1/4,1/2 y 3/4 del valor entre el intervalo de tshold y 0xFF: ");

        pz=(0xFF-tshold)/(ln-1);
        Serial.println(pz);
      }
    }
  }else{
  //else, then ths is false, so the STOP label received will affect the stimuli actions
    if(rec==0x05) {           // label/code 33024
     //analogWrite (fan, tshold);    // 1/4 of intensity
     analogWrite(fan,191);
     delayMicroseconds(1000);
     analogWrite(fan,fz[0]);
     Serial.println("Intensidad: ");
     //Serial.println(tshold);
     Serial.println(fz[0]);
     }
    if(rec==0x06) {           // label/code 33025
      //analogWrite (fan, tshold + pz);    // 2/4 of intensity
      analogWrite(fan,191);
      delayMicroseconds(1000);
      analogWrite (fan, fz[1]);    // 2/4 of intensity
      Serial.println("Intensidad: ");
      //Serial.println(tshold + pz);
      Serial.println(fz[1]);
     }
    if(rec==0x07) {           // label/code 33026
     //analogWrite (fan, tshold + pz*2);    // 3/4 of intensity
      analogWrite (fan, fz[2]);    // 2/4 of intensity
      Serial.println("Intensidad: ");
      //Serial.println(tshold + 2*pz);
      Serial.println(fz[2]);
     }  
    if(rec==0x08){            // label/code 33027
      //analogWrite (fan, tshold + 3*pz);   // full
      analogWrite (fan, fz[3]);    // 2/4 of intensity
      Serial.println("Intensidad: ");
      //Serial.println(tshold + 3*pz);
      Serial.println(fz[3]);
     }
    if (rec == 0x16){     // STOP STIMULI LABEL
      analogWrite (fan,0);
     }
  }
}
