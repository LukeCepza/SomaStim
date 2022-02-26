////////////Haptic Driver
#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 
#include <Wire.h> //I2C library 

SFE_HMD_DRV2605L HMD; //Create haptic motor driver object 

int motor = 0;

uint8_t rtp_index = 0;
uint8_t rtpn[] = {
  0x05, 200, 0x00, 500, //5
  0x10, 200, 0x00, 500, //16
  0x15, 200, 0x00, 500, //21
  0x20, 200, 0x00, 500, //32
  0x25, 200, 0x00, 500, //37
  0x30, 200, 0x00, 500, //48
  0x35, 200, 0x00, 500, //53
  0x40, 200, 0x00, 500  //64
};



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
  HMD.Mode(5); // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0b00110110); // 1: LRA Mode ; 011: 4x Braking factor; 01: Medium (default) loop gain ; 10 : 15x (default)
  HMD.Library(1); //1-5 & 7 for ERM motors, 6 for LRA motors 
  Wire.begin();
//  tcaselect(0);
//  tcaselect(1);
//  tcaselect(2);
    tcaselect(motor);
}

void loop() 
{
  ///***
    if (rtp_index < sizeof(rtpn)/sizeof(rtpn[0])) {
//       for (motor=0;motor<=2;motor++) {
            HMD.RTP(rtpn[rtp_index]);
            Serial.println(rtpn[]);
            rtp_index++;
            delay(rtpn[rtp_index]);
            //Serial.println(rtpn[]);
            rtp_index++;
//            }
        } else if (motor>2) {
                  motor=0;
                  tcaselect(motor);
        } else {
               motor++;
               tcaselect(motor);
               HMD.RTP(0x00);
               delay(2000);
               rtp_index = 0;
      }
}










     
//    motor++;
//    tcaselect(motor);
//    } else {
//      motor>3
//      motor=0;
//    }
//   motor++;
//   tcaselect(motor);
//    (motor>3){
//      motor=0;
//  }
//    if (motor>2) {
//       motor=0;
//     }
// }
//***/

/***
  int seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
  for(int wave = 1; wave <=123; wave++) //There are 123 waveform effects 
  {
     HMD.Waveform(seq, wave);
     HMD.go();
     delay(600); //give enough time to play effect 
     Serial.print("Waveform Sequence:      ");
     Serial.println(seq);
     Serial.print("Effect No.:      ");
     Serial.println(wave);

    if (wave%8==0) //Each Waveform register can queue 8 effects
    {
        seq=seq+1;
    }
    if (wave%64==0) // After the last register is used start over 
    {
        seq=0;
    }
  }
 }
  ***/
