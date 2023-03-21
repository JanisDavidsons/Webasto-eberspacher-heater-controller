#include "CombustionFan.h"

CombustionFan::CombustionFan(int pin, int targetSpeed)
{
  this->currentSpeed = targetSpeed;
  this->pin          = pin;
}

void CombustionFan::tick()
{
  if (this->currentSpeed < targetSpeed && incrementRatio != 0)
  {
    if ((step%incrementRatio) == 0)
    {
      this->currentSpeed++;
    }
    step++;
  }else{
    this->currentSpeed = targetSpeed;
  }

  int mapped = HelperFunctions::mapf(this->currentSpeed, 0, 100, 0, 255); 

  analogWrite(pin, mapped);
}

void CombustionFan::run(int speed, int incrementRatio) {
  this->targetSpeed    = speed;
  this->incrementRatio = incrementRatio;
}

void CombustionFan::turnOff(){
  this->targetSpeed = 0;
}
