uniform int uHasTexture;
uniform sampler2D uTexture;

varying vec3 vView;
varying vec3 vPosition;
varying vec2 vTexCoord;

void main() {
   vec4 refraction = vec4(0.0, 0, 0.1, 1.0);
   vec4 reflection = texture2D(uTexture, vec2(gl_FragCoord.x / 640.0, gl_FragCoord.y / 480.0));
   gl_FragColor = (refraction + reflection) / 2.0;
}
