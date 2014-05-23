uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

attribute vec3 aPosition;
attribute vec3 aTexCoord;

varying vec3 vView;
varying vec2 vTexCoord;

void main() {
   vView = -(uModelViewMatrix * vec4(aPosition, 1.0)).xyz;
   vTexCoord = aTexCoord.xy;
   gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aPosition, 1.0);
}
