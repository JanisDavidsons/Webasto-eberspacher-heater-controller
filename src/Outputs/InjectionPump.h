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
  float targetFrequency;
  float currentFrequency;
  float minFrequency = 0.3; // Hz
  float maxFrequency = 4.0; // Hz

  const double primeLowTempFuelrate  = 3.5;
  const double primeHighTempFuelrate = 2.0;
  float primeLowTemp                 = -10; //(was -10) Exhaust temp inaccurate at low temp. -10 is approx 10C
  float primeHighTemp                = 20;
  float startFuelThreshold           = -10; //Exhaust temperature, below which to use start_fuel_Cold
  float startFuelCold                = 1.2;  //Winter Setting (use below 10C)
  float startFuelWarm                = 1.0;  //Winter Setting (use below 10C)

public:
  static constexpr float THROTTLING_HIGH_FUEL   = 1.8;
  static constexpr float THROTTLING_STEADY_FUEL = 1.3;
  static constexpr float THROTTLING_LOW_FUEL    = 0.83; 

  InjectionPump(int pin, int pumpSize);

  void tick();
  void initilize();
  void turnOn();
  void turnOff();
  void updateDutyCycle();
  float getCurrentFrequency();
  void setPrimeRatio(double coolantTemp);
  void setInitInjectionRatio(double tempInit);
  void setFrequency(float frequency, int incrementSpeed = 0);
};

#endif
