varying vec4 vPosition;
varying vec3 vNormal;

varying vec3 def_position;
varying vec3 def_normal;

void main() {
   def_position = vPosition;
   def_normal = vNormal;
}
