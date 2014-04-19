uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

attribute vec3 aPosition;
varying vec4 vPosition;

attribute vec2 aTexCoord;
varying vec2 vTexCoord;

void main() {
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix *
         vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
   vTexCoord = aTexCoord;
}
