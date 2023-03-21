#ifndef _COOLANT_PUMP_H
#define _COOLANT_PUMP_H

#include "Arduino.h"

class CoolantPump
{
private:
  int pin;
  bool isOn = false;
  int water_pump_speed; // percent
public:
  CoolantPump(int pin);
  void turnOn();
  void turnOff();
};

#endif