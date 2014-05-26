uniform sampler2D uShadowMapTexture;
uniform int uHasShadows;
uniform mat4 uShadowMap;

uniform int uHasTexture;
uniform sampler2D uTexture;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shine;
};
uniform Material uMat;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uNormalMatrix;

uniform int uUseBlinnPhong;
uniform vec3 uSunDir;
uniform float uSunIntensity;
uniform int uLightning;

varying vec2 vTexCoord;
varying vec4 vViewer;
varying vec3 vNormal;
varying vec4 vShadow;
varying float vUnderWater;

void main() {
   vec3 color;
   vec3 Refl, ReflDir;
   vec3 Specular, Diffuse, Ambient, halfAngle;
   float lightningAmount, dotNLDir, dotVRDir, average, blinnValue, shine = 30.0;
   vec4 vLightAndDirectional = normalize(uViewMatrix * vec4(uSunDir, 0.0));
   vec3 directionalColor = vec3(0.8 * uSunIntensity);
   float applyShadow = 1.0;
   float bias = 0.005;
   
   vec4 shadowMapTexColor = vec4(1.0);
   if(uHasShadows != 0) {
      shadowMapTexColor = texture2D(uShadowMapTexture,
                          vec2(vShadow.x, vShadow.y));
      if(vShadow.x > 1.0 || vShadow.y > 1.0 || vShadow.x < 0.0 || vShadow.y < 0.0) {
         shadowMapTexColor.z = 1.0;
      }

   }
   if(shadowMapTexColor.z <= vShadow.z - bias)
      applyShadow = 0.7;
   
   vec4 texColor = vec4(1.0);
   if(uHasTexture != 0) {
      texColor = texture2D(uTexture, vTexCoord);
      Diffuse = texColor.xyz;
      Ambient = vec3(0.1, 0.1, 0.1);
      Specular = vec3(0.01, 0.01, 0.01);
   }
   else {
      Diffuse = uMat.diffuse;
      Ambient = uMat.ambient;
      Specular = vec3(0.01, 0.01, 0.01); 
   }

   if(uSunIntensity < 0.2)
      Ambient = vec3(0.15, 0.15, 0.15);
  
   dotNLDir = dot(normalize(vNormal), vec3(vLightAndDirectional));   
   Diffuse = directionalColor * Diffuse * dotNLDir;

   if(uUseBlinnPhong != 0) {
      halfAngle = normalize(vec3(vLightAndDirectional) + -1.0 * vec3(vViewer));
      blinnValue = pow(dot(halfAngle, vNormal), 0.1);
      Specular = directionalColor * Specular * blinnValue;
   }
   else {
      ReflDir = normalize(reflect(-1.0 * vec3(vLightAndDirectional), vNormal));
      dotVRDir = dot(-1.0 * normalize(vec3(vViewer.x, vViewer.y, vViewer.z)), ReflDir);
      Specular = directionalColor * Specular * pow(dotVRDir, shine);
   }

   color =  Diffuse + vec3(vec4(Specular, 1.0) * uViewMatrix) + Ambient;

   gl_FragColor = vec4(applyShadow * color.rgb, 1.0);

   if(vUnderWater > 0.0) {
      gl_FragColor = vec4(0.0, 
                     uSunIntensity * 0.6 * gl_FragColor.y + applyShadow * 0.2, 
                     uSunIntensity * 0.6 * gl_FragColor.z + applyShadow * 0.4, 
                     1.0);
   }

   if(uLightning != 0) {
      if(uSunIntensity > 0.5)
         lightningAmount = 2.0;
      else
         lightningAmount = 4.0;
      gl_FragColor = vec4(gl_FragColor.rgb * lightningAmount, 1.0);
   }

}
