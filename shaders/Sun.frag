uniform sampler2D uShadowMapTexture;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shine;
};
uniform Material uMat;

uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;
uniform vec3 uSunDir;
uniform float uSunIntensity;

varying vec3 vColor;
varying vec4 vPosition;
varying vec3 vNormal;
varying vec4 vShadow;

void main() {
   vec3 color;
   vec3 Refl, ReflDir, amb = uMat.ambient;
   vec3 Spec, Diffuse;
   float dotNLDir, dotVRDir;
   vec4 vLightAndDirectional = normalize(uViewMatrix * vec4(uSunDir, 0.0));
   vec3 directionalColor = vec3(0.7*uSunIntensity);
   vec4 shadowMapTexColor = texture2D(uShadowMapTexture, 
                          vec2(vShadow.x, vShadow.y));
   float applyShadow = 1.0;
   float bias = 0.001;
   if(shadowMapTexColor.z < vShadow.z - bias)
      applyShadow = 1.5 * shadowMapTexColor.x;

   if(uSunIntensity < 0.35)
      amb = vec3(0.1, 0.1, 0.1);
   
   dotNLDir = dot(normalize(vNormal), vec3(vLightAndDirectional));
   ReflDir = normalize(reflect(-1.0 * vec3(vLightAndDirectional), vNormal));
   dotVRDir = dot(-1.0 * normalize(vec3(vPosition.x, vPosition.y, vPosition.z)), 
      normalize(ReflDir));
   
   Diffuse = directionalColor * uMat.diffuse * dotNLDir;
   Spec = directionalColor * uMat.specular * pow(dotVRDir, uMat.shine) * vec3(0.0);
   color =  Diffuse + vec3(vec4(Spec, 1.0) * uViewMatrix) + vec3(1.2) * amb;
   
   gl_FragColor = vec4(applyShadow * color.rgb, 1.0);
}
