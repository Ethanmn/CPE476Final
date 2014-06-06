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

vec4 calculateDiffuse(vec2 texCoord, int useSun);

void main() {
   vec4 color;
   vec2 pixelOnScreen = vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight);
   vec4 depthOfImage =  texture2D(uPositionTexture, pixelOnScreen);

   if(uIsGodRay == 1) {
      color = calculateDiffuse(pixelOnScreen, 0);

      if(depthOfImage.z - gl_FragCoord.z > 0.0 &&
         depthOfImage.z - gl_FragCoord.z < 10.0) {
         color *= vGodRayIntensity; 
      }
      else
         discard;
   }
   else
      color = calculateDiffuse(pixelOnScreen, 1);

   gl_FragColor = color;
}

vec4 calculateDiffuse(vec2 texCoord, int useSun) {
   vec3 sunDir = uSunDir;
   float sunInt = uSunIntensity;
     
   if(useSun == 0) {
      sunDir = vec3(0.2, 1.0, 0.2);
      sunInt = min(0.9, uSunIntensity + 0.2);
   }

   vec4 Diffuse = texture2D(uDiffuseTexture, texCoord);
   if(Diffuse.a < 0.3)
      discard;
   float dotNLDir = dot(normalize(vec3(texture2D(uNormalTexture, texCoord))), sunDir);
      
   if (dotNLDir < 0.0) dotNLDir = 0.15;
   
   Diffuse = vec4(vec3(sunInt), 1.0) * Diffuse.rgba * dotNLDir;
   return Diffuse;
}
