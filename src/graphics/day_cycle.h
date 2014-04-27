#ifndef SUN_STATUS_H_
#define SUN_STATUS_H_
#include <iostream>
#include <string>
#include "units.h"

struct DayCycle {
   DayCycle();
   glm::vec3 getSunDir();
   float getSunIntensity();
   void adjustToTime(float newTime);
   void autoAdjustTime(units::MS dt);
   void switchBoolean();
   void on();
   void off();
   void adjustSun();
   
   glm::vec3 sunDir;
   float sunIntensity;
   bool dayToNight;
   float timeOfDay;
   
};

#endif
