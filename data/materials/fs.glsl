#version 110

varying vec3 n;
varying vec2 v_uv;

uniform float brightness;
uniform vec2 offset;
uniform sampler2D ao;

void main() {
  gl_FragColor = vec4(texture2D(ao, v_uv.xy).xyz, 1.0) +
                 0.3 * brightness * n.xyzz;
}
