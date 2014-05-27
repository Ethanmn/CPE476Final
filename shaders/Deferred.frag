uniform int uOutputShaderType;

uniform sampler2D uShadowMapTexture;
uniform int uHasShadows;
uniform mat4 uShadowMap;

uniform int uHasTexture;
uniform sampler2D uTexture;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shine;
};
uniform Material uMat;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

uniform int uUseBlinnPhong;
uniform vec3 uSunDir;
uniform float uSunIntensity;
uniform int uLightning;

varying vec4 vPosition;
varying vec2 vTexCoord;
varying vec4 vViewer;
varying vec3 vNormal;
varying vec4 vShadow;
varying float vUnderWater;

float calculateShadowAmount();
vec3 calculateDiffuse(vec3 lightInt, vec3 lightDir);
vec3 calculateSpecular(vec3 lightInt, vec3 lightDir);
void CheckIfUnderWater(float ShadowAmount);
void CheckIfLightning();



void main() {
   vec3 color;
   vec3 Specular, Diffuse, Ambient;
   vec3 directionalColor = vec3(0.8 * uSunIntensity);
   float ShadowAmount, LightningAmount;
   vec4 vLightAndDirectional = normalize(uViewMatrix * vec4(uSunDir, 0.0)); 
       
   ShadowAmount = calculateShadowAmount();
   Ambient = uSunIntensity < 0.2 ? vec3(0.15, 0.15, 0.15) : vec3(0.1, 0.1, 0.1);
   Diffuse = calculateDiffuse(directionalColor, vLightAndDirectional.xyz);
   Specular = calculateSpecular(directionalColor, vLightAndDirectional.xyz);

   color =  Diffuse + Specular + Ambient;
   gl_FragColor = vec4(ShadowAmount * color.rgb, 1.0);

   CheckIfUnderWater(ShadowAmount);
   CheckIfLightning();

   if(uOutputShaderType == 1)
      gl_FragColor = vPosition;
   else if(uOutputShaderType == 2)
      gl_FragColor = vec4(ShadowAmount * Diffuse.xyz, 1.0);
   else if(uOutputShaderType == 3)
      gl_FragColor = vec4(vNormal, 1.0);
    
}



float calculateShadowAmount() {
   float bias = 0.005;
   vec3 directionalColor = vec3(0.8 * uSunIntensity);
   float applyShadow = 1.0;
   vec4 shadowMapTexColor = vec4(1.0);

   if(uHasShadows != 0) {
      shadowMapTexColor = texture2D(uShadowMapTexture, vShadow.xy);

      //handles if outside of shadowMap texture
      if(vShadow.x > 1.0 || vShadow.y > 1.0 || vShadow.x < 0.0 || vShadow.y < 0.0)
         shadowMapTexColor.z = 1.0;
   }
   
   if(shadowMapTexColor.z <= vShadow.z - bias)
      applyShadow = 0.7;

   return applyShadow;
}

vec3 calculateDiffuse(vec3 lightInt, vec3 lightDir) {
   vec4 Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord) : vec4(uMat.diffuse, 1);
   if (Diffuse.a < 0.3) {
      discard;
   }

   float dotNLDir = dot(normalize(vNormal), lightDir);   
   return lightInt * Diffuse.rgb * dotNLDir;
}

vec3 calculateSpecular(vec3 lightInt, vec3 lightDir) {
   float dotVRDir, blinnValue;
   vec3 halfAngle, ReflDir, Specular = vec3(0.01, 0.01, 0.01);

   if(uUseBlinnPhong != 0) {
      halfAngle = normalize(lightDir + -1.0 * vec3(vViewer));
      blinnValue = pow(dot(halfAngle, vNormal), 0.1);
      Specular = lightInt * Specular * blinnValue;
   }
   else {
      ReflDir = normalize(reflect(-1.0 * lightDir, vNormal));
      dotVRDir = dot(-1.0 * normalize(vViewer.xyz), ReflDir);
      Specular = lightInt * Specular * pow(dotVRDir, 30.0);
   }

   return vec3(vec4(Specular, 1.0) * uViewMatrix);
}

void CheckIfUnderWater(float ShadowAmount) {
   if(vUnderWater > 0.0) {
      gl_FragColor = vec4(0.0, 
                     uSunIntensity * 0.6 * gl_FragColor.y + ShadowAmount * 0.2, 
                     uSunIntensity * 0.6 * gl_FragColor.z + ShadowAmount * 0.4, 
                     1.0);
   }
}

void CheckIfLightning() {
   float LightningAmount;
   if(uLightning != 0) {
      if(uSunIntensity > 0.5)
         LightningAmount = 2.0;
      else
         LightningAmount = 4.0;
      gl_FragColor = vec4(gl_FragColor.rgb * LightningAmount, 1.0);
   }
}
