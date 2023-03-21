#include "CoolantPump.h"

CoolantPump::CoolantPump(int pin){
    this->pin = pin;
  };

void CoolantPump::turnOn(){
  if (!isOn)
  {
    analogWrite(pin, 255);
  }
}

void CoolantPump::turnOff(){
  if (isOn){}
  {
    analogWrite(pin, 0);
  }
}
