#version 110

varying vec3 n;

void main() {
  //n = normalize(n);
  gl_FragColor = vec4((n.x + 1.0) / 2.0, (n.z + 1.0) / 2.0, (n.z + 1.0) / 2.0, 1.0);
}
