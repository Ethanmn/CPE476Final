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
   void nightToDay();
   void dayToNight();
   void on();
   void off();
   void adjustSun();
   
   glm::vec3 sunDir;
   float sunIntensity;
   bool switchingTime;
   bool switchToNight; 
   /* true will be from mid-day to night, false from night to mid-day */

   float timeOfDay;
   
};

#endif
