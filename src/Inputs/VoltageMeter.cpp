#include "VoltageMeter.h"
#include "Arduino.h"
#include "Common/Logger.h"

VoltageMeter::VoltageMeter(int pin)
{
  VoltageMeter::pin = pin;
};

void VoltageMeter::read() {
    int A0Value = analogRead(pin);
    float voltage_sensed = A0Value * (arduinoVCC / (float)inputResolution);     
    voltage = voltage_sensed * ( 1 + ( (float) ValueR2 /  (float) ValueR1) );
};

double VoltageMeter::getAverage(){  
    double voltage_temp_average=0;
    for(int i=0; i < average_of; i++)
    {
       read();
       voltage_temp_average +=voltage;
    }
      
  return voltage_temp_average / average_of;
};
