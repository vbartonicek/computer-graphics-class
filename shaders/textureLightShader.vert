#version 140


in vec3 position;           // vertex position in world space
in vec3 normal;             // vertex normal
in vec2 texCoord;           // incoming texture coordinates

uniform mat4 PVMmatrix;     // Projection * View * Model  --> model to clip coordinates
uniform mat4 Vmatrix;       // View                       --> world to eye coordinates
uniform mat4 Mmatrix;       // Model                      --> model to world coordinates
uniform mat4 normalMatrix;  // inverse transposed VMmatrix

smooth out vec3 vertexPosition;	// outgoing vertex position
smooth out vec3 vertexNormal;	// outcoing vertex normal
smooth out vec2 vertexTexCoord;	// outgoing texture coordinates
out vec4 viewSpace;



void main() {


  // eye-coordinates position and normal of vertex
  vertexPosition = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;         // vertex in eye coordinates
  vertexNormal   = normalize( (normalMatrix * vec4(normal, 0.0) ).xyz);   // normal in eye coordinates by NormalMatrix
  vertexTexCoord=texCoord;

  // vertex position after the projection (gl_Position is built-in output variable)
  gl_Position = PVMmatrix * vec4(position, 1);   // out:v vertex in clip coordinates


  viewSpace = Vmatrix * Mmatrix * vec4(position,1);

}
