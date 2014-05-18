uniform sampler2D uShadowMapTexture;
uniform int uHasShadows;
uniform sampler2D uTexture;
uniform int uLightning;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uShadowMap;
uniform vec3 uSunDir;
uniform float uSunIntensity;

varying vec2 vTexCoord;
varying vec4 vViewer;
varying vec3 vNormal;
varying vec4 vShadow;
varying float below;

void main() {
   vec3 color;
   vec3 Refl, ReflDir;
   vec3 Spec, Diffuse;
   float dotNLDir, dotVRDir, average;
   vec4 vLightAndDirectional = normalize(uViewMatrix * vec4(uSunDir, 0.0));
   vec3 directionalColor = vec3(0.8*uSunIntensity);
   vec4 texColor = texture2D(uTexture, vTexCoord);
   
   vec4 shadowMapTexColor = vec4(1.0);
   if(uHasShadows != 0)
      shadowMapTexColor = texture2D(uShadowMapTexture,
                          vec2(vShadow.x, vShadow.y));
 
   
   float applyShadow = 1.0;
   float bias = 0.005;


   /* temporary material values */
   vec3 amb = vec3(0.1, 0.1, 0.1);
   if(uSunIntensity < 0.2)
      amb = vec3(0.15, 0.15, 0.15);
   vec3 spec = vec3(0.01, 0.01, 0.01);
   float shine = 100.0;
      
   if(shadowMapTexColor.z <= vShadow.z - bias)
      applyShadow = 0.7;
   
   dotNLDir = dot(normalize(vNormal), vec3(vLightAndDirectional));
   ReflDir = normalize(reflect(-1.0 * vec3(vLightAndDirectional), vNormal));
   dotVRDir = dot(-1.0 * normalize(vec3(vViewer.x, vViewer.y, vViewer.z)), ReflDir);
   
   Diffuse = directionalColor * vec3(texColor) * dotNLDir;
   Spec = directionalColor * spec * pow(dotVRDir, shine);
   color =  Diffuse + vec3(vec4(Spec, 1.0) * uViewMatrix) + amb;

   gl_FragColor = vec4(applyShadow * color.rgb, 1.0);
   Spec = directionalColor * spec * pow(dotVRDir, 10.0);
   if(below > 0.0)
      gl_FragColor = vec4(0.0, 
                     uSunIntensity * 0.6 * gl_FragColor.y + applyShadow * 0.2, 
                     uSunIntensity * 0.6 * gl_FragColor.z + applyShadow * 0.4, 
                     1.0);

   if(uLightning != 0) {
      average = 1.2 * (color.r + color.g + color.b) / 3.0;
      average = 0.85;
      gl_FragColor = vec4(average, average, average, 1.0);
   }

}
