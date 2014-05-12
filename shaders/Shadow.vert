#version 130
uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
attribute vec3 aPosition;

void main() {
   vec4 vPosition = uModelViewMatrix * vec4(aPosition, 1.0);
   gl_Position = uProjectionMatrix * vPosition; 
}
