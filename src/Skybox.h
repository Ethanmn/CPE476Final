/* 
   Skybox.h
   Katie Keim
   Deer - CPE 476
*/

#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <vector>
#include "ground_plane.h"
#include "graphics/draw_template.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"

struct Skybox {
   Skybox(Mesh mesh);

   glm::mat4 calculateModel() const;
   std::vector<Drawable> drawables(bool isDay) const;

   private:
      DrawTemplate draw_template_day;
      DrawTemplate draw_template_night;
};

#endif //SKYBOX_H_ 
