uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

attribute vec2 aTexCoord;
attribute vec3 aPosition;

varying vec4 vPosition;
varying vec2 vTexCoord;

void main() {
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix *
         vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
   vTexCoord = aTexCoord;
}
