#include <Arduino.h>
#include "crsf.h"

#define RXD2 9
#define TXD2 10
#define SBUS_BUFFER_SIZE 25
uint8_t _rcs_buf[25] {};
uint16_t _raw_rc_values[RC_INPUT_MAX_CHANNELS] {};
uint16_t _raw_rc_count{};

const int freq = 25000;
const int resolution = 8;

const int LeftThrottlePin = 1;
const int RightThrottlePin = 3;

const int ThrottleCH1 = 1;
const int ThrottleCH2 = 2;

void SetPWMValue(float percent, int pwmChannel)
{
  int duty = map(percent, 0, 100, 0, 255);
  //Serial.println(duty);
  delay(1);
  ledcWrite(pwmChannel, duty);
}


void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(460800);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(420000, SERIAL_8N1, RXD2, TXD2);

  
  ledcSetup(ThrottleCH1, freq, resolution);
  ledcSetup(ThrottleCH2, freq, resolution);

  ledcAttachPin(LeftThrottlePin, ThrottleCH1);
  ledcAttachPin(RightThrottlePin, ThrottleCH2);
}

void loop() { //Choose Serial1 or Serial2 as required
  // Serial.println("looping");
  while (Serial2.available()) {
    size_t numBytesRead = Serial2.readBytes(_rcs_buf, SBUS_BUFFER_SIZE);
    if(numBytesRead > 0)
    {
      crsf_parse(&_rcs_buf[0], SBUS_BUFFER_SIZE, &_raw_rc_values[0], &_raw_rc_count, RC_INPUT_MAX_CHANNELS );
      //Serial.print("Channel 1: ");
      //Serial.print(_raw_rc_values[0]);
      //Serial.print("\tChannel 2: ");
      //Serial.print(_raw_rc_values[1]);
      
      //Serial.print("\tChannel 3: ");
      //Serial.print(_raw_rc_values[2]);
      //Serial.print("\tChannel 4: ");
      //Serial.print(_raw_rc_values[3]);
      //Serial.print("\tChannel 5: ");
      //Serial.println(_raw_rc_values[4]);

      int CH1_Mapped = map(_raw_rc_values[0], 988, 2011, 0, 100);
      int CH2_Mapped = map(_raw_rc_values[1], 988, 2011, 0, 100);
      
      SetPWMValue(CH1_Mapped, ThrottleCH1);
      SetPWMValue(CH2_Mapped, ThrottleCH2);
      
    }
  }
}
