#version 330

in  vec4 fragmentColor;
in float light;
out vec4 bufferColor;

void main() {
  bufferColor = fragmentColor;
}
