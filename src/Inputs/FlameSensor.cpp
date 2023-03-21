#include "FlameSensor.h"

FlameSensor::FlameSensor(int pin, VoltageMeter *voltmeter)
{
  FlameSensor::pin       = pin;
  FlameSensor::voltMeter = voltmeter;
};

void FlameSensor::initialise(){
    //Measure the heater resistance initially, before the heater is started
    //In fact, just measure the battery voltage with the heater on and off.  Assuming the battery doesn't drain too much
    //in a fraction of a second, the difference will be proportional to the heater resistance.
    initV = this->voltMeter->getAverage();
    analogWrite(pin, 255);
    delay(1); //Don't let it heat up too much
    secondV = this->voltMeter->getAverage();
    analogWrite(pin, 0);
    flameDiff = initV - secondV; //When the heater is hot, the resistance increases so the Diff should reduce 
};

bool FlameSensor::mesure(){
    initV = this->voltMeter->getAverage();
    analogWrite(pin, 255);
    delay(1); //Don't let it heat up too much
    secondV = this->voltMeter->getAverage();
    analogWrite(pin, 0);

    change = flameDiff / (initV - secondV)*100;
    // Serial.print("Flame_Sensor = "); Serial.println(change);

    if(change>flameThreshold) {
      return true;
    } else {
      return false;
    }
};
