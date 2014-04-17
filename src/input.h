#ifndef INPUT_H_
#define INPUT_H_

#include <set>

#include <SDL.h>

struct Input {
   // Call at the beginning of a frame to reset the held/released keys.
   void beginFrame() {
      pressed_keys_.clear();
      released_keys_.clear();
   }
   void keyDown(const SDL_KeyboardEvent& event) {
      pressed_keys_.insert(scancode_of(event));
      held_keys_.insert(scancode_of(event));
   }
   void keyUp(const SDL_KeyboardEvent& event) {
      held_keys_.erase(scancode_of(event));
      released_keys_.insert(scancode_of(event));
   }

   // Return whether the key was pressed *in this frame*.
   bool wasKeyPressed(SDL_Scancode scancode) {
      return pressed_keys_.count(scancode) != 0;
   }

   // Return whether the key was released *in this frame*.
   bool wasKeyReleased(SDL_Scancode scancode) {
      return released_keys_.count(scancode) != 0;
   }

   // Return whether the key is *currently* being held.
   // Returns true if the key was pressed in this frame.
   bool isKeyHeld(SDL_Scancode scancode) {
      return held_keys_.count(scancode) != 0;
   }

  private:
   SDL_Scancode scancode_of(const SDL_KeyboardEvent& event) const {
      return event.keysym.scancode;
   }
   std::set<SDL_Scancode> pressed_keys_;
   std::set<SDL_Scancode> released_keys_;
   std::set<SDL_Scancode> held_keys_;
};

#endif // INPUT_H_
