struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shine;
};

attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;
uniform Material uMat;

varying vec3 vColor;
varying vec4 vViewer;
varying vec3 vNormal;

void main() {
  vec4 vPosition;
  /* First model transforms */
  vPosition = uModelMatrix* vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  vPosition = uViewMatrix* vPosition;
  gl_Position = uProjectionMatrix * vPosition;
    
  vNormal = aNormal;
  vViewer = vPosition;

    
}
