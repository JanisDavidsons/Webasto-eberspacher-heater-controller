#ifndef _VOLTAGE_METER_H
#define _VOLTAGE_METER_H

#include "Arduino.h"

//Pin 1 is used as the on/off switch.  However, it connects to an Analog pin via a 
//potential divider.  While the heater is on, this can be used to read battery voltage
//The purpose of this is to shut down if the voltage dips below a certain value - say 11V

class VoltageMeter
{
private:
const float arduinoVCC = 4.90;//Your Arduino voltage
unsigned long ValueR1 = 5070;
unsigned long ValueR2 = 27000; // 26800 by mltimeter
double Voltage_Source = 12;
const int alanogPin = A0;//the pin connecting the voltage. 
const int inputResolution =1023;//works with most Arduino boards
const float average_of = 100;//Average of 500 readings
float voltage;
  int pin;
public:
  VoltageMeter(int pin);
  void read();
  double getAverage();
};


#endif