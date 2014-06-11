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
varying vec4 vHeight;
varying vec4 vMVPPos;

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
   float attenuation = 1.5;

   color = calculateDiffuse(pixelOnScreen, 1);
   if(uIsGodRay == 1) {
      float bias = 0.005;
      float fireflyBias = 0.025;
      float dist;

      bool isVisible = vGodRayDepth <= depthOfImage.z - bias;
      bool fireflyIsVisible = vGodRayDepth <= depthOfImage.z - fireflyBias;

      if(uIsFirefly == 1 && fireflyIsVisible) {
         dist = abs(distance(vPosition.xyz, vCenter.xyz));
            if(dist < 1.0)
               attenuation = 3.0;
            else {
               attenuation = 1.0 + 1.0 / (2.0 * log(dist));
               attenuation = max(attenuation, 1.0);
            }
      }
      else if(uIsFirefly == 0 && isVisible) {
            dist = abs(vCenter.x - vMVPPos.x); 
            float xDist = abs(vCenter.x - vMVPPos.x);
            float yDist = abs(vHeight.y + vMVPPos.y) / (2.0 * vHeight.y);

            if(dist < 1.0)
               dist = 1.0;
            test = vec4(vec3(log(dist)), 1.0);
            dist = min(2.0, 1.0 + 1.0 / (3.0 * log(dist)));
            attenuation = max(-1.0, dist / yDist);
            attenuation = max(attenuation, 1.0);
      }
      else
         discard;

      color = vec4(color.r * attenuation, color.g * attenuation, color.b * attenuation, 1.0); 
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

   if(Diffuse.r == 0.05098
      && Diffuse.g == 0.6274509
      && Diffuse.b == 1.0 ||
      Diffuse.r == 0.05098 * 0.5
      && Diffuse.g == 0.6274509 * 0.5
      && Diffuse.b == 1.0 * 0.5) {
      Diffuse = changeIfWaterPlane();
      noAmbient = true;
      if(Diffuse.r < 0.0)
         discard; 
   }
   else if(Diffuse.a < 0.8)
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
      vec4 refraction = vec4(0.0, 0.2, 0.2, 1.0);
      vec4 reflection = texture2D(uTexture, vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight));
      return (refraction + reflection) / 2.0;
   }
   return vec4(-1);
}
