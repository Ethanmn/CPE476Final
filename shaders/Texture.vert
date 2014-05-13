#version 120
uniform int uHasHeightMap;
/*uniform sampler2D uHeightMap;*/

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uShadowMap;

uniform int uHasBones;
uniform mat4 uBones[100];

attribute vec3 aShadowTexCoord;
attribute vec3 aTexCoord;
attribute vec3 aPosition;
attribute vec3 aNormal;

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

varying vec4 vPosition;
varying vec2 vTexCoord;
varying vec4 vViewer;
varying vec3 vNormal;
varying vec4 vShadow;
varying float below;

void main() {
   mat4 bone = mat4(1.0);
   if (uHasBones != 0) {
      if (aBoneID0 != -1) {
         bone = uBones[int(aBoneID0)] * aBoneWeight0;
      }
      if (aBoneID1 != -1) {
         bone += uBones[int(aBoneID1)] * aBoneWeight1;
      }
      if (aBoneID2 != -1) {
         bone += uBones[int(aBoneID2)] * aBoneWeight2;
      }
      if (aBoneID3 != -1) {
         bone += uBones[int(aBoneID3)] * aBoneWeight3;
      }
      if (aBoneID4 != -1) {
         bone += uBones[int(aBoneID4)] * aBoneWeight4;
      }
   }

   vec4 heightColor = vec4(0.0);
   float HEIGHT_MAP_SCALE = 3.0;
   if (uHasHeightMap != 0) {
      /*heightColor = vec4(0, texture2D(uHeightMap, aTexCoord.xy).x - 0.5, 0, 0.0) * HEIGHT_MAP_SCALE;*/
   }

   if(heightColor.y < 0.0)
      below = 1.0;
   else
      below = 0.0;

   vPosition = uModelViewMatrix * bone * vec4(heightColor.xyz + aPosition, 1.0);

   vViewer = vPosition;
   gl_Position = uProjectionMatrix * vPosition;


   vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0));
   vTexCoord = vec2(aTexCoord.x, aTexCoord.y);
   vShadow = uShadowMap * uModelMatrix * vec4(heightColor.xyz + aPosition, 1.0);
}
