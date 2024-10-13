#include <Arduino.h>
#include <Wire.h>
#include "nixie_rtc.h"

RTC::RTC(){
  _rtc_add = RTC_ADD;
}

void RTC::begin(void){
    
}

uint8_t RTC::refresh(void){
  Wire.beginTransmission(_rtc_add);
 
  // Seconds address register
  Wire.write(0x00);

  // Check DS1307 answer
  if (Wire.endTransmission() != 0){
    return 1;
  }

  // Reading 8 bytes
  Wire.requestFrom(_rtc_add, 8);

  //Serial.println(F("RTC debug"));
  uint8_t tmp = Wire.read();
  //Serial.println(tmp, HEX);
  _timestamp.second = bcd2bin(tmp);

  tmp = Wire.read();
  //Serial.println(tmp, HEX);
  _timestamp.minute_bcd = tmp;
  _timestamp.minute = bcd2bin(_timestamp.minute_bcd);

  tmp = Wire.read();
  //Serial.println(tmp, HEX);
  _timestamp.hour_bcd = tmp;
  _timestamp.hour = bcd2bin(_timestamp.hour_bcd);

  tmp = Wire.read();
  //Serial.println(tmp, HEX);
  _timestamp.week_day = tmp;

  tmp = Wire.read();
  //Serial.println(tmp, HEX);
  _timestamp.day = bcd2bin(tmp);

  tmp = Wire.read();
  //Serial.println(tmp, HEX);
  _timestamp.month = bcd2bin(tmp);

  tmp = Wire.read();
  //Serial.println(tmp, HEX);
  _timestamp.year = bcd2bin(tmp);

  tmp = Wire.read();
  //Serial.println(tmp, HEX);  //Control
  
  return 0;
}

void RTC::set_time(rtc_timestamp_t timestamp){
  // Transmission start to DS1307
  Wire.beginTransmission(_rtc_add);

  Wire.write(0x00); //Seconds register
  Wire.write(bin2bcd(timestamp.second & 0x7F)); // With & 0x7F clock start to work
  Wire.write(bin2bcd(timestamp.minute));
  Wire.write(bin2bcd(timestamp.hour));
  Wire.write(timestamp.week_day);
  Wire.write(bin2bcd(timestamp.day));
  Wire.write(bin2bcd(timestamp.month));
  Wire.write(bin2bcd(timestamp.year));
  Wire.write(0);

  Wire.endTransmission();
}

void RTC::set_SQW(uint8_t value){
  Wire.beginTransmission(_rtc_add);
  Wire.write(0x07); //Control register
  value &= 0x01;
  value <<= 8;
  Wire.write(value);
  Wire.endTransmission();
}

void RTC::turn_on_periodic_SQW(void){
  Wire.beginTransmission(_rtc_add);
  Wire.write(0x07); //Control register
  Wire.write(0x10);
  Wire.endTransmission();
}

uint8_t RTC::get_second(void){
  return _timestamp.second;
}

uint8_t RTC::get_minute(void){
  return _timestamp.minute;
}

uint8_t RTC::get_minute_bcd(void){
  return _timestamp.minute_bcd;
}

uint8_t RTC::get_hour(void){
  return _timestamp.hour;
}

uint8_t RTC::get_hour_bcd(void){
  return _timestamp.hour_bcd;
}

uint8_t RTC::get_week_day(void){
  return _timestamp.week_day;
}

uint8_t RTC::get_day(void){
  return _timestamp.day;
}

uint8_t RTC::get_month(void){
  return _timestamp.month;
}

uint8_t RTC::get_year(void){
  return _timestamp.year;
}

uint8_t bin2bcd(uint8_t bin){
  return (bin / 10 * 16) + (bin % 10);
}

uint8_t bcd2bin(uint8_t bcd){
  return (bcd / 16 * 10) + (bcd % 16);
}
