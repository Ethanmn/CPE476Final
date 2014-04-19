uniform sampler2D uTexUnit;
varying vec2 vTexCoord;

void main() {
   vec4 texColor = texture2D(uTexUnit, vTexCoord);
   gl_FragColor = vec4(texColor[0], texColor[1], texColor[2], 1.0);
}
