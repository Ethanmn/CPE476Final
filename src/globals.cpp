#include "globals.h"

#include <glm/gtc/matrix_transform.hpp>

const int kScreenWidth = 1280;
const int kScreenHeight = 800;

const float kScreenWidthf = kScreenWidth;
const float kScreenHeightf = kScreenHeight;

const float kAspectRatio = kScreenWidthf / kScreenHeightf;

glm::mat4 calculateProjection() {
   return glm::perspective(gFieldOfView, kScreenWidthf/kScreenHeightf, gNear, gFar);
}
glm::mat4 gProjectionMatrix = calculateProjection();

float gHeightMapScale = 10.5f;

float gNear = 1.0f;
float gFieldOfView = 80.0f;
float gFar = 1500.0f;
bool useTextureShader = true;
bool gReflections = true;

float cameraDistanceToDeer = 23.0f;
float cameraHeightAboveDeer = 12.0f;
