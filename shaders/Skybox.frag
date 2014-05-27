uniform sampler2D uTexture;
varying vec2 vTexCoord;

void main() {
   gl_FragColor = vec4(texture2D(uTexture, vTexCoord)); 
}
