#include "Logger.h"

Logger::Logger()
{
  infoMessages = "";
}

void Logger::addInfoMessage(String message)
{
  infoMessages += (message += " | ") ;
}

void Logger::printInfoMessages()
{
  Serial.print(infoMessages);
}

void Logger::printInfoMessage(String message)
{
  Serial.print(message);
  Serial.print(" | ");
}

void Logger::endRow()
{
  Serial.print("\n");
}

String Logger::infoMessages;