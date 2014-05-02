#version 130
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shine;
};
uniform Material uMat;

attribute vec3 aPosition;
attribute vec3 aNormal;

attribute int aBoneID0;
attribute int aBoneWeight0;
attribute int aBoneID1;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vColor;
varying vec4 vViewer;
varying vec3 vNormal;

void main() {
  vec4 position = uModelViewMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  gl_Position = uProjectionMatrix * position;
  if (aBoneID1 == -1 || aBoneID0 == -1) {
     vColor = vec3(1, 1, 1);
  } else {
     vColor = vec3(1, 1, 1);
  }

  vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0));
  vViewer = position;
}
