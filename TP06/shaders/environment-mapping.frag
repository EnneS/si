#version 330
#define M_PI 3.1415926535897932384626433832795

in  vec3 normalView;
in  vec3 tangentView;
in  vec3 eyeView;
in  vec2 uvcoord;

out vec4 bufferColor;

uniform vec3      light;
uniform sampler2D colormap;
uniform sampler2D aomap;
uniform sampler2D specularmap;
uniform sampler2D normalmap;
uniform sampler2D envmap;

void main() {
  /*
  float et      = 10.0;
  vec4 texColor = texture(colormap,uvcoord);
  vec4 texAO = texture(aomap, uvcoord);
  vec4 texSpecular = texture(specularmap, uvcoord);
  vec4 texNormal = texture(normalmap, uvcoord);
  vec3 l = normalize(light);
  vec3 t = normalize(tangentView);
  */

  vec3 n = normalize(normalView);
  vec3 e = normalize(eyeView);
  vec3 r = reflect(e,n);
  vec2 new_uv;
  new_uv.x = (atan(r.x, r.z)+M_PI)/(2*M_PI);
  new_uv.y = acos(-r.y)/M_PI;
  vec4 texEnv = texture(envmap, new_uv);

  bufferColor = texEnv;
}
