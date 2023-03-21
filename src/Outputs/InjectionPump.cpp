#include "InjectionPump.h"

InjectionPump::InjectionPump(int pin, int pumpSize)
{
  this->pin        = pin;
  this->pumpSize   = pumpSize;
  injectionOn      = false;
  currentFrequency = 0;
  
  switch(pumpSize){
    case 22:
        // pulse_length = 9;
        pulseLength = 40000; // microseconds (40 miliseconds)
        break;
    case 33:
        // pulse_length = 18;
        break;
    case 60:
        // pulse_length = 36;
        break;
    default:  
        // pulse_length = 0;
        pulseLength = 0;
  }
}

void InjectionPump::begin() {
  Timer1.initialize(pulseLength);
}

void InjectionPump::setFrequency(float frequency) {
  if (frequency <= minFrequency || frequency > maxFrequency) {
    turnOff();
    return;
  }else{
    injectionOn = true;
  }

  currentFrequency = frequency;
  Timer1.setPeriod(1000000.0 / frequency);
  updateDutyCycle();
}

void InjectionPump::updateDutyCycle() {
  long dutyCycle = 0; 
  if (currentFrequency > 0) {
    dutyCycle = this->pulseLength / (1000.0 / (float)currentFrequency); // calculate duty cycle to maintain pulse length of 40ms at all frequencies
  }
  Timer1.pwm(pin, dutyCycle);
}

void InjectionPump::turnOff() {
  if (!injectionOn) {
    return;
  }
  injectionOn    = false;
  injectionRatio = 0;
  Timer1.stop();
  Timer1.pwm(pin, 0);
}


void InjectionPump::setPrimeRatio(double coolantTemp) {  //sets initial prime ratio
  if(coolantTemp <= primeLowTemp){                  
    injectionRatio = primeLowTempFuelrate;              //if coolant temperature is less than a certain value use primeLowTempFuelrate
  }

  if(coolantTemp >= primeHighTemp){                     
    injectionRatio = primeHighTempFuelrate;            //if coolant temperature is more than a certain value, use primeHighTempFuelrate
  }

  float steps     = primeHighTemp-primeLowTemp;
  float fuelSteps = (primeLowTempFuelrate-primeHighTempFuelrate)/steps;  
  injectionRatio  = primeLowTempFuelrate-((coolantTemp-primeLowTemp)*fuelSteps);
  setFrequency(injectionRatio);
}

void InjectionPump::setInitInjectionRatio(double tempInit)
{
  injectionRatio = tempInit < startFuelThreshold ? startFuelCold : startFuelWarm;  //initial ratio depends on whether coolant temperature is below threshold, in such a case- it would use cold start fuel or warm start fuel.
  setFrequency(injectionRatio);
}