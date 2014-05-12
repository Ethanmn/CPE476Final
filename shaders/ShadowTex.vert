uniform int uHasHeightMap;
uniform sampler2D uHeightMap;
attribute vec3 aTexCoord;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
attribute vec3 aPosition;

void main() {
   vec4 heightColor = vec4(0.0);
   float HEIGHT_MAP_SCALE = 3.0;

   if (uHasHeightMap != 0) {
      heightColor = vec4(0, texture2D(uHeightMap, aTexCoord.xy).x - 0.5, 0, 0.0) * HEIGHT_MAP_SCALE;
   }

   vec4 vPosition = uModelViewMatrix * vec4(heightColor.xyz + aPosition, 1.0);
   gl_Position = uProjectionMatrix * vPosition; 
}