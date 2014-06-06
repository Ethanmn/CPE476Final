uniform sampler2D uShadowMapTexture;
uniform int uHasShadows;
uniform mat4 uShadowMap;
varying vec4 vShadow;

uniform int uOutputShaderType;

uniform int uHasTexture;
uniform sampler2D uTexture;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shine;   
};
uniform Material uMat;

varying vec2 vTexCoord;
varying vec4 vPosition;
varying vec3 vNormal;

vec4 calculateDiffuse();
float calculateShadowAmount();
int alphaCheck();

void main() {
   vec4 color = vec4(1, 0, 0, 1); //red if OutputShaderType is not correct
   int alpha = alphaCheck();

   if(alpha == 0)
      discard;

   if(uOutputShaderType == 0)
      color = calculateDiffuse() * vec4(vec3(calculateShadowAmount()), 1.0);
   else if(uOutputShaderType == 1)
      color = vec4(vec3(gl_FragCoord.z + 0.001), 1.0);
   else if(uOutputShaderType == 2)
      color = vec4(vNormal, 1.0);

   gl_FragColor = color;
}

vec4 calculateDiffuse() {
   vec4 Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord)  : vec4(uMat.diffuse, 1.0);
   return Diffuse;
}

int alphaCheck() {
   vec4 Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord)  : vec4(uMat.diffuse, 1.0);
   if (Diffuse.a < 0.3) {
      return 0;
   }
   return 1;
}

float calculateShadowAmount() {
   float bias = 0.005;
   vec3 directionalColor = vec3(0.8 * 0.3/*sunintensity*/);
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
