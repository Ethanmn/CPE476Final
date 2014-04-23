uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
//uniform mat4 uNormalMatrix;

attribute vec2 aTexCoord;
attribute vec3 aPosition;

varying vec4 vPosition;
varying vec2 vTexCoord;

void main() {
   //mat4 normal = uNormalMatrix;
   gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix *
         vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
   vTexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
