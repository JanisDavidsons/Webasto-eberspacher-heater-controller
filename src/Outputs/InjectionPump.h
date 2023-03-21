#ifndef _INJECTION_PUMP_H
#define _INJECTION_PUMP_H

#include "Arduino.h"
#include <TimerOne.h>

class InjectionPump
{
private:
  int pin;
  int step;
  int pumpSize; 
  long pulseLength;
  bool injectionOn;
  int incrementRatio;
  float currentFrequency;
  float minFrequency = 0.3; // Hz
  float maxFrequency = 4.0; // Hz

  const double primeLowTempFuelrate  = 3.5;
  const double primeHighTempFuelrate = 2.0;
  float primeLowTemp                 = -10; //(was -10) Exhaust temp inaccurate at low temp. -10 is approx 10C
  float primeHighTemp                = 20;
  float startFuelThreshold           = -10; //Exhaust temperature, below which to use start_fuel_Cold
  float startFuelCold = 1.2;  //Winter Setting (use below 10C)
  float startFuelWarm = 1.0;  //Winter Setting (use below 10C)

public:
  double injectionRatio       = 0; // fuel_need
  double targetInjectionRatio = 0;
  bool fuelPurge = false;
  bool leanBurn  = false;

  InjectionPump(int pin, int pumpSize);

  void begin();
  void turnOn();
  void turnOff();
  void setFrequency(float frequency);
  void updateDutyCycle();
  void setPrimeRatio(double coolantTemp);
  void setInitInjectionRatio(double tempInit);
};

#endif
