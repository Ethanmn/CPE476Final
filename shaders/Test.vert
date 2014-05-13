#version 120
attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uInverseLight;

varying vec3 vNormal;

void main() {
  vec4 vPosition;
  vPosition = uModelViewMatrix* vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  gl_Position = uProjectionMatrix * vPosition;
    
  vNormal = aNormal;
}
