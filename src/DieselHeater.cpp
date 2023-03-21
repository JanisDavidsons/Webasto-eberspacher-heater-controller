#include "DieselHeater.h"
#include "Arduino.h"

DieselHeater::DieselHeater(
    GlowPlug *glowPlug           = nullptr,
    CoolantPump *coolantPump     = nullptr,
    VoltageMeter *voltmeter      = nullptr,
    Temperature *tempController  = nullptr,
    CombustionFan *combustionFan = nullptr,
    InjectionPump *injectionPump = nullptr
    )
{
  this->glowPlug       = glowPlug;
  this->coolantPump    = coolantPump;
  this->voltmeter      = voltmeter;
  this->tempController = tempController;
  this->combustionFan  = combustionFan;
  this->injectionPump  = injectionPump;
}

void DieselHeater::setState()
{
  Serial.print(stateTitleMap[heaterState]);
  Serial.print(" | ");
  Serial.print(message);

  double switchVoltage = voltmeter->getAverage();

  if (switchVoltage > minVoltageTreshold && startFailures < 3)
  {
    if (heaterState == DieselHeater::OFF || heaterState == DieselHeater::SLEEP)
    {
      coolantPump->turnOn();

      if (tempController->coolantTemp < minHeaterTemp)
      {
        heaterState = DieselHeater::STARTING;
      }
    }
  }
  else if(switchVoltage > 1 && switchVoltage < minVoltageTreshold)
  {
    heaterState = DieselHeater::SLEEP;   
    message = "V-l | "; 
  } else if (switchVoltage < 1){
    heaterState = DieselHeater::OFF;
    message = "Sw-o | "; 
  }
}

void DieselHeater::run()
{
  switch (heaterState)
  {
  case DieselHeater::OFF:
    offProcedure();
    break;
  case DieselHeater::STARTING:
    startUpProcedure();
    break;  
  case DieselHeater::RUNNING:
    runProcedure();
    break;  
  case DieselHeater::SLEEP:
    sleepProcedure();
    break;
  case DieselHeater::RESTARTING:
    restartProcedure();
    break;
  case DieselHeater::SHUTTING_DOWN:
    shutDownProcedure();
  break;
  }
}

void DieselHeater::startUpProcedure()
{
  if (tempController->exhaustTemp > 150)
  {
    message = "Cooling < 150";
    combustionFan->run(80);
    injectionPump->turnOff();

    return;
  }

  if (hasErrors)
  {
    return;
  }

  timeSinceSleep      = 0;
  timeSinceOff        = 0;
  glowTime            = 0;

  switch (timeSinceStarting)
  {
  case 1:
      message = "cch | ";
      combustionFan->run(70);
      tempController->setInitExhaustTemp();
      glowPlug->run(100, 60, 1);
      injectionPump->turnOff();
    break;
  case 7:
      message = "p1 | ";
      combustionFan->run(15, 1);
      injectionPump->setPrimeRatio(tempController->coolantTemp);
    break;
  case 10:
    message="p0 | ";
    combustionFan->turnOff();
    injectionPump->turnOff();
  break;
  case 17 ... 40:
    message = "st | ";
    combustionFan->run(CombustionFan::START_FAN_SPEED, 1);
    injectionPump->setInitInjectionRatio(tempController->exhaustTempInit);

    if (tempController->exhaustTemp - tempController->exhaustTempInit > 15)
      {
        heaterState   = DieselHeater::RUNNING;
        message       = "on | ";
        startFailures = 0;
        glowPlug      ->turnOff();
        tempController->setInitExhaustTemp();
      }
  break;
    case 41:
      heaterState         = DieselHeater::RESTARTING;
      timeSinceRestarting = 0;
      startFailures++;
    break;
  }
}

void DieselHeater::runProcedure()
{
  timeSinceStarting = 0;

  switch (timeSinceRunning)
  {
  case 1:
    combustionFan->run(95, 1);
    break;
  
  default:
    break;
  }
}

void DieselHeater::restartProcedure()
{
  if (startFailures >=3)
  {
    heaterState = DieselHeater::SHUTTING_DOWN;
    return;
  }

  switch (timeSinceRestarting)
  {
  case 1:
      glowPlug     ->run(100, 10, 1);
      injectionPump->turnOff();
      timeSinceStarting = 0;
      message           = "rt |";
    break;
  case 120:
    heaterState = DieselHeater::STARTING;
  break;
  
  default:
    break;
  }
}

void DieselHeater::sleepProcedure()
{
  timeSinceStarting = 0;
  injectionPump->turnOff();

    switch (timeSinceSleep)
    {
    case 0:
      combustionFan->run(40);;
      break;

    case 20:
      combustionFan->turnOff();
      glowPlug     ->turnOff();
      break;

    case 40:
      coolantPump->turnOff();
      break;
    }
}

void DieselHeater::offProcedure()
{
  timeSinceRunning      = 0;
  timeSinceSleep        = 0;
  timeSinceStarting     = 0;
  timeSinceRestarting   = 0;
  timeSinceShuttingDown = 0;
  injectionPump->turnOff();
  glowPlug     ->turnOff();
  combustionFan->turnOff();
  coolantPump  ->turnOff();

  if (timeSinceOff > 300)
  {
    startFailures = 0;
  }
  
}

void DieselHeater::shutDownProcedure()
{
  timeSinceRunning    = 0;
  timeSinceStarting   = 0;
  timeSinceRestarting = 0;

  injectionPump->turnOff();
  glowPlug     ->turnOff();

  switch (timeSinceShuttingDown)
    {
    case 10:
      combustionFan->turnOff();
      break;
    case 20:
      coolantPump->turnOff();
      break;
    case 30:
      heaterState = DieselHeater::OFF;
      break;
    }
}

void DieselHeater::setTime()
{
  switch (heaterState)
  {
  case DieselHeater::OFF:
    timeSinceOff++;
    break;
  case DieselHeater::SLEEP:
    timeSinceSleep++;
    break;
  case DieselHeater::STARTING:
      timeSinceStarting++;
    break;
  case DieselHeater::RUNNING:
      timeSinceRunning++;
    break;
  case DieselHeater::SHUTTING_DOWN:
    timeSinceShuttingDown++;
    break;
  case DieselHeater::RESTARTING:
    timeSinceRestarting++;
  break;  
  default:
    // unexpected state, turn off imidiatelly
    hasErrors = true;
    heaterState = DieselHeater::OFF;
    break;
  }

  Seconds =  timeSinceRunning  % SECS_PER_MIN ;
  Minutes = (timeSinceRunning  / SECS_PER_MIN) % SECS_PER_MIN;
  Hours   = (timeSinceRunning  % SECS_PER_DAY) / SECS_PER_HOUR;  
}