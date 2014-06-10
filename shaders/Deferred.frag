uniform int uOutputShaderType;

uniform sampler2D uShadowMapTexture;
uniform int uHasShadows;
uniform mat4 uShadowMap;
uniform int uHasHeightMap;

uniform int uHasTexture;
uniform sampler2D uTexture;
uniform int uVaryMaterial;

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
varying float vHeightMapHeight;


vec4 calculateDiffuse();
vec4 checkIfUnderWater(vec4 Diffuse);
int alphaCheck();
float calculateShadowAmount();

void main() {
   vec4 color = vec4(1, 0, 0, 1); 
   //red if OutputShaderType is not correct
   int alpha = alphaCheck();

   if(alpha == 0)
      discard;

   if(uOutputShaderType == 0) { 
      color = checkIfUnderWater(calculateDiffuse() * calculateShadowAmount());
   }
   else if(uOutputShaderType == 1)
      color = vec4(vec3(gl_FragCoord.z + 0.001), 1.0);
   else if(uOutputShaderType == 2)
      color = vec4(vNormal, 1.0);

   gl_FragColor = color;
}

vec4 calculateDiffuse() {
   vec4 Diffuse;
   
   if(uVaryMaterial == 1) {
      Diffuse = uHasTexture != 0 ? vec4(vec3(0.7), 1.0) * texture2D(uTexture, vTexCoord) 
                                   + vec4(0.3 * uMat.diffuse,1.0) : vec4(uMat.diffuse, 1.0);
   }
   else {
      Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord) : vec4(uMat.diffuse, 1.0);
   }

   return Diffuse;
}

int alphaCheck() {
   vec4 Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord) : vec4(uMat.diffuse, 1.0);

   if (Diffuse.a < 0.8)
      return 0;
   
   return 1;
}

vec4 checkIfUnderWater(vec4 Diffuse) {
   vec4 DiffuseWithWater = Diffuse;
   float underWater = vHeightMapHeight < 0.1 ? vHeightMapHeight * -0.2 : 0.0; 
   if(uHasHeightMap != 0 && underWater > 0.0) {
      DiffuseWithWater = vec4(0.0, underWater + DiffuseWithWater.y, 
                                   underWater + DiffuseWithWater.z, 1.0);
   }
   return DiffuseWithWater;
}

float calculateShadowAmount() {
   float bias = -0.06;
   vec3 directionalColor = vec3(0.8);
   float applyShadow = 1.0;
   vec4 shadowMapTexColor = vec4(1.0);
   vec3 texCoord = vec3(uShadowMap * vPosition);


   if(uHasShadows != 0) {
      shadowMapTexColor = texture2D(uShadowMapTexture, texCoord.xy);

      //handles if outside of shadowMap texture
      if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0) {
         shadowMapTexColor.z = 1.0;
      }
   }
  
   float differenceInDepth = shadowMapTexColor.z - texCoord.z;
   if(differenceInDepth > bias) {
      differenceInDepth = 1.0;
      applyShadow = 1.2;
   }
   else
      differenceInDepth = 0.0;

   /*gl_FragColor = vec4(vec3(differenceInDepth), 1.0);   */

   return applyShadow;
}
