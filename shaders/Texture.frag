uniform sampler2D uTexture;
varying vec2 vTexCoord;

uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;
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
   
   dotNLDir = dot(normalize(vNormal), vec3(vLightAndDirectional));
   ReflDir = normalize(reflect(-1.0 * vec3(vLightAndDirectional), vNormal));
   dotVRDir = dot(-1.0 * normalize(vec3(vViewer.x, vViewer.y, vViewer.z)), normalize(ReflDir));
   
   Diffuse = directionalColor * dotNLDir;
   Spec = directionalColor * pow(dotVRDir, 1000.0);
   color =  Diffuse + vec3(vec4(Spec, 1.0) * uViewMatrix) + vec3(0.4);

   gl_FragColor = vec4(color.x * texColor[0],
                       color.y * texColor[1],
                       color.z * texColor[2], 1.0);
}
