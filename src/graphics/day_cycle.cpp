#include <glm/glm.hpp>
#include <OPENGL/gl.h>
#include "day_cycle.h"
#include <stdio.h>
#include <string>

using namespace std;

DayCycle::DayCycle() :
   sunDir(glm::vec3(0.5, 0.5, 0.2)),
   sunIntensity(0.5),
   dayToNight(false),
   timeOfDay(0.25)
{}

glm::vec3 DayCycle::getSunDir() {
   return sunDir;
}

float DayCycle::getSunIntensity() {
   return sunIntensity;
}

void DayCycle::adjustToTime(float newTime) {
   timeOfDay = newTime;
   adjustSun();
}

void DayCycle::switchBoolean() {
   dayToNight = !dayToNight;
}

void DayCycle::autoAdjustTime() {
   if(dayToNight) {
         timeOfDay += 0.001;
      if(timeOfDay >= 1.0)
         timeOfDay = 0.0;
      adjustSun();
   }
}

void DayCycle::adjustSun() {
   float tempTimeOfDay = timeOfDay;
   glClearColor (0.05098 * sunIntensity, 0.6274509 * sunIntensity,
                 sunIntensity, 1.0f);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if(tempTimeOfDay > 1.0)
      sunIntensity = 0.1;
   else {
      sunDir.x = cos(PI * tempTimeOfDay);
      sunDir.y = sin(PI * tempTimeOfDay);
      if(tempTimeOfDay > 0.5)
         tempTimeOfDay = 1.0 - tempTimeOfDay;
      tempTimeOfDay /= 0.5;
      sunIntensity = tempTimeOfDay;
   }
}
