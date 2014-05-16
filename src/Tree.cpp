/*
   Tree.cpp
   Katie Keim
   CPE 476 - Deer
   */
#include "Tree.h"

const int TREE_SCALE = 5;

const int BOUNDING_SIZE = 4;
const float BOUNDING_ERR_X = 7.26;
const float BOUNDING_ERR_Z = -1.75;

Tree::Tree(glm::vec3 position) :
   bRect(BoundingRectangle(glm::vec2(position.x + BOUNDING_ERR_X, position.z + BOUNDING_ERR_Z), 
            glm::vec2(BOUNDING_SIZE, BOUNDING_SIZE), 0.0f)),
   position(position)
{}

glm::mat4 Tree::calculateModel() const {
   const auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(TREE_SCALE));

   const auto rotateTreeUp = glm::rotate(
         glm::mat4(1.0f),
         (float)(-90),
         glm::vec3(1, 0, 0));

   const auto translate = glm::translate(
         glm::mat4(1.0f),
         glm::vec3(position.x, position.y, position.z));
   return glm::mat4(translate * scale * rotateTreeUp);
} 

bool Tree::isBlocker() {
   return true;
}
/*
void Tree::shadowDraw(Shader& shader, const UniformLocationMap& uniform_locations,
      glm::vec3 sunDir) {
   const auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(TREE_SCALE));

   const auto rotateTreeUp = glm::rotate(
         glm::mat4(1.0f),
         (float)(-90),
         glm::vec3(1, 0, 0));

   const auto translate = glm::translate(
         glm::mat4(1.0f),
         glm::vec3(position.x, position.y, position.z));
   const glm::mat4 model_matrix(translate * scale * rotateTreeUp);

   if(betterShadow)
      setupBetterShadowShader(shader, uniform_locations, sunDir, model_matrix);
   else
      setupShadowShader(shader, uniform_locations, sunDir, model_matrix);
   shader.drawMesh(draw_template_.mesh);
}
*/


void Tree::performObjectHit(SoundEngine&) {
}
