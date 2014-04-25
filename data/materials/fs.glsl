#version 110

varying vec3 n;

uniform float brightness;
uniform vec2 offset;

void main() {
  //n = normalize(n);
  gl_FragColor = brightness * vec4((n.x + 1.0) / 2.0 + offset.x, (n.z + 1.0) / 2.0 + offset.y, (n.z + 1.0) / 2.0, 1.0);
}
