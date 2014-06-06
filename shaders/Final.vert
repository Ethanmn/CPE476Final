uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uModelMatrix;

uniform sampler2D uPositionTexture;
varying vec2 vTexCoord;

attribute vec3 aPosition;
attribute vec3 aTexCoord;

uniform int uIsGodRay;
uniform vec3 uGodRayCenter;
varying float vGodRayIntensity;

void main() {
   vTexCoord = vec2(aTexCoord);
   vec4 pos = vec4(aPosition, 1.0);
   vec4 pixel_pos = uProjectionMatrix * uModelViewMatrix * pos;

   if(uIsGodRay == 1) {
      pixel_pos.z = 0.0;
      vGodRayIntensity = 1.0 + distance(vec3(uModelMatrix * pos), uGodRayCenter)/40.0;
   }

   gl_Position = pixel_pos;
}
