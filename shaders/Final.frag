uniform sampler2D uPositionTexture;
uniform sampler2D uDiffuseTexture;
uniform sampler2D uNormalTexture;

uniform vec3 uSunDir;
uniform float uSunIntensity;

varying vec4 vPixelSpacePos;

void main() {
   gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);

}
