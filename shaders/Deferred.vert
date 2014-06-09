uniform mat4 uModelViewProjectionMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;
uniform int uHasTexture;

#ifdef USE_BONES
uniform int uHasBones;
uniform mat4 uBones[40];
#endif

uniform int uHasHeightMap;
#ifdef USE_HEIGHT_MAP
uniform sampler2D uHeightMap;
uniform float uHeightMapScale;
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

varying vec4 vPosition;
varying vec3 vNormal;
varying vec2 vTexCoord;
varying float vUnderWater;

mat4 calculateBones();
vec3 calculateHeight();

void main() {
   mat4 bone = calculateBones();

   vec4 pos = bone * vec4(calculateHeight() + aPosition, 1.0);
   vPosition = uModelMatrix * pos;
   vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0));
   vTexCoord = uHasTexture != 0 ? vec2(aTexCoord.x, aTexCoord.y) : vec2(0.0, 0.0);

   gl_Position = uModelViewProjectionMatrix * pos;
}

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

vec3 calculateHeight() {
   vec3 heightOffset = vec3(0.0);
#ifdef USE_HEIGHT_MAP
   float HEIGHT_MAP_SCALE = 3.0;
   if (uHasHeightMap != 0) {
      heightOffset = vec3(0, (texture2D(uHeightMap, aTexCoord.xy).x - 0.5) * uHeightMapScale, 0);
   }
   vUnderWater = heightOffset.y < 0.1 ? heightOffset.y * -0.2 : 0.0; 
#endif
   return heightOffset;
}
