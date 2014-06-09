#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <glm/glm.hpp>

extern const int kScreenWidth;
extern const int kScreenHeight;
extern const float kScreenWidthf;
extern const float kScreenHeightf;

extern const bool kDebugUseDeerModels;

extern const float kAspectRatio;
extern const float kHeightMapScale;

extern float kFar;
extern float kNear;
extern float kFieldOfView;
extern glm::mat4 kProjectionMatrix;
extern bool useTextureShader;

glm::mat4 calculateProjection();

extern float cameraDistanceToDeer;
extern float cameraHeightAboveDeer;

#endif // GLOBALS_H_
