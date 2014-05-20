struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shine;
};
uniform Material uMat;

uniform int uHasBones;
uniform mat4 uBones[40];

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

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vColor;
varying vec4 vPosition;
varying vec3 vNormal;

varying vec4 vShadow;
uniform mat4 uShadowMap;

void main() {
  mat4 bone = mat4(1.0);

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

  vec4 position = uModelViewMatrix * bone * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  gl_Position = uProjectionMatrix * position;

  vNormal = vec3(uNormalMatrix * vec4(aNormal, 0.0));
  vPosition = position;
  vShadow = uShadowMap * uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}
