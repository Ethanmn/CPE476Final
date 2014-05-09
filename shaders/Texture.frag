uniform sampler2D uTexture;
varying vec2 vTexCoord;

uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uShadowMapInverse;
uniform vec3 uSunDir;
uniform float uSunIntensity;

varying vec4 vViewer;
varying vec3 vNormal;

void main() {
   
   vec4 texColor = texture2D(uTexture, vTexCoord);

   vec3 color;
   vec3 Refl, ReflDir;
   vec3 Spec, Diffuse;
   float dotNLDir, dotVRDir;
   vec4 vLightAndDirectional = normalize(uViewMatrix * vec4(uSunDir, 0.0));
   vec3 directionalColor = vec3(0.7*uSunIntensity);
   
   /* temporary material values */
   vec3 amb = vec3(0.1, 0.1, 0.1);
   vec3 spec = vec3(0.01, 0.01, 0.01);
   float shine = 100.0;
   
   dotNLDir = dot(normalize(vNormal), vec3(vLightAndDirectional));
   ReflDir = normalize(reflect(-1.0 * vec3(vLightAndDirectional), vNormal));
   dotVRDir = dot(-1.0 * normalize(vec3(vViewer.x, vViewer.y, vViewer.z)), ReflDir);
   
   Diffuse = directionalColor * vec3(texColor) * dotNLDir;
   Spec = directionalColor * spec * pow(dotVRDir, shine);
   color =  Diffuse + vec3(vec4(Spec, 1.0) * uViewMatrix) + amb;

   gl_FragColor = vec4(color.r, color.g, color.b, 1.0);

}
