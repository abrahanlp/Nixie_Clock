#include <Arduino.h>
#include <Wire.h>
#include "nixie_driver.h"

const byte nixi_tens_table[11]  = {0x2, 0xC, 0x4, 0x0, 0x8, 0x9, 0x1, 0x3, 0xB, 0xA, 0x0F};
const byte nixi_units_table[11] = {0x4, 0x3, 0x2, 0x0, 0x1, 0x9, 0x8, 0xC, 0xD, 0x5, 0x0F};
                                 // 0    1    2    3    4    5    6    7    8    9

const byte NIXIE_TUBE_OFF = 10;

NixieDriver::NixieDriver(){
  _nixie_driver.hours_address = PORT_0_ADD;
  _nixie_driver.hours_clk = PORT0_CLK_PIN;
  _nixie_driver.minutes_address = PORT_1_ADD;
  _nixie_driver.minutes_clk = PORT1_CLK_PIN;

  for(byte i = 0; i < sizeof(_nixie_driver.tube_value); i++){
    _nixie_driver.tube_value[i] = NIXIE_TUBE_OFF;
  }
}

void NixieDriver::begin(void){
  digitalWrite(HV_CNTRL_PIN, LOW);
  digitalWrite(PORT0_CLK_PIN, LOW);
  digitalWrite(PORT1_CLK_PIN, LOW);
  pinMode(HV_CNTRL_PIN, OUTPUT);
  pinMode(PORT0_CLK_PIN, OUTPUT);
  pinMode(PORT1_CLK_PIN, OUTPUT);

  for(byte i = 0; i < sizeof(_nixie_driver.tube_value); i++){
    set_Nixie(i, NIXIE_TUBE_OFF);
  }
}

void NixieDriver::set_Nixie(uint8_t n, uint8_t value){
  int port = 0;
  byte port_clk = 0;
  byte val = 0;

  Serial.print(n, HEX);
  Serial.print(" ");
  Serial.println(value, HEX);
  
  if(value > 9){
    value = 10; //NIXIE OFF
  }

  _nixie_driver.tube_value[n] = value;

  switch(n){
    case 0:
    case 1:
      val = nixi_tens_table[_nixie_driver.tube_value[0]];
      val |= nixi_units_table[_nixie_driver.tube_value[1]] << 4;
      port = _nixie_driver.hours_address;
      port_clk = _nixie_driver.hours_clk;
      break;
    case 2:
    case 3:
      val = nixi_tens_table[_nixie_driver.tube_value[2]];
      val |= nixi_units_table[_nixie_driver.tube_value[3]] << 4;
      port = _nixie_driver.minutes_address;
      port_clk = _nixie_driver.minutes_clk;
      break;
    default:
      Serial.println("Wrong tube");
      break;
  }

  Serial.println(val, HEX);
  Wire.beginTransmission(port);
  Wire.write(val);
  Wire.endTransmission();

  digitalWrite(port_clk, HIGH);
  delay(5);
  digitalWrite(port_clk, LOW);
}

void NixieDriver::high_voltage_switch(uint8_t onOff){
  digitalWrite(HV_CNTRL_PIN, onOff);
}

byte NixieDriver::get_high_voltage_status(void){
  return digitalRead(HV_CNTRL_PIN);
}
