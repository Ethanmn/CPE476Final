uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

attribute vec3 aPosition;
attribute vec3 aTexCoord;

varying vec2 vTexCoord;

void main() {
   vec4 vPosition = uModelViewMatrix * vec4(aPosition, 1.0);
   gl_Position = uProjectionMatrix * vPosition;
   vTexCoord = vec2(aTexCoord.x, aTexCoord.y); 
}
