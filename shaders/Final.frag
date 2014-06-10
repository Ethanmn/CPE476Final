uniform sampler2D uDiffuseTexture;
uniform sampler2D uPositionTexture;
uniform sampler2D uNormalTexture;

uniform sampler2D uTexture;

uniform mat4 uViewMatrix;
uniform float uScreenWidth;
uniform float uScreenHeight;

uniform vec3 uSunDir;
uniform float uSunIntensity;
uniform int uLightning;
uniform vec3 uGodRayCenter;
uniform int uIsWater;

varying vec4 vNormal;
varying vec4 vPosition;
varying vec2 vTexCoord;

uniform int uIsGodRay;
uniform int uIsFirefly;
varying vec4 vCenter;
varying float vGodRayIntensity;
varying float vGodRayDepth;

vec4 calculateDiffuse(vec2 texCoord, int useSun);
vec4 calculateAmbient(vec2 texCoord, float AmbientAmount);
vec4 checkIfLightning(vec4 Diffuse);
vec4 changeIfWaterPlane();

void main() {
   vec4 color, test, glow = vec4(1.0);
   vec2 pixelOnScreen = vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight);
   vec4 depthOfImage =  texture2D(uPositionTexture, pixelOnScreen);
   float attenuation;

   color = calculateDiffuse(pixelOnScreen, 1);
   if(uIsGodRay == 1) {
      float bias = 0.025;

      bool isVisible = vGodRayDepth <= depthOfImage.z - bias;
      if(isVisible) {
         float dist = abs(distance(vPosition.xyz, vCenter.xyz));
         if(dist < 1.0)
            attenuation = 3.0;
         else {
            attenuation = 1.0 / (3.0 * log(dist));
            attenuation = min(1.0, attenuation);
            test = vec4(vec3(attenuation), 1.0);
            attenuation += 1.0;
         }
         attenuation = max(attenuation, 1.0);
         color = vec4(color.r * attenuation, color.g * attenuation, color.b * attenuation, 1.0); 
      }
      else {
         discard;
      }
   }
   else
      color = calculateDiffuse(pixelOnScreen, 1);

   gl_FragColor = color;
}

vec4 calculateDiffuse(vec2 texCoord, int useSun) {
   float AmbientAmount = 0.23;
   bool noAmbient = false;
   vec3 sunDir = normalize(vec3(uViewMatrix * vec4(uSunDir, 0.0)));
   float sunInt = uSunIntensity;
   if(useSun == 0)
      sunInt = 1.0;

   vec4 Diffuse = texture2D(uDiffuseTexture, texCoord);
   if(Diffuse.r == 0.05098 * uSunIntensity
      && Diffuse.g == 0.6274509 * uSunIntensity
      && Diffuse.b == uSunIntensity) {
      Diffuse = changeIfWaterPlane();
      noAmbient = true;
      if(Diffuse.x < 0.0)
         discard; 
   }

   if(Diffuse.a < 0.8)
      discard;
   float dotNLDir = dot(normalize(vec3(texture2D(uNormalTexture, texCoord))), sunDir);
      
   if (dotNLDir < 0.0) dotNLDir = 0.15;
   
   Diffuse = vec4(vec3(sunInt), 1.0) * Diffuse.rgba * dotNLDir;
  
   if(noAmbient)
      return checkIfLightning(Diffuse); 
   else
      return checkIfLightning(Diffuse) + calculateAmbient(texCoord, AmbientAmount);
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

vec4 changeIfWaterPlane() {
   if(uIsWater == 1) {
      vec4 refraction = vec4(0.0, 0, 0.4, 1.0);
      vec4 reflection = texture2D(uTexture, vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight));
      return (refraction + reflection) / 2.0;
   }
   return vec4(-1);
}
