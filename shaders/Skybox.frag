uniform sampler2D uTexture;
varying vec2 vTexCoord;

void main() {
   gl_FragColor = vec4(texture2D(uTexture, vTexCoord));// * vec4(0.5f, 0.5f, 0.5f, 1.0f); 
}
