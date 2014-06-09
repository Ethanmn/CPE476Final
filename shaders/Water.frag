uniform sampler2D uTexture;

uniform float uScreenWidth;
uniform float uScreenHeight;

void main() {
   vec4 refraction = vec4(0.0, 0, 0.4, 1.0);
   vec4 reflection = texture2D(uTexture, vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight));
   gl_FragColor = (refraction + reflection) / 2.0;
   gl_FragColor = reflection;
}
