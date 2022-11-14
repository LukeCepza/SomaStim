// This device is a TX node aka the master
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
// instance of radio
RF24 radio(9, 10); // using pin 9 for the CE pin, and pin 10 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"M2T", "M2A", "M2V"};
int ang_01_Io, ang_02_Io, ten, dA;
int ang_01, ang_02;
int c, newtons, angle;
byte payload = 0x00;

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
#define CW_LIMIT_ANGLE 0x000      // lowest clockwise angle is 1, as when set to 0 it set servo to wheel mode
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
    distend();
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

// Aumentar la tension de forma estatica.
int tense(double newtons, double ten)
{
    ang_02_Io = 1023;
    ang_01_Io = 0;
    Dynamixel.servo(SERVO_01, ang_01_Io, 0x060);
    Dynamixel.servo(SERVO_02, ang_02_Io, 0x060);
    delay(1000);
    ten = 0;
    int m = 20; // Proporcion tensante, es la velocidad a la que se tensa.
    Serial.println("t = " + String(eltime) + " YT = " + String(Yt) + " m = " + String(m) + " da = " + String(dA) + " A02 = " + String(ang_02_Io));
    eltime = 0;
    previousTime = millis();
    while (eltime < 7000)
    {
        eltime = millis() - previousTime;
        Yt = ReadStrainGauge();

        if (Yt < newtons - .02 * newtons)
        {
            ten = ten + m;
        }
        else if (Yt > newtons + .02 * newtons)
        {
            ten = ten - m;
            m = 1; // newtons/200;
        }
        if (ten > 1024 && ten <= 1536)
        { // THIRD ANGLE
            ang_01 = 1023;
            ang_02 = ang_02_Io - ten + 1023;
        }
        else if (ten > 512 && ten <= 1024)
        { // SECOND ANGLE
            ang_02 = 1023;
            ang_01 = ang_01_Io + ten;
        }
        else if (ten > 0 && ten <= 512)
        { // FIRST ANGLE
            ang_02 = 1023;
            ang_01 = ang_01_Io + ten;
        }
        Dynamixel.servo(SERVO_01, ang_01, 0x060);
        Dynamixel.servo(SERVO_02, ang_02, 0x060);
        Serial.println();
        // Tolerance error 2%
        dA = ten;
        delay(m * 5); // Cuando se esta tensando por primera vez, la duración entre cado paso de tensado es mayor que cuando ya se ha
        // sobrepasado el nivel de interes.
        Serial.print("t = " + String(eltime) + " YT = " + String(Yt) + " m = " + String(m) + " ten = " + String(ten) + " A02 = " + String(ang_02_Io));
    }
    return ten;
}

void stim_right(int newtons)
{   
    // newtons = 203.94;
    eltime = 0;
    previousTime = millis();
    int lag = 0;
    int cur_ang = 0;
    int mov = 5;
    int m = newtons / 200; //This constant is to adjust the tension if it is decaying
    ;
    while ((eltime < 6000) && ((ang_01 - cur_ang) > 0) && ((ang_02 - cur_ang) > 0))
    {
        cur_ang = cur_ang + mov;
        if (Yt < newtons - .02 * newtons)
        {
            lag = lag + m;
        }
        if (Yt > newtons + .02 * newtons)
        {
            lag = lag - m;
        }
        eltime = millis() - previousTime;
        Yt = ReadStrainGauge();
        Serial.print("t = " + String(eltime) + " YT = " + String(Yt) + " ten = " + String(ten) + " da = " + String(dA) + " A02 = " + String(ang_02_Io));
        Dynamixel.servo(SERVO_01, ang_01 - cur_ang +lag, 0x050);  Dynamixel.servo(SERVO_02, ang_02 - cur_ang, 0x050); Serial.println("");
        delay(40);
    }
}

void stim(int newtons)
{   
    // newtons = 203.94;
    eltime = 0;
    previousTime = millis();
    int wait = 4;
    int vel = 600;
    int lag1 = 0;
    int lag2 = 0;
    int cur_ang = 0;
    int mov = 20;
    int m = newtons / 100; //This constant is to adjust the tension if it is decaying
    bool right = true;
    while ((eltime < 7000))
    {
        eltime = millis() - previousTime;
        if (right){
          cur_ang = cur_ang + mov;
          if (((ang_01 - cur_ang + lag1) > 0) && ((ang_02 - cur_ang + lag2) > 0))
          {
              Yt = ReadStrainGauge();
              if (Yt < newtons - .02 * newtons)
                  {lag1 = lag1 + m;}
              else if (Yt > newtons + .02 * newtons)
                  {lag1 = lag1 - m;}
              Dynamixel.servo(SERVO_01, ang_01 - cur_ang +lag1, vel);  Dynamixel.servo(SERVO_02, ang_02 - cur_ang + lag2, vel); Serial.println("");
              Serial.print("t = " + String(eltime) + " YT = " + String(Yt) + " cur_ang ="+String(cur_ang)+" cur_ang_1 = " + String(ang_01 - cur_ang +lag1) + " cur_ang_2 = " + String(ang_02 - cur_ang + lag2));
              delay(wait);
          }
          else {
            cur_ang = cur_ang - mov;
            right = not right;
          }    
        }
        else if (not right){
          cur_ang = cur_ang - mov;
        if (((ang_01 - cur_ang + lag1) < 1024) && ((ang_02 - cur_ang + lag2) < 1024) && (not right)) {
            Yt = ReadStrainGauge();
            if (Yt < newtons - .02 * newtons)
                {lag2 = lag2 - m;}
            else if (Yt > newtons + .02 * newtons)
                {lag2 = lag2 + m;}
            Dynamixel.servo(SERVO_01, ang_01 - cur_ang +lag1, vel);  Dynamixel.servo(SERVO_02, ang_02 - cur_ang + lag2, vel);  Serial.println(""); 
            Serial.print("t = " + String(eltime) + " YT = " + String(Yt) + " cur_ang ="+String(cur_ang)+" cur_ang_1 = " + String(ang_01 - cur_ang +lag1) + " cur_ang_2 = " + String(ang_02 - cur_ang + lag2));
            delay(wait);
        }
        else {
            right = not right;
            cur_ang = cur_ang + mov;
        }
    }
}
}
// programación completa del prototipo
void SerialEventWrite(byte rec)
{
    // 200 gramos
    if (rec == 0x0D) //
    {
        zerotare();
        ten = 0;
        ten = tense(200, ten);
    }
    else if (rec == 0x01)
    {
        stim_right(200);
        distend();
    }
    // 300 gramos
    else if (rec == 0x0E)
    {
        zerotare();
        ten = 0x200;
        dA = tense(300, ten);
    }
    else if (rec == 0x02)
    {
        stim_right(300);
        distend();
    }
    // 400 gramos
    else if (rec == 0x0F)
    {
        zerotare();
        ten = 0x250;
        dA = tense(400, ten);
    }
    else if (rec == 0x03)
    {
        stim_right(400);
        distend();
    }
    // 600 gramos
    else if (rec == 0x10)
    {
        zerotare();
        ten = 0x300;
        dA = tense(600, ten);
    }
    else if (rec == 0x04)
    {
        stim(600);
        distend();
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
void distend()
{
    ang_02_Io = 1023;
    ang_01_Io = 0;
    Dynamixel.servo(SERVO_01, ang_01_Io, 0x300);
    Dynamixel.servo(SERVO_02, ang_02_Io, 0x300);
    delay(5000);
}
