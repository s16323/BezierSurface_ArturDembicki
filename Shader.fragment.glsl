#version 430 core
layout (location = 0) out vec4 color;


uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
} light;

uniform float lambda;

uniform struct Material{
  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
} material;


in TessOut{
    vec3 normal;
    vec3 light_dir;
    vec3 view_dir;
    float dist;
} vertex;

void main(void)
{
    float attenuation = 1.0 /(light.attenuation[0] +
      light.attenuation[1] * vertex.dist +
      light.attenuation[2] * vertex.dist * vertex.dist);

    vec3 normal = normalize(vertex.normal)*lambda;
    vec3 light_dir = normalize(vertex.light_dir);
    vec3 view_dir  = normalize(vertex.view_dir);

    color = material.emission;
    color += material.ambient * light.ambient;
    float n_dot_l = max(dot(normal, light_dir), 0.0);
    color += material.diffuse * light.diffuse * n_dot_l;
    float r_dot_v_pow =
        max(pow(dot(reflect(-light_dir, normal), view_dir), material.shininess), 0.0);
    color += material.specular * light.specular * r_dot_v_pow;
    color *= attenuation;
}
