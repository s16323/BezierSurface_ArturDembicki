#version 430
layout(location=0) in vec4 in_position;

uniform mat4 model_matrix;
uniform mat4 view_matrix;


void main(void){
    gl_Position = (view_matrix * model_matrix) * in_position;
}
