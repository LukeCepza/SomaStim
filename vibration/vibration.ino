////////////Haptic Driver
#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 
#include <Wire.h> //I2C library 

SFE_HMD_DRV2605L HMD; //Create haptic motor driver object 

uint8_t rtp_index = 0;
uint8_t rtpn[] = {
  0x05, 400, 0x00, 200, 
  0x40, 400, 0x00, 200, 
  0x05, 400, 0x00, 200,
  0x40, 400, 0x00, 200 
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
  HMD.MotorSelect(0b10110110); // 1: LRA Mode ; 011: 4x Braking factor; 01: Medium (default) loop gain ; 10 : 15x (default)
  HMD.Library(6); //1-5 & 7 for ERM motors, 6 for LRA motors 
  Wire.begin();
  tcaselect(0);

}
void loop() 
{
  ///***
  if (rtp_index < sizeof(rtpn)/sizeof(rtpn[0])) {
    HMD.RTP(rtpn[rtp_index]);
    rtp_index++;
    delay(rtpn[rtp_index]);
    rtp_index++;
  } else {
    HMD.RTP(0x00);
    delay(1000);
    rtp_index = 0;
  }
  }
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