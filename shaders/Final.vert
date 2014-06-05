uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
varying vec2 vTexCoord;

attribute vec3 aPosition;
attribute vec3 aTexCoord;

void main() {
   vTexCoord = vec2(aTexCoord);
   gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
}
