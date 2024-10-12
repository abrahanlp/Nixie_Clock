#include <avr/wdt.h>
#include <stdlib.h>
#include "nixie_rtc.h"
#include "nixie_driver.h"

RTC rtc;
NixieDriver nixies;
uint8_t rtc_flag = 0;
char rx_buffer[20] = {0};
uint8_t rx_buffer_index = 0;

void setup() {
  wdt_enable(WDTO_2S);

  Serial.begin(115200);
  Serial.println(F("Nixie Clock by FaultyProject"));
  rtc.begin();
  nixies.begin();

  if(rtc.refresh()){
    system_error(F("RTC setup"));
  }

  set_nixies_time();
  nixies.high_voltage_switch(1);

  set_periodic_int(1000);
}

void loop(){
  //Process RX data
  if(Serial.available()){
    char tmp = Serial.read();
    if(tmp == '\n'){
      rx_buffer[rx_buffer_index] = 0;
      rx_buffer_index = 0;
      Serial.println(rx_buffer);

      //Process prompt
      switch(rx_buffer[0]){
        case 'C':
        case 'c':
          if(set_clock(rx_buffer)){
            print_help();
          }
          break;

        case 'V':
        case 'v':
          print_time();
          print_hv_status();
          break;

        case 'S':
        case 's':
          if(set_high_voltage(rx_buffer[2])){
            print_help();
          }
          break;
        
        case 'N':
        case 'n':
          nixies.set_Nixie(rx_buffer[2] - 0x30, rx_buffer[4] - 0x30);
          break;

        default:
          //Invalid command
          print_help();
          break;
      }
    }else {
      rx_buffer[rx_buffer_index++] = tmp;
    }
  }

  // Perform periodic task
  if(rtc_flag){
    rtc_flag = 0;
    if (rtc.refresh()){
      system_error(F("RTC refresh loop"));
    }else{
      if(rtc.get_second() == 0){
        set_nixies_time();
      }

      wdt_reset();
    }
  }
}

/*
  System error
*/
void system_error(const __FlashStringHelper *description_error){
  Serial.print(F("Error->"));
  Serial.println(description_error);
  while(1){};
}

/*
  Set Nixie tubes with HH:mm
*/
void set_nixies_time(void){
  if(nixies.set_Nixie(3, rtc.get_minute_bcd() & 0x0F)){
    system_error(F("Nixie set dec min"));
  }
  if(nixies.set_Nixie(2, (rtc.get_minute_bcd() >> 4) & 0x0F)){
    system_error(F("Nixie set ud min"));
  }
  if(nixies.set_Nixie(1, rtc.get_hour_bcd() & 0x0F)){
    system_error(F("Nixie set dec hour"));
  }
  if(nixies.set_Nixie(0, (rtc.get_hour_bcd() >> 4) & 0x0F)){
    system_error(F("Nixie set ud hour"));
  }
}

/*
  Periodic interrupt service
*/
ISR(TIMER1_COMPA_vect){
  rtc_flag = 1;
}

/*
  Set high voltage
*/
uint8_t set_high_voltage(char hv_set){
  uint8_t error = 0;

  switch(hv_set){
    case '1':
      nixies.high_voltage_switch(1);
      break;
    case '0':
      nixies.high_voltage_switch(0);
      break;
    default:
      error = 1;
      break;
  }

  return error;
}

/*
  Set RTC date and time
*/
uint8_t set_clock(char *str_time){
  rtc_timestamp_t timestamp_tmp = {0};
  char str[5] = {0};
  uint8_t d = 0;
  uint8_t m = 0;
  uint8_t y = 0;

  if(str_time[1] != ' ' || str_time[8] != '-' || str_time[15] != 0){
    return 1;
  }

  str[0] = str_time[2];
  str[1] = str_time[3];
  timestamp_tmp.day = atoi(str);
  str[0] = str_time[4];
  str[1] = str_time[5];
  timestamp_tmp.month = atoi(str);
  str[0] = str_time[6];
  str[1] = str_time[7];
  timestamp_tmp.year = atoi(str);

  uint8_t weekday = (d+=m<3 ? y-- : y-2,23*m/9+d+4+y/4-y/100+y/400) % 7;

  str[0] = str_time[9];
  str[1] = str_time[10];
  timestamp_tmp.hour = atoi(str);
  str[0] = str_time[11];
  str[1] = str_time[12];
  timestamp_tmp.minute = atoi(str);
  str[0] = str_time[13];
  str[1] = str_time[14];
  timestamp_tmp.second = atoi(str);

  rtc.set_time(timestamp_tmp);

  return 0;
}

/*
  Print High Voltage status
*/
void print_hv_status(void){
  Serial.print("High Voltage ");
  if(nixies.get_high_voltage_status()){
    Serial.println("ON!!!");
  }else{
    Serial.println("off");
  }
}

/*
  Print date and time trace
*/
void print_time(void){
  char str_t[15] = {0};

  sprintf(str_t, "%02d/%02d/%02d(%02d)-", rtc.get_day(), rtc.get_month(), rtc.get_year(), rtc.get_week_day());
  Serial.print(str_t);
  sprintf(str_t, "%02d:%02d:%02d\r\n", rtc.get_hour(), rtc.get_minute(), rtc.get_second());
  Serial.print(str_t);
}

/*
  Print help
*/
void print_help(void){
  Serial.println("Nixie Clock by FaultyProject");
  Serial.println("C DDMMYY(ww)-HHmmSS => Set day(DD) month(MM) year(YY) hour(HH) minute(mm) second(SS)");
  Serial.println("V => Print date and time DDMMYY()-HHmmSS");
  Serial.println("S x => High Voltage Control, x=1: Turn on. x=0: Turn off");
  Serial.println("N n x => Set nixie tube n to x");
}

/*
  Set periodic interrupt
*/
void set_periodic_int(int interval_ms){
  // CTC mode (Clear Timer on Compare Match)
  TCCR1A = 0;
  TCCR1B = 0;

  // Timer1 prescaler -> 1024
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

  // Set interval
  OCR1A = (16000000 / 1024) * (interval_ms / 1000.0) - 1;

  // Enable channel A interrupt
  TIMSK1 |= (1 << OCIE1A);
}
