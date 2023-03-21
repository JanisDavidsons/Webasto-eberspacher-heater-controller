#ifndef _HEATER_LOGGER_H
#define _HEATER_LOGGER_H
#include <Arduino.h>
#include "Common/Logger.h"

class Logger
{
private:

public:
  Logger();
  static String infoMessages;
  static void addInfoMessage(String message);
  void printInfoMessages();
  void printInfoMessage(String message);
  void endRow();
};

#endif