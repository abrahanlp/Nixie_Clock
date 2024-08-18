#include <Arduino.h>
#include <Wire.h>
#include "nixie_driver.h"

NixieDriver::NixieDriver(){
  _port0_add = PORT_0_ADD;
  _port1_add = PORT_1_ADD;
}

void NixieDriver::begin(void){
  digitalWrite(HV_CNTRL_PIN, LOW);
  pinMode(HV_CNTRL_PIN, OUTPUT);
}

void NixieDriver::set_Nixie(uint8_t n, uint8_t value){
  
}

void NixieDriver::high_voltage_switch(uint8_t onOff){
  digitalWrite(HV_CNTRL_PIN, onOff);
}

uint8_t NixieDriver::get_high_voltage_status(void){
  return digitalRead(HV_CNTRL_PIN);
}
