uniform sampler2D uDiffuseTexture;
uniform sampler2D uPositionTexture;
uniform sampler2D uNormalTexture;

uniform mat4 uViewMatrix;
uniform float uScreenWidth;
uniform float uScreenHeight;

uniform vec3 uSunDir;
uniform float uSunIntensity;
uniform int uLightning;
varying vec4 vNormal;

varying vec4 vPosition;
varying vec2 vTexCoord;

uniform int uIsGodRay;
uniform int uIsFirefly;
varying vec4 vCenterPos;
varying float vGodRayIntensity;
varying float vGodRayDepth;

vec4 calculateDiffuse(vec2 texCoord, int useSun);
vec4 calculateAmbient(vec2 texCoord, float AmbientAmount);
vec4 checkIfLightning(vec4 Diffuse);

void main() {
   vec4 color, test, glow = vec4(1.0);
   vec2 pixelOnScreen = vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight);
   vec4 depthOfImage =  texture2D(uPositionTexture, pixelOnScreen);
   float AmbientAmount = 0.23;

   color = calculateDiffuse(pixelOnScreen, 1);
   if(uIsGodRay == 1) {

      bool isVisible = vGodRayDepth <= depthOfImage.z;
      if(isVisible) {
         float dist = distance(vPosition.xy, vCenterPos.xy);
         glow *= vec4(vec3(10.0/dist), 1.0);
         if(dist < 1.0) {
            test = vec4(1,0,0,1);
            color + vec4(2.0, 2.0, 0.0, 1.0);
         }
         else  {
            test = vec4(0,1,1,0);
            color + vec4(0.5, 0.5, 0.0, 1.0);
         }
      }
      else {
         discard;
      }
      color = glow;
   }

   gl_FragColor = color + calculateAmbient(pixelOnScreen, AmbientAmount);
}

vec4 calculateDiffuse(vec2 texCoord, int useSun) {
   vec3 sunDir = normalize(vec3(uViewMatrix * vec4(uSunDir, 0.0)));
   float sunInt = uSunIntensity;
   if(useSun == 0)
      sunInt = 1.0;

   vec4 Diffuse = texture2D(uDiffuseTexture, texCoord);
  
   if(Diffuse.r == 0.05098 * uSunIntensity
      && Diffuse.g == 0.6274509 * uSunIntensity
      && Diffuse.b == uSunIntensity)
      discard;
   
   if(Diffuse.a < 0.8)
      discard;
   float dotNLDir = dot(normalize(vec3(texture2D(uNormalTexture, texCoord))), sunDir);
      
   if (dotNLDir < 0.0) dotNLDir = 0.15;
   
   Diffuse = vec4(vec3(sunInt), 1.0) * Diffuse.rgba * dotNLDir;
   return checkIfLightning(Diffuse);
}

vec4 calculateAmbient(vec2 texCoord, float AmbientAmount) {
   vec4 Diffuse = texture2D(uDiffuseTexture, texCoord);
   return vec4(Diffuse.rgb, 1.0) * AmbientAmount;
}


vec4 checkIfLightning(vec4 Diffuse) {
   float LightningAmount;
   if(uLightning != 0) {
      if(uSunIntensity > 0.5)
         LightningAmount = 3.0;
      else
         LightningAmount = 6.0;
      Diffuse = vec4(Diffuse.rgb * LightningAmount, 1.0);
   }
   return Diffuse;
}
