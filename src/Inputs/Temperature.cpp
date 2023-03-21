#include "Temperature.h"

#define filterSamples 13 // filterSamples should  be an odd number, no smaller than 3

Temperature::Temperature(
    int exhaustTempPin,
    int coolantTempPin,
    int burn_mode,
    Thermistor *coolant = nullptr,
    Thermistor *exhaust = nullptr)
{
  Temperature::exhaustTempPin = exhaustTempPin;
  Temperature::coolantTempPin = coolantTempPin;
  Temperature::coolant = new SmoothThermistor(coolant, SMOOTHING_FACTOR);
  Temperature::exhaust = new SmoothThermistor(exhaust, SMOOTHING_FACTOR);
  Temperature::dht = new DHT_Unified(DHTPIN, DHTTYPE);

  dht->begin();
  dht->temperature().getSensor(&sensor);
  dht->humidity().getSensor(&sensor);
}

void Temperature::setTemperatures()
{

  coolantTemp = coolant->readCelsius();
  exhaustTemp = exhaust->readCelsius();
  this->setToomTemperature();

  // Serial.print("Coolant: ");
  // Serial.print(coolantCelsius);

  // Array of historic exhaust temperatures
  for (int i = 10; i >= 1; i--)
  {                                            // updating the exhaust temperature history
    exhaustTempSec[i] = exhaustTempSec[i - 1]; // shift array values, deleting the older one
  }
  exhaustTempSec[0] = exhaustTemp; // add new temp value

  String exhaustHistorical;

  for (float i : exhaustTempSec)
  {
    exhaustHistorical += String(i) + " ";
  }

  // Logger::getDriver().info("Exhaust [ %s]", exhaustHistorical.c_str());
}

// int Temperature::read(int pin)
// {
//   float temp_temp = 0;
//   if (pin == exhaustTempPin)
//   {
//     // rawDataExhaust = thermistor->read()/10;

//     if (millis() - GWTLast_Sec > 1000)
//     {
//       GWTLast_Sec = millis();

//       /*
//           //When the fan speed is changing, it sometimes affects the temp readings.  Mute changes when
//           if((EX_Mute == false) && (temp_temp > 0)) {
//             Last_Mute_T = temp_temp;
//           } else {
//             temp_temp = Last_Mute_T;
//           }
//       */
//       // Limit rate of change to Max_Change_Per_Sec
//       if (temp_temp < 120)
//       {
//         // At low temperatures rise fast, fall slow damping
//         if ((Last_Exh_T - temp_temp) > Max_Change_Per_Sec)
//         {
//           if ((temp_temp - Last_Exh_T) > 0)
//           {
//             Last_Exh_T += Max_Change_Per_Sec;
//           }
//           else
//           {
//             Last_Exh_T -= Max_Change_Per_Sec;
//           }
//         }
//         else
//         {
//           Last_Exh_T = temp_temp;
//         }
//       }
//       else
//       {
//         // At higher temperatures damp temp rise & fall
//         if (abs(Last_Exh_T - temp_temp) > Max_Change_Per_Sec)
//         {
//           if ((temp_temp - Last_Exh_T) > 0)
//           {
//             Last_Exh_T += Max_Change_Per_Sec;
//           }
//           else
//           {
//             Last_Exh_T -= Max_Change_Per_Sec;
//           }
//         }
//         else
//         {
//           Last_Exh_T = temp_temp;
//         }
//       }
//     }
//     return Last_Exh_T;
//   }
//   else
//   {
//     // rawDataWater = thermistor2->read();
//     rawDataWater = 0;

//     rawDataWater = rawDataWater / 10;
//     // if(debug_glow_plug_on) rawDataWater = rawDataWater -14; //Glow plug causes the temp to misread by 14C

//     if (temp_temp < 0 || temp_temp > 100)
//       temp_temp = 101; // Out of range, make sure heater does not start
//     if (((millis() - Last_TSec > 1000) && burn_mode == 2) || ((millis() - Last_TSec > 300) && burn_mode != 2))
//     {
//       Last_TSec = millis();
//       // Limit rate of change to Max_Change_Per_Sec
//       temp_temp = 0; // was => digitalSmooth(rawDataWater, CoolantSmoothArray);
//       temp_temp = rawDataWater;

//       // Limit the rate the read temperature is allowed to fall
//       if ((Last_Wat_T - temp_temp) > Max_Change_Per_Sec)
//       {

//         if ((temp_temp - Last_Wat_T) > 0)
//         {
//           Last_Wat_T += Max_Change_Per_Sec;
//         }
//         else
//         {
//           Last_Wat_T -= Max_Change_Per_Sec;
//         }
//       }
//       else
//       {
//         Last_Wat_T = temp_temp;
//       }

//       // Last_Wat_T = SVWFilter(temp_temp);  //Only feed data to filter once every 5 sec
//       // Last_Wat_T = temp_temp;  //Only feed data to filter once every 5 sec
//     }
//     return Last_Wat_T;
//   }
// };

void Temperature::setToomTemperature()
{
  sensors_event_t event;
  dht->temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    // Serial.println("Error reading temperature!");
  }
  else
  {
    roomTemp = event.temperature;
  }
}

void Temperature::setInitExhaustTemp()
{
  exhaustTempInit = exhaustTemp;
}