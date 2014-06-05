/*uniform sampler2D uShadowMapTexture;*/
/*uniform int uHasShadows;*/
/*uniform mat4 uShadowMap;*/
/*varying vec4 vShadow;*/

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

void main() {
   gl_FragData[0] = calculateDiffuse();
   gl_FragData[1] = vPosition;
   gl_FragData[2] = vec4(vNormal, 1.0);
}

vec4 calculateDiffuse() {
   vec4 Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord)  : vec4(uMat.diffuse, 1.0);
   if (Diffuse.a < 0.3) {
      discard;
   }
   Diffuse *= calculateShadowAmount(); 
   return Diffuse;
}

float calculateShadowAmount() {
   return 1.0;
   /*float bias = 0.005;*/
   /*float applyShadow = 1.0;*/
   /*vec4 shadowMapTexColor = vec4(1.0);*/

   /*if(uHasShadows != 0) {*/
      /*shadowMapTexColor = texture2D(uShadowMapTexture, vShadow.xy);*/
      /*//handles if outside of shadowMap texture*/
      /*if(vShadow.x > 1.0 || vShadow.y > 1.0 || vShadow.x < 0.0 || vShadow.y < 0.0)*/
         /*shadowMapTexColor.z = 1.0;*/
   /*}*/
   
   /*if(shadowMapTexColor.z <= vShadow.z - bias)*/
      /*applyShadow = 0.7;*/

   /*return applyShadow;*/
}


