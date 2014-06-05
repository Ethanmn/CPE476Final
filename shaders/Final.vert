uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

uniform sampler2D uPositionTexture;
varying vec2 vTexCoord;

attribute vec3 aPosition;
attribute vec3 aTexCoord;

uniform int uIsGodRay;
uniform vec3 uGodRayCenter;
varying float vGodRayIntensity;

void main() {
   vTexCoord = vec2(aTexCoord);
   vec4 pos = uModelViewMatrix * vec4(aPosition, 1.0);
   pos = uProjectionMatrix * pos;

   if(uIsGodRay == 1) {
      pos.z = 0.0;
      vGodRayIntensity = min(2.0, max(0.0,  distance(vec3(pos), uGodRayCenter) / 10.0));
   }

   gl_Position = pos;
}
