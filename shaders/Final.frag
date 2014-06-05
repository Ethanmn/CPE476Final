uniform sampler2D uDiffuseTexture;
uniform sampler2D uPositionTexture;
uniform sampler2D uNormalTexture;
uniform float uScreenWidth;
uniform float uScreenHeight;

uniform vec3 uSunDir;
uniform float uSunIntensity;
uniform int uIsGodRay;

varying vec2 vTexCoord;
varying float vGodRayIntensity;

vec4 calculateDiffuse(vec2 texCoord);

void main() {
   vec4 color;
   vec2 pixelOnScreen = vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight);
   vec4 depthOfImage =  texture2D(uPositionTexture, pixelOnScreen);

   if(uIsGodRay == 1) {
      color = calculateDiffuse(pixelOnScreen);

      if(depthOfImage.z - gl_FragCoord.z > 0.0) {
         color *= vGodRayIntensity; 
      }
      else
         discard;
   }
   else
      color = calculateDiffuse(pixelOnScreen);

   gl_FragColor = color;
}

vec4 calculateDiffuse(vec2 texCoord) {
   vec4 Diffuse = texture2D(uDiffuseTexture, texCoord);
   if(Diffuse.a < 0.3)
      discard;
   float dotNLDir = dot(normalize(vec3(texture2D(uNormalTexture, texCoord))), uSunDir);   
   if (dotNLDir < 0.0) dotNLDir = 0.1;
   return vec4(vec3(uSunIntensity), 1.0) * Diffuse.rgba * dotNLDir;
}
