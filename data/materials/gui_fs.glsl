#version 110

varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D gui;

void main() {
  gl_FragColor = vec4(texture2D(gui, v_uv.xy).xyz, 1.0);// * v_color;
}
