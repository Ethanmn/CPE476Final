uniform int uOutputShaderType;

/*uniform sampler2D uShadowMapTexture;*/
/*uniform int uHasShadows;*/
/*uniform mat4 uShadowMap;*/

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
varying float vUnderWater;

vec4 calculateDiffuse();
vec4 checkIfUnderWater(vec4 Diffuse, float ShadowAmount);
int alphaCheck();
float calculateShadowAmount();

void main() {
   vec4 color = vec4(1, 0, 0, 1); //red if OutputShaderType is not correct
   int alpha = alphaCheck();

   if(alpha == 0)
      discard;

   if(uOutputShaderType == 0) { 
      color = checkIfUnderWater(calculateDiffuse(), 0.0);
   }
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
   if (Diffuse.a < 0.8) {
      return 0;
   }
   return 1;
}

vec4 checkIfUnderWater(vec4 Diffuse, float ShadowAmount) {
   vec4 DiffuseWithWater = Diffuse;
   if(vUnderWater > 0.0)
      DiffuseWithWater = vec4(0.0, vUnderWater * DiffuseWithWater.y + ShadowAmount * 0.2, 
                              vUnderWater * DiffuseWithWater.z + ShadowAmount * 0.4, 1.0);
   return DiffuseWithWater;
}

float calculateShadowAmount() {
   return 1.0;
   /*float bias = 0.005;*/
   /*vec3 directionalColor = vec3(0.8 * 0.3[>sunintensity<]);*/
   /*float applyShadow = 1.0;*/
   /*vec4 shadowMapTexColor = vec4(1.0);*/
   /*vec2 texCoord = vec2(uShadowMap * vPosition);*/

   /*if(uHasShadows != 0) {*/
      /*shadowMapTexColor = texture2D(uShadowMapTexture, texCoord);*/
      /*if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)*/
         /*shadowMapTexColor.z = 1.0;*/
   /*}*/
   /*if(shadowMapTexColor.z <= gl_FragCoord.z - bias)*/
      /*applyShadow = shadowMapTexColor.x;*/
   
   /*return applyShadow;*/
}
