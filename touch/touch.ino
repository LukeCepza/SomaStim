// This device is a TX node aka the master
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
//instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = { "M2T", "M2A", "M2V" };

int c, newtons;
byte payload = 0x00;

//PID controller constants
double kp = -0.5;
double ki = -0.000010;
double kd = -0.0004;
//PID controller with library
#include <PIDController.hpp>
PID::PIDParameters<double> parameters(kp,ki,kd); // (kp,ki,kd)
PID::PIDController<double> pidController(parameters);


unsigned long currentTime, previousTime;
double elapsedTime;
double error, lastError, cumError, rateError;
double input, output;
double TimeZero, currtime;
double Yt, Ut, setPoint;

boolean newDataReady = 0;
int serialPrintInterval = 0;  //increase value to slow down serial print activity
double elTime;                //compute time elapsed from previous computation

//-----------------Servos-----------------// 
//https://github.com/zcshiner/Dynamixel_Serial/blob/master/Dynamixel%20Institution%20V2.pdf
//https://github.com/zcshiner/Dynamixel_Serial
#include <Dynamixel_Serial.h>       // Library needed to control Dynamixal servo
#define SERVO_01 0x01               // ID of which we will set Dynamixel too
#define SERVO_02 0x02               // ID of which we will set Dynamixel too
#define SERVO_ControlPin 0x02       // Control pin of buffer chip, NOTE: this does not matter becasue we are not using a half to full contorl buffer.
#define SERVO_SET_Baudrate 500000    // Baud rate speed which the Dynamixel will be set too (57600)
//--------------Strain Gauge--------------// 
//https://github.com/sparkfun/HX711-Load-Cell-Amplifier
//https://github.com/bogde/HX711
//https://github.com/olkal/HX711_ADC
#include <HX711_ADC.h>
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
bool cal1 = false;
bool cal2 = false;
boolean _resume = false;
unsigned long t = 0;

//****************************SET UP******************************//
void setup() {
    //---------------PID LIBRARY----------------//
    pidController.TurnOn();
    
    //------------------Radio ------------------//
      // initialize the transceiver on the SPI bus
    Serial.begin(500000);
    while (!Serial) {
        // some boards need to wait to ensure access to serial over USB
    }
    if (!radio.begin()) {
        Serial.println(F("radio hardware is not responding!!"));
        delay(200);
        while (1) {} // hold in infinite loop
    }
    radio.setPALevel(RF24_PA_LOW);            // RF24_PA_MAX is default.
    radio.setPayloadSize(sizeof(payload));    // float datatype occupies 2 bytes
    radio.openReadingPipe(1, address[0]);     // pipe 1/6 - "M2T" - Master to Touch 
    radio.startListening();                   // put radio in RX mode

    //-----------------Servos-----------------// 
    Dynamixel.begin(SERVO_SET_Baudrate);                                  // We now need to set Ardiuno to the new Baudrate speed
    Serial.println("CLEARDATA");                                          // column headers
    Serial.println("LABEL, RMotor, LMotor, Yt, Ut, Time, CurrTime, RelativeTime, Phase");   // column headers
    Serial.println("ResetTimer");                                         // column headers
    Serial.println("DATA, TIME, TIMER");
    Dynamixel.setDirectionPin(SERVO_ControlPin);  Serial.print(",");      // Optional. Set direction control pin
    Dynamixel.setMode(SERVO_01, WHEEL, 0, 0);     Serial.print(",");      // set mode to WHEEL
    Dynamixel.setMode(SERVO_02, WHEEL, 0, 0);                             // set mode to WHEEL

    TimeZero = millis();
    currtime = millis();

    //--------------Strain Gauge--------------// 
    LoadCell.begin();
    LoadCell.start(stabilizingtime, _tare);
    while (!LoadCell.update()) {}
    if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
      Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
      while (1);
    }
    else {
      LoadCell.setCalFactor(-461);      // user set calibration value (float), initial value 1.0 may be used for this sketch
      //Serial.println("Startup is complete");
    }
}//**************************END SETUP*******************************//

//**************************** LOOP *********************************//
void loop() {
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
        uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
        radio.read(&payload, bytes);            // fetch payload from FIFO +++++++++++++++

        SerialEventWrite(payload);
    }
} //**************************** END LOOP ***************************//


//******************** FUNCIONES ***************************
// determinar los newtons en base a lectura de entrada proveniente del master
int levels(byte ret) {
    switch (ret) {
    case 0x10:
        return 2;
        break;
    case 0x01:
        return 2;
        break;
    case 0x02:
        return 3;
        break;
    case 0x03:
        return 4;
        break;
    case 0x04:
        return 6;
        break;
    }
}

//programación completa del prototipo
void SerialEventWrite(byte rec) {
    if (rec == 0x10) {            // si rec tiene código para iniciar: START
        delayP(5000);
        //delay(25000);           // espera 25s
        delayP(5000);
        //delay(20000);           // espera 20s para colocar el brazo (Aqui notifica master a usuario)
        c = 0;
        distend(1000);
        newtons = levels(rec);  // levels toma etiqueta para obtener nivel de fuerza
        Prepare(newtons);
    }

    if (rec == 0x01 || rec == 0x02 || rec == 0x03) {
        newtons = levels(rec);    // levels toma etiqueta para obtener nivel de fuerza
        GETERP(newtons); //*********************************
        c++;
        distend(1000);
        delay(5000);
        delay(10000);           //Mover prototipo a otro lugar del brazo para siguiente estimulación

        if (c < 3) {
            Prepare(newtons); //******************************
        }
        else if (c >= 3 && rec != 0x04) {
            c = 0;
            //delay(85000);    // Descanso
            delay(10000);
            //delay(20000);    // espera 20s para colocar el brazo (Aqui notifica master a usuario)
            delay(5000);
            newtons = levels(rec + 0x01);     // levels toma etiqueta para obtener nivel de fuerza
            Prepare(newtons); //******************************
        }
        else {
            while (true) {
                Serial.println("ENDPROGRAM");
            }
        }
    }
}

void delayP(int msec) {
  unsigned long relativeTime = millis();                //Tiempo en el que comenzo el movimiento
  unsigned long currtime = millis();
  elTime = (double)(currtime - relativeTime);           //Tiempo que ha pasado desde que comenzo el movimiento
  while (msec > elTime) {
    Yt = ReadStrainGauge();
    Serial.print("DATA, TIME, TIMER,");
    Serial.print("N,N");
    Serial.println(", " + String(Yt) + ", " + String("N") + ", " + String(elTime / 10) + ", " + String(currtime / 10) + ", " + String(relativeTime / 10) + ", Reading");
    delay(10);
  }
}

// función zero tare y tensar
void Prepare(int newtons) {
    LoadCell.start(stabilizingtime, _tare);               // zero tare
    unsigned long relativeTime = millis();                //Tiempo en el que comenzo el movimiento
                                                            //TENSAR NIVEL CN - dura 9s
    while (elTime <= 9000) {
        currtime = millis();                                //Tiempo real
        elTime = (double)(currtime - relativeTime);         //Tiempo que ha pasado desde que comenzo el movimiento
        Yt = ReadStrainGauge();
        Ut = computePID1(Yt, newtons);
        //Ut = computePID2(Yt,newtons, elTime);               
        //if(Ut > 300){Ut = 300;}; if(Ut < 0){ Ut = 0;}       //#########################REVISAR
        Serial.println(", " + String(Yt) + ", " + String(Ut) + ", " + String(elTime / 10) + ", " + String(currtime / 10) + ", " + String(relativeTime / 10) + ", Preparing");
        Serial.print("DATA, TIME, TIMER");
        Tense(Ut);
        //*** motores siguen con movimiento de tensado
    }
    //Stop servo-motors
    Dynamixel.wheel(SERVO_01, LEFT, 0);   delay(10);
    Dynamixel.wheel(SERVO_02, RIGHT, 0);
}

// función zero tare y tensar
void GETERP(int newtons) {
    LoadCell.start(stabilizingtime, _tare);               // zero tare
    unsigned long relativeTime = millis();                //Tiempo en el que comenzo el movimiento
    
    //TENSAR NIVEL CN - dura 6
    while (elTime <= 6000) {
        currtime = millis();                                //Tiempo real
        elTime = (double)(currtime - relativeTime);         //Tiempo que ha pasado desde que comenzo el movimiento
        Yt = ReadStrainGauge();
        Ut = computePID1(Yt, newtons);
        //Ut = computePID(Yt, newtons, elTime);               
        //if (Ut > 300) { Ut = 300; }; if (Ut < 0) { Ut = 0; } //#########################REVISAR
        Serial.println(", " + String(Yt) + ", " + String(Ut) + ", " + String(elTime / 10) + ", " + String(currtime / 10) + ", " + String(relativeTime / 10) + ", Stimulus");
        Serial.print("DATA, TIME, TIMER,");
        if (elTime <= 3000) {
            Dynamixel.wheel(SERVO_01, RIGHT, 300 - Ut);    Serial.print(",");   delay(10);
            Dynamixel.wheel(SERVO_02, RIGHT, 300 + Ut);    
        }
        if (elTime > 3000 & elTime < 6000) {
            Dynamixel.wheel(SERVO_01, LEFT, 300 + Ut);     Serial.print(",");   delay(10);
            Dynamixel.wheel(SERVO_02, LEFT, 300 - Ut);     
        }
    }
}

//lectura del peso del brazo/pierna
double ReadStrainGauge() {
    while (!newDataReady) {
        if (LoadCell.update()) newDataReady = true;
    }
    if (newDataReady) {
        if (millis() > t + serialPrintInterval) {
            double Yt = LoadCell.getData();
            newDataReady = 0;
            t = millis();
            return Yt;
        }
    }
}

//Aumentar la tension de forma estatica.
void Tense(double Ut) {
    if (Ut > 0) {
        Dynamixel.wheel(SERVO_01, LEFT, Ut);      Serial.print(",");    delay(10);
        Dynamixel.wheel(SERVO_02, RIGHT, Ut);
    }
    if (Ut < 0) {
        Dynamixel.wheel(SERVO_01, RIGHT, -Ut);    Serial.print(",");    delay(10);
        Dynamixel.wheel(SERVO_02, LEFT, -Ut);
    }
}

void distend(int lenght) {
    Dynamixel.wheel(SERVO_01, RIGHT, 200);
    Dynamixel.wheel(SERVO_02, LEFT, 200);
    delay(lenght);
    Dynamixel.wheel(SERVO_01, LEFT, 0);
    Dynamixel.wheel(SERVO_02, RIGHT, 0);
    delay(10);

    Serial.println(", "+String(Yt)+", "+String(Ut)+", "+String(elTime/10)+", "+String(currtime/10) + ", Distend"); 
    Serial.print("DATA, TIME, TIMER,");
}

//cálculo de PID con librería
double computePID1(double inp, int newt) {
    pidController.Setpoint = newt * 101.9716213;

    pidController.Input = inp;
    pidController.Update();

    output = pidController.Output;
    return output;
}

//Cálculo de PID
double computePID2(double inp, int newt, double elTime) {
    setPoint = newt * 101.9716213;
    currentTime = elTime;                                     //get current time
    elapsedTime = (double)(currentTime - previousTime);       //compute time elapsed from previous computation

    error = setPoint - inp;                                   // determine error
    cumError += error * elapsedTime;                          // compute integral
    rateError = (error - lastError) / elapsedTime;              // compute derivative

    double output = kp * error + ki * cumError + kd * rateError;   //PID output               

    lastError = error;                                        //remember current error
    previousTime = currentTime;                               //remember current time

    return output;                                            //have function return the PID output
}
//******* END LOOP ****************************
