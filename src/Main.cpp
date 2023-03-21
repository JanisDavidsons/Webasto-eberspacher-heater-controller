#include "Arduino.h"
#include <math.h> // needed to perform some calculations
#include "DieselHeater.h"
#include <time.h>
#include <arduino-timer.h>

#include <Thermistor.h>
#include <NTC_Thermistor.h>
#include <SmoothThermistor.h>
#include <Adafruit_Sensor.h>

#include "Common/Logger.h"

#include "Outputs/InjectionPump.h"
#include "Outputs/Glowplug.h"
#include "Outputs/CoolantPump.h"
#include "Outputs/CombustionFan.h"

#include "Inputs/Temperature.h"
#include "Inputs/VoltageMeter.h"

// Pin Connections
int injectionPumpPin  = 9; 
int glowPlugPin       = 8;
int combustionFanPin  = 6; 
int coolantPumpPin    = 4;
int coolantTempPin    = A1;
int exhaustTempPin    = A2;
int lambda_pin        = A3;
int buttonPin         = A0;
int pumpSize          = 22;
double avarageVoltage = 0;

// PWM properties

/* timed evenets */
auto timer = timer_create_default();

// Common
Logger logger;

// Inputs
// Thermistor* coolantNtc  = new NTC_Thermistor(coolantTempPin,10000,10000,25,3836);
// Thermistor* exhautstNtc = new NTC_Thermistor(exhaustTempPin,100000,1320000,25,3836);
Thermistor *coolantNtc = new NTC_Thermistor(coolantTempPin, 10000, 10000, 25, 3836);
Thermistor *exhautstNtc = new NTC_Thermistor(exhaustTempPin, 10000, 10000, 25, 3836);
Temperature tempController(exhaustTempPin, coolantTempPin, 0, coolantNtc, exhautstNtc);
VoltageMeter voltageMeter(buttonPin);

// outputs
GlowPlug glowPlug(glowPlugPin);
CoolantPump coolantPump(coolantPumpPin);
CombustionFan combustionFan(combustionFanPin, 0);
InjectionPump injectionPump(injectionPumpPin, pumpSize);

DieselHeater dieselHeater(
    &glowPlug,
    &coolantPump,
    &voltageMeter,
    &tempController,
    &combustionFan,
    &injectionPump
  );

void everySecondCallback();
void runTimeCallback();

void setup()
{
  Serial.begin(115200);

  pinMode(glowPlugPin, OUTPUT);
  pinMode(injectionPumpPin, OUTPUT);
  pinMode(combustionFanPin, OUTPUT);
  pinMode(coolantPumpPin, OUTPUT);
  pinMode(coolantTempPin, INPUT);
  pinMode(exhaustTempPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(lambda_pin, INPUT);

  injectionPump.begin();

  everySecondCallback();
  runTimeCallback();
}

void loop()
{
  timer.tick();
  // int potValue    = analogRead(exhaustTempPin);
  // float frequency = map(potValue, 0, 1023, 0.3, 40) / 10.0;
  // frequency       = max(injectionPump.getMinFrequency(), min(injectionPump.getMaxFrequency(), frequency)); // constrain frequency to the range
  // injectionPump.setFrequency(frequency);
}

void everySecondCallback()
{
  timer.every(
      1000,
      [](void *) -> bool
      {
        tempController.setTemperatures();
        avarageVoltage = voltageMeter.getAverage();
        dieselHeater.run();
        dieselHeater.setState();
        dieselHeater.setTime();

        char logData[55]          = "";
        char voltageStr[7]        = "";
        char coolantTmpStr[7]     = "";
        char exhaustTmpStr[7]     = "";
        char insideTmpStr[7]      = "";
        char injectionRatioStr[7] = "";

        dtostrf(avarageVoltage, 5, 2, voltageStr);
        dtostrf(tempController.coolantTemp, 5, 2, coolantTmpStr);
        dtostrf(tempController.exhaustTemp, 5, 2, exhaustTmpStr);
        dtostrf(tempController.roomTemp, 5, 2, insideTmpStr);
        dtostrf(injectionPump.injectionRatio, 4, 2, injectionRatioStr);
        
        sprintf(
          logData,
          "Co %s | Ex %s | In %s | f %s | %d", 
          coolantTmpStr,
          exhaustTmpStr,
          insideTmpStr,
          injectionRatioStr,
          combustionFan.currentSpeed
         );

         Serial.print(logData);

        // Serial.print(combustionFan.currentSpeed);
        // Serial.print(" | ");
        // Serial.print(tempController.coolantTemp);
        // Serial.print(" | ");
        // Serial.print(tempController.exhaustTemp);

        logger.endRow();
        return true;
      });
}

void runTimeCallback()
{
  timer.every(
      100,
      [](void *) -> bool
      {
        combustionFan.tick();
        glowPlug.tick();
        return true;
      });
}