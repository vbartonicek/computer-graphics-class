#version 140

uniform mat4 PVMmatrix;     // Projection * View * Model --> model to clip coordinates

in vec3 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 texCoord_v; // outgoing vertex texture coordinates

void main() {

  // vertex position after the projection (gl_Position is predefined output variable)
  gl_Position = PVMmatrix * vec4(position, 1);   // outgoing vertex in clip coordinates

  // outputs entering the fragment shader
  texCoord_v = texCoord;

}