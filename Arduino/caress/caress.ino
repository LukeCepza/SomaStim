// This device is a TX node aka the master
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
// instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"M2T", "M2A", "M2V"};
float ang_01_Io, ang_02_Io;
int ang_01, ang_02;
int c, newtons, angle;
byte payload = 0x00;
byte pos = 0;

unsigned long currentTime, previousTime, eltime;
double elapsedTime;
double input, output;
double TimeZero, currtime;
double Yt, Ut, setPoint;

boolean newDataReady = 0;
int serialPrintInterval = 0; // increase value to slow down serial print activity
double elTime;               // compute time elapsed from previous computation

//-----------------Servos-----------------//
// https://github.com/zcshiner/Dynamixel_Serial/blob/master/Dynamixel%20Institution%20V2.pdf
// https://github.com/zcshiner/Dynamixel_Serial
#include <Dynamixel_Serial.h>     // Library needed to control Dynamixal servo
#define SERVO_01 0x01             // ID of which we will set Dynamixel too
#define SERVO_02 0x02             // ID of which we will set Dynamixel too
#define SERVO_ControlPin 0x02     // Control pin of buffer chip, NOTE: this does not matter becasue we are not using a half to full contorl buffer.
#define SERVO_SET_Baudrate 500000 // Baud rate speed which the Dynamixel will be set too (57600)
#define CW_LIMIT_ANGLE 0x001      // lowest clockwise angle is 1, as when set to 0 it set servo to wheel mode
#define CCW_LIMIT_ANGLE 0x3FF     // Highest anit-clockwise angle is 0XFFF, as when set to 0 it set servo to wheel mode

//--------------Strain Gauge--------------//
// https://github.com/sparkfun/HX711-Load-Cell-Amplifier
// https://github.com/bogde/HX711
// https://github.com/olkal/HX711_ADC
#include <HX711_ADC.h>
const int HX711_dout = 4; // mcu > HX711 dout pin
const int HX711_sck = 5;  // mcu > HX711 sck pin
// HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
boolean _tare = true;        // set this to false if you don't want tare to be performed in the next step
bool cal1 = false;
bool cal2 = false;
boolean _resume = false;
unsigned long t = 0;

//****************************SET UP******************************//
void setup()
{
    //------------------Radio ------------------//
    // initialize the transceiver on the SPI bus
    Serial.begin(500000);
    while (!Serial)
    {
        // some boards need to wait to ensure access to serial over USB
    }
    if (!radio.begin())
    {
        Serial.println(F("radio hardware is not responding!!"));
        delay(200);
        while (1)
        {
        } // hold in infinite loop
    }
    radio.setPALevel(RF24_PA_LOW);         // RF24_PA_MAX is default.
    radio.setPayloadSize(sizeof(payload)); // float datatype occupies 2 bytes
    radio.openReadingPipe(1, address[0]);  // pipe 1/6 - "M2T" - Master to Touch
    radio.startListening();                // put radio in RX mode

    //-----------------Servos-----------------//
    Dynamixel.begin(SERVO_SET_Baudrate);                                                  // We now need to set Ardiuno to the new Baudrate speed
    Serial.println("CLEARDATA");                                                          // column headers
    Serial.println("LABEL, RMotor, LMotor, Yt, Ut, Time, CurrTime, RelativeTime, Phase"); // column headers
    Serial.println("ResetTimer");                                                         // column headers
    Serial.println("DATA, TIME, TIMER");
    Dynamixel.setDirectionPin(SERVO_ControlPin);
    Serial.print(","); // Optional. Set direction control pin
    Dynamixel.setMode(SERVO_02, SERVO, CW_LIMIT_ANGLE, CCW_LIMIT_ANGLE);
    Serial.print(",");                                                   // set mode to SERVO and set angle limits
    Dynamixel.setMode(SERVO_01, SERVO, CW_LIMIT_ANGLE, CCW_LIMIT_ANGLE); // set mode to SERVO and set angle limits
    //--------------Strain Gauge--------------//
    LoadCell.begin();
    LoadCell.start(stabilizingtime, _tare);
    while (!LoadCell.update())
    {
    }
    if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag())
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1)
            ;
    }
    else
    {
        LoadCell.setCalFactor(-461); // user set calibration value (float), initial value 1.0 may be used for this sketch
                                     // Serial.println("Startup is complete");
    }
    zeropos();
} //**************************END SETUP*******************************//

//**************************** LOOP *********************************//
void loop()
{
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
        uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
        radio.read(&payload, bytes);            // fetch payload from FIFO +++++++++++++++
        SerialEventWrite(payload);
    }

} //**************************** END LOOP ***************************//

// programación completa del prototipo
void SerialEventWrite(byte rec)
{
    if (rec == 0x01)
    {
        pos = stim(pos,40,1); 
    }
    else if (rec == 0x02)
    {
        pos = stim(pos,80,1);
    }
    else if (rec == 0x03)
    {
        pos = stim(pos,120,1);
    }
    else if (rec == 0x04)
    {
        pos = stim(pos,200,1);

    }
}
// Quitar el peso del brazo
void zerotare()
{
    LoadCell.start(stabilizingtime, _tare); // zero tare
    unsigned long relativeTime = millis();  // Tiempo en el que comenzo el movimiento
}
// lectura del peso del brazo/pierna
double ReadStrainGauge()
{
    while (!newDataReady)
    {
        if (LoadCell.update())
            newDataReady = true;
    }
    if (newDataReady)
    {
        if (millis() > t + serialPrintInterval)
        {
            double Yt = LoadCell.getData();
            newDataReady = 0;
            t = millis();
            return Yt;
        }
    }
}

bool stim(bool pos, int vel, int t){
    if (pos) {
        stim_left(vel, t);
    }
    else {
        stim_right(vel, t);
    }
    return !pos;
}

void stim_right(int vel, int t)
{
    ang_01_Io= ang_01_Io + vel * t * 2.27;
    ang_02_Io= ang_02_Io + vel * t * 2.27;
    Serial.println("");
    Serial.println(ang_01_Io);
    Serial.println("");
    Dynamixel.servo(SERVO_01, int(ang_01_Io), vel);
    Dynamixel.servo(SERVO_02, int(ang_01_Io), vel);
}

void stim_left(int vel, int t)
{
    ang_01_Io= ang_01_Io -  vel * t * 2.27;
    ang_02_Io= ang_02_Io -  vel * t * 2.27;
    Serial.println("");
    Serial.println(ang_01_Io);
    Serial.println("");
    Dynamixel.servo(SERVO_01, int(ang_01_Io), vel);
    Dynamixel.servo(SERVO_02, int(ang_01_Io), vel);
}

void zeropos()
{
    ang_02_Io = 511;
    ang_01_Io = 511;
    Dynamixel.servo(SERVO_01, ang_01_Io, 0x150);
    Dynamixel.servo(SERVO_02, ang_02_Io, 0x150);
}
