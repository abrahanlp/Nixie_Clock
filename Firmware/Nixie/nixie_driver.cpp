#include <Arduino.h>
#include <Wire.h>
#include "nixie_driver.h"

const byte nixi_table[4][10] = {{0x4, 0x3, 0x2, 0x0, 0x1, 0x9, 0x8, 0xC, 0xD, 0x5},
                                {0x2, 0xC, 0x4, 0x0, 0x8, 0x9, 0x1, 0x3, 0xB, 0xA},
                                {0x4, 0x3, 0x2, 0x0, 0x1, 0x9, 0x8, 0xC, 0xD, 0x5},
                                {0x2, 0xC, 0x4, 0x0, 0x8, 0x9, 0x1, 0x3, 0xB, 0xA}};
                               // 0    1    2    3    4    5    6    7    8    9

const byte NIXIE_TUBE_OFF = 0x06;

NixieDriver::NixieDriver(){
  _port0_add = PORT_0_ADD;
  _port1_add = PORT_1_ADD;

  for(byte i = 0; i < sizeof(_nixie_val); i++){
    _nixie_val[i] = NIXIE_TUBE_OFF;
  }
}

void NixieDriver::begin(void){
  digitalWrite(HV_CNTRL_PIN, LOW);
  digitalWrite(PORT0_CLK_PIN, LOW);
  digitalWrite(PORT1_CLK_PIN, LOW);
  pinMode(HV_CNTRL_PIN, OUTPUT);
  pinMode(PORT0_CLK_PIN, OUTPUT);
  pinMode(PORT1_CLK_PIN, OUTPUT);
}

void NixieDriver::set_Nixie(uint8_t n, uint8_t value){
  int port = 0;
  byte port_clk = 0;
  byte val = 0;

  _nixie_val[n] = nixi_table[n][value];

  switch(n){
    case 0:
    case 1:
      val = _nixie_val[0];
      val |= (_nixie_val[1] << 4);
      port = _port0_add;
      port_clk = PORT0_CLK_PIN;
      break;
    case 2:
    case 3:
      val = _nixie_val[2];
      val |= (_nixie_val[3] << 4);
      port = _port1_add;
      port_clk = PORT1_CLK_PIN;
      break;
    default:
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
