uniform sampler2D uTexture;
varying vec2 vTexCoord;
uniform float uSunIntensity;

void main() {
   vec4 texColor = texture2D(uTexture, vTexCoord);
   gl_FragColor = vec4(uSunIntensity * texColor[0],
                       uSunIntensity * texColor[1],
                       uSunIntensity * texColor[2], 1.0);
}
