#version 430 core

layout (quads, equal_spacing, cw) in;


uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
} light;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out TessOut
{
    vec3 normal;
    vec3 light_dir;
    vec3 view_dir;
    float dist;
} vertex;


vec4 bezier(vec4 p0, vec4 p1, vec4 p2, vec4 p3, float t)
{
    vec4 q0, q1, q2, r0, r1;

    q0=mix(p0, p1, t);
    q1=mix(p1, p2, t);
    q2=mix(p2, p3, t);
    r0=mix(q0, q1, t);
    r1=mix(q1, q2, t);
    return mix(r0, r1, t);
}


vec4 evaluate_patch(vec2 at)
{
    vec4 p[4];
    int i;

    for (i = 0; i < 4; i++)
    {
        p[i] = bezier(gl_in[i + 0].gl_Position,
                            gl_in[i + 4].gl_Position,
                            gl_in[i + 8].gl_Position,
                            gl_in[i + 12].gl_Position,
                            at.y);
    }

    return bezier(p[0], p[1], p[2], p[3], at.x);
}

const float epsilon = 0.001;

void main(void){
    vec4 p1 = evaluate_patch(gl_TessCoord.xy);
    vec4 p3 = evaluate_patch(gl_TessCoord.xy + vec2(0.0, epsilon));
    vec4 p2 = evaluate_patch(gl_TessCoord.xy + vec2(epsilon, 0.0));

    vec3 v1 = normalize(p2.xyz/p2.w - p1.xyz/p1.w);
    vec3 v2 = normalize(p3.xyz/p3.w - p1.xyz/p1.w);

    vec4 light_position = view_matrix * light.position;

    vertex.normal = normalize(cross(v1, v2));
    vertex.light_dir = normalize(light_position.xyz - p1.xyz/p1.w);
    vertex.view_dir = normalize(-p1.xyz);
    vertex.dist = distance(light_position, p1);

    gl_Position = projection_matrix * p1;
}
