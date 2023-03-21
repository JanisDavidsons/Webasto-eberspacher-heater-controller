#ifndef _FLAME_SENSOR_H
#define _FLAME_SENSOR_H

#include "Inputs/VoltageMeter.h"

class FlameSensor
{
private:
  int pin;
  VoltageMeter *voltMeter = nullptr;
  float flameDiff = 0; 
  float flameThreshold = 1.000;
  float initV = 0;
  float secondV = 0;
  float change = 0;
public:
  FlameSensor(int pin, VoltageMeter *voltmeter);
  void initialise();
  bool mesure();
};

#endif