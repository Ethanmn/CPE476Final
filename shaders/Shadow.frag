void main() {
   float depth = gl_FragCoord.z;
   gl_FragColor = vec4(depth - 0.01, depth - 0.01, depth - 0.01, 1.0);
}
