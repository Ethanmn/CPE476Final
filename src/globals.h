#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <glm/glm.hpp>

extern const int kScreenWidth;
extern const int kScreenHeight;
extern const float kScreenWidthf;
extern const float kScreenHeightf;

extern const bool kDebugUseDeerModels;

extern const float kAspectRatio;
extern float gHeightMapScale;

extern float gFar;
extern float gNear;
extern float gFieldOfView;
extern glm::mat4 gProjectionMatrix;
extern bool useTextureShader;
extern bool gReflections;

glm::mat4 calculateProjection();

extern float cameraDistanceToDeer;
extern float cameraHeightAboveDeer;

extern const int kMaxDensityLevel;
extern int gDensityLevel;

extern float redFlowerTimer;

#endif // GLOBALS_H_
