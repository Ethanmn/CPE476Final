#version 130
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shine;
};
uniform Material uMat;

uniform int uHasBones;
uniform mat4 uBones[100];

attribute vec3 aPosition;
attribute vec3 aNormal;

attribute int aBoneID0;
attribute int aBoneWeight0;
attribute int aBoneID1;
attribute int aBoneWeight1;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vColor;
varying vec4 vViewer;
varying vec3 vNormal;

void main() {
  mat4 bone = mat4(1.0);
  if (uHasBones != 0) {
     if (aBoneID0 != -1) {
        bone = uBones[aBoneID0] * aBoneWeight0;
     }
     if (aBoneID1 != -1) {
        bone += uBones[aBoneID1] * aBoneWeight1;
     }
  }

  vec4 position = uModelViewMatrix * bone * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  gl_Position = uProjectionMatrix * position;

  vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0));
  vViewer = position;
}
