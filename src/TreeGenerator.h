/* 
   TreeGenerator.h
   Katie Keim
   Deer - CPE 476
*/
#include "GroundPlane.h"
#include <cstdlib>
#include "graphics/shader.h"
#include "graphics/location_maps.h"
#include "graphics/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

struct TreeGenerator {
   TreeGenerator(const Mesh& mesh);

   void generateTrees();
   void drawTrees(Shader& shader, const UniformLocationMap& uniform_locations, const glm::mat4& viewMatrix);
   bool checkForTree(int x, int z);

   private:
      Mesh treeMesh1;

      std::vector<std::vector<bool>> trees;
      void printConsoleTreeMap(); 
};
