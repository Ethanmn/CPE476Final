/* 
   TreeGenerator.cpp
   Katie Keim
   Deer - CPE 476
*/
#include "TreeGenerator.h"

const int TREE_SIZE = 20;
const int TREE_DENSITY = 4; //Higher numbers here will mean less trees.
const int TREE_SCALE = 5;

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
            boxes.push_back(BoundingRectangle(glm::vec2(row * TREE_SIZE - groundSize, col * TREE_SIZE - groundSize), glm::vec2(4, 4), 0.0f));
         }
      }
   }

   //printConsoleTreeMap();
}

//Draw the trees
void TreeGenerator::drawTrees(Shader& shader, const UniformLocationMap& uniform_locations, const glm::mat4& viewMatrix) {
   glm::mat4 rotate, rotateTreeUp;
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

            rotate = glm::rotate(
               glm::mat4(1.0f),
               (float)((row * col) % 180),
               glm::vec3(0, 1, 0));

            translate = glm::translate(
                glm::mat4(1.0f),
               glm::vec3(row * TREE_SIZE - groundSize, TREE_SCALE * TREE_SIZE / 2, col * TREE_SIZE - groundSize));
            model_matrix = translate * rotate * scale * rotateTreeUp;

            shader.sendUniform(Uniform::NORMAL, uniform_locations, glm::transpose(glm::inverse(viewMatrix * model_matrix)));
            shader.sendUniform(Uniform::MODEL, uniform_locations, model_matrix);
            //shader.sendUniform(Uniform::COLOR, uniform_locations, glm::vec4(0, 1, 0, 0.5f));
            shader.drawMesh(treeMesh1);
         }
      }
   }

   /*for (int index = 0; index < (int)boxes.size(); index++) {
      boxes[index].draw(shader, uniform_locations, const glm::mat4& viewMatrix);
   }*/
}

//Get if there is a tree at a spot
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
