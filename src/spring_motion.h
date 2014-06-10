#ifndef SPRING_MOTION_H_
#define SPRING_MOTION_H_

#include <glm/glm.hpp>
#include "units.h"

//Original Spring Motion written by Katie Keim.
//Moved into a separate file to be used in multiple places.

// Templated to work with vectors or scalars.
template<typename T>
struct SpringMotion {
   SpringMotion(float strength, float damp, float speed) :
      strength_(strength),
      damp_(damp),
      speed_(speed)
   {}

   T step(units::MS dt, const T& target, T position) const {
      const auto displacement = position - target;
      const auto dispLength = glm::length(displacement);

      //Only move if the distance is not super small
      if (dispLength >= 0.09f) {
         //Find the magnitude of the spring for this distance
         const auto springMag = strength_ * dispLength + damp_ * (1 / dispLength);

         //Calculate the scale of displacement based on the magnitude and distance
         const auto scalar = (1.0f / dispLength) * springMag;

         //Apply the scale and move based on the scaled displacement
         position -= dt * speed_ * displacement * scalar;
      } else {
         position = target;
      }
      return position;
   }

  private:
   float strength_;      //Larger = stiffer spring
   float damp_;          //Larger = lighter mass (more spring motion),
   float speed_;         //Larger = faster spring movement
};

#endif // SPRING_MOTION_H_
