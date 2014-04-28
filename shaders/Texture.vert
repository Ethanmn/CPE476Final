uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
//uniform mat4 uViewMatrix;
//uniform mat4 uNormalMatrix;

attribute vec2 aTexCoord;
attribute vec3 aPosition;

varying vec4 vPosition;
varying vec2 vTexCoord;

void main() {
   gl_Position = uProjectionMatrix * uModelViewMatrix *
         vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
   vTexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
