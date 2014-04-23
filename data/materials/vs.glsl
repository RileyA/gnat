#version 110

attribute vec3 position;
attribute vec3 normal;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;
uniform mat4 mvp;

varying vec3 n;

void main() {
  gl_Position = mvp * vec4(position.xyz, 1.0);
  n = normal;
}
