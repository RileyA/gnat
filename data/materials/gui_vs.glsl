#version 110

attribute vec3 position;
attribute vec2 uv;
attribute vec4 color;

varying vec2 v_uv;
varying vec4 v_color;

void main() {
  gl_Position = vec4(position.xyz, 1.0);
  v_uv = uv;
  v_color = color;
}
