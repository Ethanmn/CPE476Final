uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

uniform int uHasTexture;

uniform mat4 uShadowMap;

#ifdef USE_BONES
uniform int uHasBones;
uniform mat4 uBones[40];
#endif

attribute vec3 aTexCoord;
attribute vec3 aPosition;
attribute vec3 aNormal;

#ifdef USE_BONES
attribute float aBoneID0;
attribute float aBoneWeight0;
attribute float aBoneID1;
attribute float aBoneWeight1;
attribute float aBoneID2;
attribute float aBoneWeight2;
attribute float aBoneID3;
attribute float aBoneWeight3;
attribute float aBoneID4;
attribute float aBoneWeight4;
#endif

varying vec4 vViewer;
varying vec3 vNormal;
varying vec2 vTexCoord;
varying vec4 vShadow;
varying float vHeightMapHeight;

mat4 calculateBones() {
   mat4 bone = mat4(1.0);
#ifdef USE_BONES
   if (uHasBones != 0) {
      if (int(aBoneID0) != -1) {
         bone = uBones[int(aBoneID0)] * aBoneWeight0;
      }
      if (int(aBoneID1) != -1) {
         bone += uBones[int(aBoneID1)] * aBoneWeight1;
      }
      if (int(aBoneID2) != -1) {
         bone += uBones[int(aBoneID2)] * aBoneWeight2;
      }
      if (int(aBoneID3) != -1) {
         bone += uBones[int(aBoneID3)] * aBoneWeight3;
      }
      if (int(aBoneID4) != -1) {
         bone += uBones[int(aBoneID4)] * aBoneWeight4;
      }
   }
#endif
   return bone;
}

void main() {
   vec4 pos = uModelMatrix * calculateBones() * vec4(aPosition, 1.0);
   vViewer = uViewMatrix * pos;
   vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0));
   vTexCoord = uHasTexture != 0 ? vec2(aTexCoord) : vec2(0.0, 0.0);
   vShadow = uShadowMap * pos;
   vHeightMapHeight = aPosition.y;
   gl_Position = uProjectionMatrix * vViewer;
}
