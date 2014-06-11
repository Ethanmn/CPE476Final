/*
   GameObject.h
   Katie Keim
   CPE 476 - Deer
*/
#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "bounding_rectangle.h"
#include "globals.h"

struct SoundEngine;

struct GameObject {
   virtual BoundingRectangle getBoundingRectangle() = 0;

   virtual bool isBlocker() = 0;

   virtual void performObjectHit(SoundEngine& sound_engine) = 0;
   virtual int  density() const = 0;

};
inline int makeDensity() {
   return rand() % (kMaxDensityLevel +1 );
}

#endif //GAME_OBJECT_H_
