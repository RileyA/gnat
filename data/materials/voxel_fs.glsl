#version 110

varying vec2 v_uv;

uniform sampler2D diffuse;

void main() {
  gl_FragColor = vec4(texture2D(diffuse, v_uv.xy).xyz, 1.0);
}
