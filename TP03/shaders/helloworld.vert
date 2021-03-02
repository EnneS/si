#version 330

// starting from OpenGL 3.3 it is possible to use this syntax to make the relations with arrays
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 mvp; // modelview projection matrix (constant for all the vertices)

uniform float time;

out vec3 rasterizedNormal; // normal (will be rasterized from vertex to fragment shader)

void main() {
  vec3 pos = position;

  pos.x = pos.x + exp(pos.y*time);
  gl_Position = mvp*vec4(pos,1.0);
  rasterizedNormal = normal;

}
