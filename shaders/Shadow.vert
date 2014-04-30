uniform mat4 uShadowMVP;
attribute vec3 aPosition;

void main() {
  gl_Position = uShadowMVP * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}