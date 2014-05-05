/* 
   TreeGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "TreeGenerator.h"
#include "graphics/shader_setup.h"

const int TREE_SIZE = 20;
const int TREE_DENSITY = 4; //Higher numbers here will mean less trees.
const int TREE_SCALE = 5;

const float BOUNDING_ERR_X = 7.26;
const float BOUNDING_ERR_Z = -1.75;

TreeGenerator::TreeGenerator(const Mesh& mesh) {
   treeMesh1 = mesh;
}

//Generate the trees
void TreeGenerator::generateTrees() {
   int size = GroundPlane::GROUND_SCALE / TREE_SIZE; 
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for (int row = 0; row < size; row++) {
      std::vector<bool> colVec;
      for (int col = 0; col < size; col++) {
         bool hasTree = (rand() % TREE_DENSITY == 0);
         colVec.push_back(hasTree);
      }
      trees.push_back(colVec);
   }

   for(int row = 0; row < (int)trees.size(); row++) {
      std::vector<bool> colVec = trees[row];
      for (int col = 0; col < (int)colVec.size(); col++) {
         if (colVec[col]) {
            boxes.push_back(BoundingRectangle(glm::vec2(row * TREE_SIZE - groundSize + BOUNDING_ERR_X, col * TREE_SIZE - groundSize + BOUNDING_ERR_Z), glm::vec2(4, 4), 0.0f));
         }
      }
   }

   //printConsoleTreeMap();
}

//Draw the trees
void TreeGenerator::drawTrees(Shader& shader, const UniformLocationMap& uniform_locations, const glm::mat4& viewMatrix) {
   glm::mat4 rotateTreeUp;
   glm::mat4 translate;
   glm::mat4 scale;
   glm::mat4 model_matrix;
   int groundSize = GroundPlane::GROUND_SCALE / 2;

   for(int row = 0; row < (int)trees.size(); row++) {
      std::vector<bool> colVec = trees[row];
      for (int col = 0; col < (int)colVec.size(); col++) {
         if (colVec[col]) {
            scale = glm::scale(glm::mat4(1.0f), glm::vec3(TREE_SCALE));

            rotateTreeUp = glm::rotate(
               glm::mat4(1.0f),
               (float)(-90),
               glm::vec3(1, 0, 0));

            translate = glm::translate(
                glm::mat4(1.0f),
               glm::vec3(row * TREE_SIZE - groundSize, TREE_SCALE * TREE_SIZE / 2, col * TREE_SIZE - groundSize));
            model_matrix = translate * scale * rotateTreeUp;

            setupModelView(shader, uniform_locations, model_matrix,
                           viewMatrix, true);
            shader.drawMesh(treeMesh1);
         }
      }
   }

   for (int index = 0; index < (int)boxes.size(); index++) {
      boxes[index].draw(uniform_locations, shader, 0.0f, viewMatrix);
   }
}

std::vector<BoundingRectangle> TreeGenerator::getBoundingBoxes() {
   return boxes;
}

void TreeGenerator::printConsoleTreeMap() {
   std::vector<std::vector<bool>>::const_iterator itRow;
   std::vector<bool>::const_iterator itCol;

   printf("Tree Map:\n");
   for(itRow = trees.begin(); itRow != trees.end(); itRow++) {
      std::vector<bool> col = *itRow;
      for (itCol = col.begin(); itCol != col.end(); itCol++) {
         if (*itCol) {
            printf(" X ");
         }
         else {
            printf(" - ");
         }
      }
      printf("\n");
   }     
}
