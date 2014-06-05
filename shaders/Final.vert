uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uModelMatrix;

uniform mat4 uShadowMap;
varying vec4 vShadow;

attribute vec3 aPosition;

void main() {
   gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
   vShadow = uShadowMap * uModelMatrix * vec4(aPosition, 1.0); 
}
