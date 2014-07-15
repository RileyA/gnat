#version 110

varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D gui;

void main() {
  gl_FragColor = texture2D(gui, v_uv.xy);// * v_color;
}
