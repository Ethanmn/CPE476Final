/*uniform sampler2D uPositionTexture;*/
uniform sampler2D uDiffuseTexture;
uniform sampler2D uNormalTexture;

uniform vec3 uSunDir;
uniform float uSunIntensity;

vec4 calculateDiffuse();

void main() {
   gl_FragColor = calculateDiffuse();
}

vec4 calculateDiffuse() {
   vec4 Diffuse = texture2D(uDiffuseTexture, gl_FragCoord.xy);
   float dotNLDir = dot(normalize(vec3(texture2D(uNormalTexture, gl_FragCoord.xy))), uSunDir);   
   if (dotNLDir < 0.0) dotNLDir = 0.1;
   return vec4(vec3(uSunIntensity), 1.0) * Diffuse.rgba * dotNLDir;
}
