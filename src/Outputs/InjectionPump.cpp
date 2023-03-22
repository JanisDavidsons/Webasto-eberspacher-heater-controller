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

void InjectionPump::initilize() {
  Timer1.initialize(pulseLength);
}

void InjectionPump::tick()
{
  if (this->currentFrequency < targetFrequency && incrementRatio != 0) // this allows to increment frequency slowly to target the value.
  {
    if ((step%incrementRatio) == 0)
    {
      this->currentFrequency += 0.1; // if frequency has to be incremented in steps, it is done by 0.1 Hz at each step
      Timer1.setPeriod(1000000.0 / this->currentFrequency);
      updateDutyCycle();
    }
    step++;
  } else if (this->currentFrequency != targetFrequency) // else instant frequency change is requested
  {
    this->currentFrequency = targetFrequency;
    Timer1.setPeriod(1000000.0 / this->currentFrequency);
    updateDutyCycle();
  }
}

void InjectionPump::setFrequency(float frequency, int incrementRatio) {
  if (frequency <= minFrequency || frequency > maxFrequency) {
    turnOff();
    return;
  }else{
    injectionOn = true;
  }
    this->targetFrequency = frequency;
    this->incrementRatio  = incrementRatio;
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
  injectionOn      = false;
  targetFrequency = 0;
  Timer1.stop();
  Timer1.pwm(pin, 0);
}


void InjectionPump::setPrimeRatio(double coolantTemp) {  //sets initial prime ratio
  if(coolantTemp <= primeLowTemp){                  
    targetFrequency = primeLowTempFuelrate;              //if coolant temperature is less than a certain value use primeLowTempFuelrate
  }

  if(coolantTemp >= primeHighTemp){                     
    targetFrequency = primeHighTempFuelrate;            //if coolant temperature is more than a certain value, use primeHighTempFuelrate
  }

  float steps     = primeHighTemp-primeLowTemp;
  float fuelSteps = (primeLowTempFuelrate-primeHighTempFuelrate)/steps;  
  targetFrequency  = primeLowTempFuelrate-((coolantTemp-primeLowTemp)*fuelSteps);
  setFrequency(targetFrequency);
}

void InjectionPump::setInitInjectionRatio(double tempInit)
{
  targetFrequency = tempInit < startFuelThreshold ? startFuelCold : startFuelWarm;  //initial ratio depends on whether coolant temperature is below threshold, in such a case- it would use cold start fuel or warm start fuel.
  setFrequency(targetFrequency);
}

float InjectionPump::getCurrentFrequency()
{
  return currentFrequency;
}