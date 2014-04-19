uniform sampler2D uTexture;

varying vec2 vTexCoord;

void main() {
   vec4 texColor = texture2D(uTexture, vTexCoord);
   gl_FragColor = vec4(texColor[0], texColor[1], texColor[2], 1.0);
}
