#include "Arduino.h"
#include "HelperFunctions.h"

int HelperFunctions::mapf(int x, int in_min, int in_max, int out_min, int out_max) {
  x = constrain(x, in_min, in_max);
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
