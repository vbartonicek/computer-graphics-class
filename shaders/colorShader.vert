#version 140

uniform mat4 PVMmatrix;
in vec3 position;
in vec3 color;
smooth out vec4 theColor;


void main() {
  gl_Position = PVMmatrix * vec4(position, 1.0);
  //theColor = vec4(color,1.0) * (gl_VertexID % 256)/255;
  theColor = vec4(1.0,1.0,0.6,1.0) * (gl_VertexID % 256)/255;
	//theColor = vec4(color, 1.0);
}