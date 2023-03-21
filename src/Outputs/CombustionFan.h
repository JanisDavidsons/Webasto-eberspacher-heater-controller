#ifndef _COMBUSTION_FAN_H
#define _COMBUSTION_FAN_H

#include "../Common/HelperFunctions.h"
#include "Arduino.h"

class CombustionFan
{
private:
  int targetSpeed    = 0;
  unsigned int step  = 0;
  int incrementRatio = 0;
  int pin;
public:
  static const int START_FAN_SPEED = 40;
  int currentSpeed   = 0;

  CombustionFan(int pin, int targetSpeed = 0);
  void tick();
  void run(int speed, int incrementSpeed = 0);
  void turnOff();
};

#endif