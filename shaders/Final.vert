uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

uniform sampler2D uPositionTexture;
uniform sampler2D uNormalTexture;
uniform float uScreenWidth;
uniform float uScreenHeight;
varying vec2 vTexCoord;

uniform mat4 uNormalMatrix;
attribute vec3 aNormal;
varying vec4 vNormal;

attribute vec3 aPosition;
attribute vec3 aTexCoord;
varying vec4 vPosition;

uniform int uIsGodRay;
uniform vec3 uGodRayCenter;
varying float vGodRayIntensity;
varying float vGodRayDepth;

void main() {
   vTexCoord = vec2(aTexCoord);
   vPosition = uModelMatrix * vec4(aPosition, 1.0);
   vec4 pixel_pos = uProjectionMatrix * uViewMatrix * vPosition;
   vNormal = uNormalMatrix * vec4(aNormal, 1.0);

   if(uIsGodRay == 1) {
      vGodRayDepth = pixel_pos.z / pixel_pos.w;
      pixel_pos.z = 0.0;
      /*vGodRayIntensity = 1.0 + distance(vec3(vPosition), uGodRayCenter)/40.0;*/
      if(vNormal.z > 0.1)
         vGodRayIntensity = max(1.0, 0.5 + 1.0 / vNormal.z);
      else
         vGodRayIntensity = 0.5 + 1.0 / 0.1;
      vGodRayIntensity = min(5.5, vGodRayIntensity); 
   }

   gl_Position = pixel_pos;
}
