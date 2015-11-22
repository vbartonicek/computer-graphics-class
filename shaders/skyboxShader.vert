#version 140

uniform mat4 inversePVmatrix;
in vec2 screenCoord;
out vec3 texCoord_v;

void main() {
  vec4 farplaneCoord = vec4(screenCoord, 0.9999, 1.0);
  vec4 worldViewCoord = inversePVmatrix * farplaneCoord;
  texCoord_v = worldViewCoord.xyz / worldViewCoord.w;
  gl_Position = farplaneCoord;
}