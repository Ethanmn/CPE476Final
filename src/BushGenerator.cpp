/* 
   BushGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "BushGenerator.h"

const int BUSH_DENSITY = 4; //Larger value means MORE bushes

const int BUSH_SIZE = 10;

//Need the * 100 to translate between modulo and floats
const float BUSH_SCALE_MIN = 0.8 * 100;
const float BUSH_SCALE_MAX = 1.3 * 100;

const int BUSH_RUSTLE_MIN = 150;
const int BUSH_RUSTLE_MAX = 450;

BushGenerator::BushGenerator(const Mesh& mesh) {
   bushMesh1 = mesh;
   bushMesh1.material = Material(glm::vec3(1.2) * glm::vec3(0.45, 0.24, 0.15));
}

//Generate the trees
void BushGenerator::generate() {
   std::vector<std::vector<bool>> trees;
   int size = GroundPlane::GROUND_SCALE / BUSH_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      for (int col = 0; col < size; col++) {
         if (rand() % BUSH_DENSITY == 0) {
            bushes.push_back(Bush(bushMesh1, glm::vec3(row, 0, col), (rand() % (int)(BUSH_SCALE_MAX - BUSH_SCALE_MIN) + BUSH_SCALE_MIN)  / 100.0f, rand() % (int)(BUSH_RUSTLE_MAX - BUSH_RUSTLE_MIN) + BUSH_RUSTLE_MIN));
         }
      }
   }
}

std::vector<Bush> BushGenerator::getBushes() {
   return bushes;
}
