#version 110

attribute vec3 position;
attribute vec2 uv;

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;
uniform mat4 mvp;

varying vec2 v_uv;

void main() {
  gl_Position = mvp * vec4(position.xyz, 1.0);
  v_uv = uv;
}
