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
varying vec4 vViewer;
varying vec3 vNormal;


void main() {
   vec3 color;
   vec3 Refl, ReflDir;
   vec3 Spec, Diffuse;
   float dotNLDir, dotVRDir;
   vec4 vLightAndDirectional = normalize(uViewMatrix * vec4(uSunDir, 0.0));
   vec3 directionalColor = vec3(uSunIntensity);
   
   dotNLDir = dot(normalize(vNormal), vec3(vLightAndDirectional));
   ReflDir = normalize(reflect(-1.0 * vec3(vLightAndDirectional), vNormal));
   dotVRDir = dot(-1.0 * normalize(vec3(vViewer.x, vViewer.y, vViewer.z)), normalize(ReflDir));
   
   Diffuse = directionalColor * uMat.diffuse * dotNLDir;
   Spec = directionalColor * uMat.specular * pow(dotVRDir, uMat.shine);
   color =  Diffuse + vec3(vec4(Spec, 1.0) * uViewMatrix) + uMat.ambient;
   
   gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
   /*gl_FragColor = vec4(color.r * vColor.r, color.g * vColor.g, color.b * vColor.b, 1.0);*/
}
