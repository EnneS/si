#version 330

in vec3 vNormal;
in vec3 vView;
uniform vec3 lightDir; // light direction
out vec4 bufferColor;

void main() {
  vec3 n = normalize(vNormal);
  vec3 v = normalize(vView);
  vec3 l = normalize(lightDir);
  vec3 r = reflect(-l, n);

  vec3 Kd = vec3(0.5, 0.5, 0.5);
  vec3 Ka = vec3(1, 0.3, 0);
  vec3 Ks = vec3(0.5, 0.5, 0.5);

  vec3 couleur = Kd * dot(n, l);
  float es = 50;

  bufferColor = vec4(couleur,1.0) + vec4(Ka,1.0) + vec4(Ks * (pow(dot(r,v),es)), 1.0);
}
