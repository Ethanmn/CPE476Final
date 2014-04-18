struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};

uniform mat4 uViewMatrix;
uniform Material uMat;
uniform vec3 uSunDir;
uniform float uSunIntensity;

varying vec3 vColor;
varying vec4 vViewer;
varying vec3 vNormal;


void main() {
   vec3 tempSun = vec3(1.5, 1.0, 0.0);
   float tempInt = 0.8;
    vec3 color;
    vec3 Refl, ReflDir;
    vec3 Spec, Diffuse;
    float dotNLDir, dotVRDir;
    vec4 vLightAndDirectional = normalize(vec4(tempSun, 0.0));
    vec3 directionalColor = vec3(tempInt) * vec3(1.0, 1.0, 1.0);

    dotNLDir = dot(normalize(vNormal), vec3(vLightAndDirectional));
    ReflDir = normalize(reflect(-1.0 * vec3(vLightAndDirectional), vNormal));
    dotVRDir = dot(-1.0 * normalize(vec3(vViewer.x, vViewer.y, vViewer.z)),
                normalize(ReflDir));
    
    Diffuse = directionalColor * dotNLDir;
    Spec = directionalColor * pow(dotVRDir, 10.0);
    
    color =  Diffuse + vec3(vec4(Spec, 1.0) * uViewMatrix) + vec3(0.1, 0.1, 0.1);
    
    gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
