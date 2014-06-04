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
   gl_FragData[0] = calculateDiffuse();
   gl_FragData[1] = vPosition;
   gl_FragData[2] = vec4(vNormal, 1.0);
}

vec4 calculateDiffuse() {
   vec4 Diffuse = uHasTexture != 0 ? texture2D(uTexture, vTexCoord)  : vec4(uMat.diffuse, 1.0);
   if (Diffuse.a < 0.3) {
      discard;
   } 
   return Diffuse;
}


