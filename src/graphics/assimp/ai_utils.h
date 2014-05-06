#ifndef AI_UTILS_H_
#define AI_UTILS_H_

#include <glm/glm.hpp>
#include <assimp/scene.h>

inline glm::mat4 fromAiMatrix4x4(const aiMatrix4x4& ai_mat) {
   glm::mat4 glm_mat;
   glm_mat[0][0] = ai_mat.a1;
   glm_mat[1][0] = ai_mat.a2;
   glm_mat[2][0] = ai_mat.a3;
   glm_mat[3][0] = ai_mat.a4;

   glm_mat[0][1] = ai_mat.b1;
   glm_mat[1][1] = ai_mat.b2;
   glm_mat[2][1] = ai_mat.b3;
   glm_mat[3][1] = ai_mat.b4;

   glm_mat[0][2] = ai_mat.c1;
   glm_mat[1][2] = ai_mat.c2;
   glm_mat[2][2] = ai_mat.c3;
   glm_mat[3][2] = ai_mat.c4;

   glm_mat[0][3] = ai_mat.d1;
   glm_mat[1][3] = ai_mat.d2;
   glm_mat[2][3] = ai_mat.d3;
   glm_mat[3][3] = ai_mat.d4;
   return glm_mat;
}

#endif // AI_UTILS_H_
