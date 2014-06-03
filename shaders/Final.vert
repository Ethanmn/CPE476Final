uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
attribute vec3 aPosition;

varying vec4 vPixelSpacePos;

void main() {
   vec4 vPosition = uModelViewMatrix * vec4(aPosition, 1.0);
   vPixelSpacePos = uProjectionMatrix * vPosition;
   gl_Position = vPixelSpacePos; 
}
