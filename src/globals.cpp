#include "globals.h"

#include <glm/gtc/matrix_transform.hpp>

const int kScreenWidth = 1280;
const int kScreenHeight = 800;

const float kScreenWidthf = kScreenWidth;
const float kScreenHeightf = kScreenHeight;

const float kFieldOfView = 80.0f;
const float kAspectRatio = kScreenWidthf / kScreenHeightf;
const float kNear = 1.0f;
const float kFar = 500.0f;

const glm::mat4 kProjectionMatrix = glm::perspective(kFieldOfView, kScreenWidthf/kScreenHeightf, kNear, kFar);

const float kHeightMapScale = 3.0f;
