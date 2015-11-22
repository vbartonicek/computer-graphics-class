#version 140

uniform mat4 PVMmatrix;     // Projection * View * Model --> model to clip coordinate
uniform float time;

in vec3 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 texCoord_v; // outgoing vertex texture coordinates

float decay=0.025f;

void main() {

  // vertex position after the projection (gl_Position is predefined output variable)
  gl_Position = PVMmatrix * vec4(position, 1);   // outgoing vertex in clip coordinates

  float localTime= time*decay;

  vec2 offset=vec2((floor(localTime)-localTime)*4.0f+1.0f,0.0f);

  // outputs entering the fragment shader
  texCoord_v = texCoord+offset;

}