#ifndef GROUND_PLANE_H_
#define GROUND_PLANE_H_

//#include "bounds.h"
//#include "graphics/gl_adapters/uniform_matrix.h"
#include "graphics/mesh.h"
#include "glm/glm.hpp"
//#include "graphics/model_view_uniform_matrix.h"

struct Shader;
struct Shaders;

struct GroundPlane {
   GroundPlane(Shaders& shaders);

   void draw(Shader& shader, const glm::mat4& view);

   //Bounds x_bounds() const { return Bounds(-10, 10); }
   //Bounds z_bounds() const { return Bounds(-10, 10); }

  private:
   Mesh mesh_;
   //ModelViewUniformMatrix model_matrix_;
   GLUniformLocationMap blue_uniform_;
};

#endif // GROUND_PLANE_H_
