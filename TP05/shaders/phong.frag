#version 330

in  vec3 normalView;
in  vec3 eyeView;
in vec2 texcoord;

out vec4 bufferColor;

uniform vec3 light;
uniform sampler2D maTexture;

void main() {
  // Phong parameters (colors and roughness)
  const vec3 ambient  = vec3(0.3,0.3,0.2);
  const vec3 diffuse  = vec3(0.3,0.5,0.8);
  const vec3 specular = vec3(0.8,0.2,0.2);
  const float et = 50.0;

  // normal / view and light directions (in camera space)
  vec3 n = normalize(normalView);
  vec3 e = normalize(eyeView);
  vec3 l = normalize(light);

  // diffuse and specular components of the phong shading model
  float diff = max(dot(l,n),0.0);
  float spec = pow(max(dot(reflect(l,n),e),0.0),et);

  // pour afficher un rendu phong classique 
  vec3 color = ambient + diff*diffuse + spec*specular;
  // bufferColor = vec4(color,1.0);

  // pour afficher les coordonnées de textures
  //bufferColor = vec4(texcoord,0,1);

  // pour afficher les couleurs contenues dans la texture à ces coordonnées
  bufferColor = texture(maTexture,texcoord);
}

