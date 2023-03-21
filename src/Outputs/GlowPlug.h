#ifndef _GLOW_PLUG_H
#define _GLOW_PLUG_H

#include "../Common/HelperFunctions.h"
#include "Arduino.h"

class GlowPlug
{
private:
  int pin;
  unsigned int step  = 0;
  int incrementRatio = 0;
  int currentValue   = 0;
  int targetValue    = 0;
  int glowTime       = 0; // seconds
  int outputValue    = 0;
public:

  GlowPlug(int pin);
  void tick();
  void run(int targetValue, int seconds = 0, int incrementRatio = 0);
  void turnOff();
  bool isOn();
};

#endif