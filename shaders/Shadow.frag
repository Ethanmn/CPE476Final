uniform int uHasTexture;
uniform sampler2D uTexture;
varying vec2 vTexCoord;

void checkIfAlpha();
void main() {
   checkIfAlpha(); //discards pixel if alpha < 1.0;
   float depth = gl_FragCoord.z;
   gl_FragData[0] = vec4(depth - 0.01, depth - 0.01, depth - 0.01, 1.0);
}

void checkIfAlpha() { 
   vec4 tex = uHasTexture != 0 ? texture2D(uTexture, vTexCoord) : vec4(0, 0, 0, 1);
   if (tex.a < 0.7) {
     /* this alpha value should match the alpha check in texture */
     discard;
   }
}
