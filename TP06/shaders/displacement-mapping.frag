#version 330

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

void main() {
  float et      = 10.0;
  vec4 texColor = texture(colormap,uvcoord);
  vec4 texAO = texture(aomap, uvcoord);
  vec4 texSpecular = texture(specularmap, uvcoord);
  vec4 texNormal = texture(normalmap, uvcoord);

  vec3 n = normalize(normalView);
  vec3 e = normalize(eyeView);
  vec3 l = normalize(light);
  vec3 t = normalize(tangentView);

  vec3 b = cross(t,n);
  mat3 tbn = mat3(t, b, n);
  n = tbn * ((texNormal.xyz*2)-1);

  float diff = max(dot(l,n),0.);
  float spec = pow(max(dot(reflect(l,n),e),0.0),et);

  bufferColor = (texColor*(diff + spec)*2.0)*texAO*texSpecular;
}
