uniform sampler2D uTexture;

uniform float uScreenWidth;
uniform float uScreenHeight;

varying vec3 vView;
varying vec3 vPosition;
varying vec2 vTexCoord;

void main() {
   vec4 refraction = vec4(0.0, 0, 0.1, 1.0);
   vec4 reflection = texture2D(uTexture, vec2(gl_FragCoord.x / uScreenWidth, gl_FragCoord.y / uScreenHeight));
   gl_FragColor = (refraction + reflection) / 2.0;
}
