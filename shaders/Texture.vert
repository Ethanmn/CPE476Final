uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

attribute vec3 aTexCoord;
attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec4 vPosition;
varying vec2 vTexCoord;
varying vec4 vViewer;
varying vec3 vNormal;

void main() {
  vec4 vPosition, light;
   
  vPosition = uModelViewMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  gl_Position = uProjectionMatrix * vPosition;
   
  vNormal = vec3(uNormalMatrix * vec4(aNormal, 1.0));
  vViewer = vPosition;
  vTexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
