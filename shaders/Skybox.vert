uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform int uIsTitleScreen;

attribute vec3 aPosition;
attribute vec3 aTexCoord;

varying vec2 vTexCoord;

void main() {
   vec4 vPosition = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
   if(uIsTitleScreen == 1)
      vPosition.z = 0.0;
   gl_Position = vPosition;
   vTexCoord = vec2(aTexCoord.x, aTexCoord.y); 
}
