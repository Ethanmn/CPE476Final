attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

varying vec3 vNormal;

void main() {
  vec4 vPosition;
  vPosition = uModelMatrix* vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  vPosition = uViewMatrix* vPosition;
  gl_Position = uProjectionMatrix * vPosition;
    
  vNormal = aNormal;
}
