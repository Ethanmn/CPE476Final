/*uniform sampler2D uShadowMapTexture;*/
/*uniform int uHasShadows;*/
/*uniform mat4 uShadowMap;*/
/*varying vec4 vShadow;*/
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

void main() {
   vec4 color = vec4(1, 0, 0, 1); //red if OutputShaderType is not correct

   if(uOutputShaderType == 0)
      color = calculateDiffuse();
   else if(uOutputShaderType == 1)
      color = vPosition;
   else if(uOutputShaderType == 2)
      color = vec4(vNormal, 1.0);

   gl_FragColor = color;
}

vec4 calculateDiffuse() {
   vec4 Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord)  : vec4(uMat.diffuse, 1.0);
   if (Diffuse.a < 0.3) {
      discard;
   }
   return Diffuse;
}
