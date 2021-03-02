#version 330

out vec4 bufferColor;

in vec3 rasterizedNormal; // received (rasterized) normal

uniform vec3 myColor;
uniform float time;

void main() {
  // re-normalize 
  vec3 normal = normalize(rasterizedNormal);
  
  // normal coordinates are used as colors here 
  bufferColor = vec4(sin(time)*normal*0.5+0.5,1.0);

  // color modified by a global variable 
  //bufferColor = vec4(myColor*(normal*0.5+0.5),1.0);
}
