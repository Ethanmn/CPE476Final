<<<<<<< HEAD
//#version 120
=======
>>>>>>> 287a7b506543a8b09061d9ea47b6630d76373352
attribute vec3 aPosition;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

void main() {
  gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
}
