#include "GlowPlug.h"
#include "Arduino.h"

GlowPlug::GlowPlug(int pin){
  this->pin = pin;
};

void GlowPlug::tick(){

    if (glowTime > 0)
    {
      if ((step%incrementRatio) == 0 && currentValue < targetValue)
      {
        this->currentValue +=2;
        outputValue = HelperFunctions::mapf(this->currentValue, 0, 100, 0, 255); 
        analogWrite(pin, outputValue);
      }
    
      if ((step%10) == 0) // every second
      {
        glowTime--;
      }
      step++;
    }else{
      step         = 0;
      glowTime     = 0;
      targetValue  = 0;
      currentValue = 0;
      analogWrite(pin, 0);
    }
}

void GlowPlug::run(int targetValue, int seconds, int incrementRatio) { // just turn the plug on if glow_time > 0, and decrement glow_time every second
  this->targetValue    = targetValue;
  this->glowTime       = seconds;
  this->incrementRatio = incrementRatio;
}

bool GlowPlug::isOn(){
  return this->glowTime != 0;
}

void GlowPlug::turnOff(){
  step         = 0;
  glowTime     = 0;
  targetValue  = 0;
  currentValue = 0;
}
