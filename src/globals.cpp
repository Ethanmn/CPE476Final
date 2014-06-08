#include "globals.h"

#include <glm/gtc/matrix_transform.hpp>

const int kScreenWidth = 1280;
const int kScreenHeight = 800;

const float kScreenWidthf = kScreenWidth;
const float kScreenHeightf = kScreenHeight;

const float kAspectRatio = kScreenWidthf / kScreenHeightf;

glm::mat4 calculateProjection() {
   return glm::perspective(kFieldOfView, kScreenWidthf/kScreenHeightf, kNear, kFar);
}
glm::mat4 kProjectionMatrix = calculateProjection();

const float kHeightMapScale = 3.0f;

float kNear = 1.0f;
float kFieldOfView = 80.0f;
float kFar = 500.0f;
bool useTextureShader = true;
