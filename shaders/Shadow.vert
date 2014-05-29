uniform mat4 uModelViewProjectionMatrix;
attribute vec3 aPosition;

void main() {
   gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);
}
