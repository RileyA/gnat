#version 110

attribute vec3 position;
attribute vec3 normal;

varying vec3 n;

void main() {
  gl_Position = vec4(position.xyz, 1.0);
  n = normal;
}
