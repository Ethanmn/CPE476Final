/* 
   TreeGenerator.h
   Katie Keim
   Deer - CPE 476
*/
#include "ground_plane.h"
#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "bounding_rectangle.h"

struct TreeGenerator {
   TreeGenerator(const Mesh& mesh);

   void generateTrees();
   void drawTrees(Shader& shader, const UniformLocationMap& uniform_locations, const glm::mat4& viewMatrix);
   void shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir, glm::vec3 deerPos);
   std::vector<BoundingRectangle> getBoundingBoxes();

   private:
      Mesh treeMesh1;

      std::vector<std::vector<bool>> trees;
      void printConsoleTreeMap(); 
      std::vector<BoundingRectangle> boxes;
};
