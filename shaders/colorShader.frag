
#version 140

smooth in vec4 theColor;
out vec4 outputColor;

void main() {
  outputColor = theColor;
//outputColor = vec4(1.0,0.5,0.0,1.0);
}
