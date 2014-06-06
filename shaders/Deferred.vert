uniform mat4 uModelViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;
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

uniform mat4 uShadowMap;
varying vec4 vShadow;

varying vec4 vPosition;
varying vec3 vNormal;
varying vec2 vTexCoord;
varying float vUnderWater;

void main() {
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

   vec4 heightColor = vec4(0.0);
#ifdef USE_HEIGHT_MAP
   float HEIGHT_MAP_SCALE = 3.0;
   if (uHasHeightMap != 0) {
      heightColor = vec4(0, texture2D(uHeightMap, aTexCoord.xy).x - 0.5, 0, 0.0) * uHeightMapScale;
   }
#endif

   vPosition = uModelViewMatrix * bone * vec4(heightColor.xyz + aPosition, 1.0);
   vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0));
   vTexCoord = uHasTexture != 0 ? vec2(aTexCoord.x, aTexCoord.y) : vec2(0.0, 0.0);
   vShadow = uShadowMap * uModelMatrix * vec4(aPosition, 1.0);
   vUnderWater = heightColor.y < 0.0 ? heightColor.y * -0.5 - 0.2 : 0.0; 
   gl_Position = uProjectionMatrix * vPosition;

}

