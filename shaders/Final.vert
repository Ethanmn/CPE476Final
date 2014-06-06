uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

uniform sampler2D uPositionTexture;
varying vec2 vTexCoord;

attribute vec3 aPosition;
attribute vec3 aTexCoord;
varying vec4 vPosition;

uniform int uIsGodRay;
uniform vec3 uGodRayCenter;
varying float vGodRayIntensity;

void main() {
   vTexCoord = vec2(aTexCoord);
   vPosition = uModelMatrix * vec4(aPosition, 1.0);
   vec4 pixel_pos = uProjectionMatrix * uViewMatrix * vPosition;

   if(uIsGodRay == 1) {
      pixel_pos.z = 0.0;
      vGodRayIntensity = 1.0 + distance(vec3(vPosition), uGodRayCenter)/40.0;
   }

   gl_Position = pixel_pos;
}
