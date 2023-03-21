#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H

#include "NTC_Thermistor.h"
#include <SmoothThermistor.h>
#include "Arduino.h"
#include <DHT.h>
#include <DHT_U.h>
#include "Common/Logger.h"

#define filterSamples   13              // filterSamples should  be an odd number, no smaller than 3
#define SMOOTHING_FACTOR 5 // NTC
#define DHTPIN 12           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11      // DHT 11

class Temperature
{
private:
  int exhaustTempPin;
  int coolantTempPin;
  float exhaustTempSec[11]; // array of last 10 sec exhaust temp, degres C
  int waterTempSec[181];
  int GWTLast_Sec;
  float Last_Exh_T = 0;
  float Last_Wat_T = 0;
  int Last_TSec;
  int burn_mode;
  // float Max_Change_Per_Sec = 2;  //Used to slow down changes in temperature to remove spikes
  // float rawDataWater, smoothDataCoolant;  // variables for sensor1 data
  // float rawDataExhaust, smoothDataExhaust;  // variables for sensor2 data
  // float CoolantSmoothArray [filterSamples];   // array for holding raw sensor values for sensor1 
  // float ExhaustSmoothArray [filterSamples];   // array for holding raw sensor values for sensor2 
  Thermistor *coolant = nullptr ;
  Thermistor *exhaust = nullptr;
  DHT_Unified *dht    = nullptr;
  sensor_t sensor;
  void setToomTemperature();
public:
  double exhaustTempInit;
  double coolantTemp; // degres C
  double exhaustTemp; // degres C
  float roomTemp;
  // String exhaustHistorical;
  Temperature(
    int exhaustTempPin, 
    int coolantTempPin,

    int burn_mode, 
    Thermistor *coolant, 
    Thermistor *exhaust
    );
  int read(int pin);
  void setTemperatures();
  void setInitExhaustTemp();
};

#endif