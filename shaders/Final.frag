/*uniform sampler2D uPositionTexture;*/
uniform sampler2D uDiffuseTexture;
uniform sampler2D uNormalTexture;

uniform sampler2D uShadowMapTexture;
uniform int uHasShadows;
varying vec4 vShadow; 

uniform vec3 uSunDir;
uniform float uSunIntensity;

vec4 calculateDiffuse();
float calculateShadowAmount();

void main() {
   gl_FragColor = calculateDiffuse() * calculateShadowAmount();
}

vec4 calculateDiffuse() {
   vec4 Diffuse = texture2D(uDiffuseTexture, gl_FragCoord.xy / 2.0);
   float dotNLDir = dot(normalize(vec3(texture2D(uNormalTexture, gl_FragCoord.xy / 2.0))), uSunDir);   
   if (dotNLDir < 0.0) dotNLDir = 0.1;
   return vec4(vec3(uSunIntensity), 1.0) * Diffuse.rgba * dotNLDir;
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
