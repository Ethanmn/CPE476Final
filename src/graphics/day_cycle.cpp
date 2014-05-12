#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#endif
#include <glm/glm.hpp>
#include "day_cycle.h"
#include <stdio.h>
#include <string>
#include <glm/gtc/constants.hpp>

using namespace std;

DayCycle::DayCycle() :
   sunDir(glm::vec3(0.0, 1.0, 0.2)),
   sunIntensity(1.0),
   switchingTime(false),
   timeOfDay(0.5)
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

void DayCycle::dayToNight() {
   switchingTime = true;
   switchToNight = false;
}

void DayCycle::nightToDay() {
   switchingTime = true;
   switchToNight = true;
}


void DayCycle::on() {
   switchingTime = true;
}


void DayCycle::off() {
   switchingTime = false;
}

void DayCycle::autoAdjustTime(units::MS dt) {
   if(switchingTime) {
         timeOfDay += dt * 0.0001;
      if(timeOfDay >= 1.0)
         timeOfDay = 0.0;
      adjustSun();
   }
}

void DayCycle::adjustSun() {
   float tempTimeOfDay = timeOfDay;
   
   glClearColor (0.05098 * sunIntensity, 
                 0.6274509 * sunIntensity,
                 sunIntensity, 1.0f);
   
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if(switchingTime && switchToNight && timeOfDay <= 0.1)
      switchingTime = false;
   else if(switchingTime && !switchToNight && timeOfDay >= 0.35 && timeOfDay <= 0.65)
      switchingTime = false; 
 
   if(switchingTime) {
      if(tempTimeOfDay > 1.0)
         sunIntensity = 0.1;
      sunDir.x = cos(glm::pi<float>() * tempTimeOfDay);
      sunDir.y = sin(glm::pi<float>() * tempTimeOfDay);
      if(tempTimeOfDay > 0.5)
         tempTimeOfDay = 1.0 - tempTimeOfDay;
      tempTimeOfDay /= 0.5;
      sunIntensity = tempTimeOfDay;
   }
}
