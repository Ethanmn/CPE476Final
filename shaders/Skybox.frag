uniform sampler2D uTexture;
varying vec2 vTexCoord;

void main() {
   vec4 texture = vec4(texture2D(uTexture, vTexCoord));
   if(texture.a < 0.5 || texture.r < 0.1 && texture.g < 0.1 && texture.b < 0.1)
      discard;
   gl_FragColor = texture;
}
