#version 330

// input attributes 
layout(location = 0) in vec3 position;// position of the vertex in world space
layout(location = 1) in vec3 normal;  // normal of the vertex
layout(location = 2) in vec3 tangent; // tangent of the vertex
layout(location = 3) in vec2 coord;   // uv-coord of the vertex 

uniform mat4 mdvMat; // modelview matrix (constant for all the vertices)
uniform mat4 projMat; // projection matrix (constant for all the vertices)
uniform mat3 normalMat; // normal matrix (constant for all the vertices)

// output variables that will be interpolated during rasterization (equivalent to varying)
out vec3 normalView;
out vec3 tangentView;
out vec3 eyeView;
out vec2 uvcoord;
uniform sampler2D normalmap;
uniform sampler2D heightmap;

void main() {
  uvcoord     = coord*10.0;
  normalView  = normalize(normalMat*normal);
  tangentView = normalize(normalMat*tangent);
  eyeView     = normalize((mdvMat*vec4(position,1.0)).xyz);

  vec3 n = normalize(normalView);
  vec3 t = normalize(tangentView);

  float texHeight = (texture(heightmap, uvcoord).x);
  vec3 p = position + texHeight*normal*0.05;
  gl_Position = projMat*mdvMat*vec4(p,1.0);
}
