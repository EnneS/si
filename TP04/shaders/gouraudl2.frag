#version 330

in vec3 vNormal;
uniform vec3 lightDir; // light direction
out vec4 bufferColor;

void main() {
  vec3 n = normalize(vNormal);
  vec3 l = normalize(lightDir);
  vec3 Kd = vec3(1, 0.3, 0);
  vec3 couleur = Kd * dot(n, l);

  vec4 fragmentColor = vec4(couleur,1.0);

  bufferColor = fragmentColor;
}
