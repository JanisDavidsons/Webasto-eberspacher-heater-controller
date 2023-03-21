#ifndef _DIESEL_HEATER_H
#define _DIESEL_HEATER_H

#include "Arduino.h"

#include "Common/Logger.h"

#include "Outputs/InjectionPump.h"
#include "Outputs/GlowPlug.h"
#include "Outputs/CoolantPump.h"
#include "Outputs/CombustionFan.h"

#include "Inputs/Temperature.h"
#include "Inputs/VoltageMeter.h"

class DieselHeater
{
private:
  GlowPlug *glowPlug           = nullptr;
  CoolantPump *coolantPump     = nullptr;
  VoltageMeter *voltmeter      = nullptr;
  Temperature *tempController  = nullptr;
  CombustionFan *combustionFan = nullptr;
  InjectionPump *injectionPump = nullptr;

  void startUpProcedure();
  void runProcedure();
  void sleepProcedure();
  void offProcedure();
  void restartProcedure();
  void shutDownProcedure();

public:
  DieselHeater(
      GlowPlug *glowPlug,
      CoolantPump *coolantPump,
      VoltageMeter *voltmeter,
      Temperature *tempController,
      CombustionFan *combustionFan,
      InjectionPump *injectionPump
  );

  unsigned long int timeSinceOff;    
  unsigned long int timeSinceSleep;    
  unsigned long int timeSinceRunning;    
  unsigned long int timeSinceStarting;    
  unsigned long int timeSinceRestarting;    
  unsigned long int timeSinceShuttingDown;    

  const unsigned int SECS_PER_MIN      = 60;
  const unsigned int SECS_PER_HOUR     = 3600;
  const unsigned long int SECS_PER_DAY = 86400;

  unsigned int Seconds;
  unsigned int Minutes;
  unsigned int Hours;    

  static const int OFF           = 0;
  static const int SLEEP         = 1;
  static const int STARTING      = 2;
  static const int RUNNING       = 3;
  static const int SHUTTING_DOWN = 4;
  static const int RESTARTING    = 5;

  const char* stateTitleMap[6] = { 
    "off", 
    "sleep",
    "starting", 
    "running", 
    "shutting down",
    "restarting"
  };

  int heaterState                = DieselHeater::OFF;
  int startFailures              = 0;
  const float minVoltageTreshold = 10.5;
  String shutDownReason          = "";
  int glowTime                   = 0;
  bool cooledOff                 = false;
  bool hasErrors                 = false;
  String message                 = "";

  // Heater Config
  static const int minHeaterTemp = 55;  // Increase fuel if below

  bool lean_burn;

  int full_power_increment_time = 30; // seconds

  void setState();
  void setTime();
  void run();
};

#endif