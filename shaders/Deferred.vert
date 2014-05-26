uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec4 vPosition;
varying vec3 vNormal;

void main() {
   vPosition = (uModelViewMatrix * vec4(aPosition.xyz, 1.0)).xyz;
   vNormal = (uNormalMatrix * vec4(aNormal, 0.0)).xyz;
   gl_Position = uProjectionMatrix * vec4(vPosition, 1.0);
}
