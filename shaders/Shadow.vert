uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
attribute vec3 aPosition;

void main() {
  gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}