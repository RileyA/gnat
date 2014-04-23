#version 110

varying vec3 n;

void main() {
  //n = normalize(n);
  gl_FragColor = vec4(n.xyz, 1.0);
}
