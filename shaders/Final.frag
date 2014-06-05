uniform sampler2D uDiffuseTexture;
/*uniform sampler2D uPositionTexture;*/
uniform sampler2D uNormalTexture;

uniform vec3 uSunDir;
uniform float uSunIntensity;

varying vec2 vTexCoord;

vec4 calculateDiffuse();

void main() {
   gl_FragColor = calculateDiffuse();
}

vec4 calculateDiffuse() {
   vec4 Diffuse = texture2D(uDiffuseTexture, vTexCoord);
   float dotNLDir = dot(normalize(vec3(texture2D(uNormalTexture, vTexCoord))), uSunDir);   
   if (dotNLDir < 0.0) dotNLDir = 0.1;
   return vec4(vec3(uSunIntensity), 1.0) * Diffuse.rgba * dotNLDir;
}
