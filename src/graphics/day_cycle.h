#ifndef SUN_STATUS_H_
#define SUN_STATUS_H_
#include <iostream>
#include <string>

#define PI 3.1415926

struct DayCycle {
   DayCycle();
   glm::vec3 getSunDir();
   float getSunIntensity();
   void adjustToTime(float newTime);
   void autoAdjustTime();
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
