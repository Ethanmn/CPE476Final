uniform int uHasTexture;
uniform sampler2D uTexture;
varying vec2 vTexCoord;
varying vec4 vPosition;

void checkIfAlpha();
void main() {
   checkIfAlpha(); //discards pixel if alpha < 1.0;
   float depth = gl_FragCoord.z - 0.01;
   gl_FragData[0] = vec4(depth, depth, depth, 1.0);
}

void checkIfAlpha() { 
   vec4 tex = uHasTexture != 0 ? texture2D(uTexture, vTexCoord) : vec4(0, 0, 0, 1);
   if (tex.a < 0.8) {
     /* this alpha value should match the alpha check in texture */
     discard;
   }
}
