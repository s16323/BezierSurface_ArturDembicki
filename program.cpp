#include "program.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "glerror.h"

using namespace std;


void Program::Initialize(const char* vertex_shader_file,
                         const char* fragment_shader_file,
                         const char* tess_control_shader_file,
                         const char* tess_evaluation_shader_file){
    vertex_shader_ = LoadAndCompileShaderOrDie(vertex_shader_file,
                                               GL_VERTEX_SHADER);
    fragment_shader_ = LoadAndCompileShaderOrDie(fragment_shader_file,
                                                 GL_FRAGMENT_SHADER);
    tess_control_shader_ = LoadAndCompileShaderOrDie(tess_control_shader_file,
                                                 GL_TESS_CONTROL_SHADER);
    tess_evaluation_shader_ = LoadAndCompileShaderOrDie(tess_evaluation_shader_file,
                                                 GL_TESS_EVALUATION_SHADER);


    program_ = LinkProgramOrDie(vertex_shader_,
                                fragment_shader_,
                                tess_control_shader_,
                                tess_evaluation_shader_
                                );
    glUseProgram(program_);
    projection_matrix_location_ = GetUniformLocationOrDie("projection_matrix");
    view_matrix_location_ = GetUniformLocationOrDie("view_matrix");
    model_matrix_location_ = GetUniformLocationOrDie("model_matrix");
    material_locations_.emission = GetUniformLocationOrDie("material.emission");
    material_locations_.ambient = GetUniformLocationOrDie("material.ambient");
    material_locations_.diffuse = GetUniformLocationOrDie("material.diffuse");
    material_locations_.specular = GetUniformLocationOrDie("material.specular");
    material_locations_.shininess = GetUniformLocationOrDie("material.shininess");
    light_locations_.ambient = GetUniformLocationOrDie("light.ambient");
    light_locations_.attenuation = GetUniformLocationOrDie("light.attenuation");
    light_locations_.diffuse = GetUniformLocationOrDie("light.diffuse");
    light_locations_.position = GetUniformLocationOrDie("light.position");
    light_locations_.specular = GetUniformLocationOrDie("light.specular");
    lambda_location_ = GetUniformLocationOrDie("lambda");
    glUseProgram(0);
}


GLuint Program::LinkProgramOrDie(GLint vertex_shader,
                                 GLint fragment_shader,
                                 GLint tess_control_shader,
                                 GLint tess_evaluation_shader){
    GLuint new_program = glCreateProgram();
    glAttachShader(new_program, vertex_shader);
    glAttachShader(new_program, fragment_shader);
    glAttachShader(new_program, tess_evaluation_shader);
    glAttachShader(new_program, tess_control_shader);
    glLinkProgram(new_program);

    GLint  linked;
    glGetProgramiv(new_program, GL_LINK_STATUS, &linked);
    if ( !linked ) {
        cerr << "Shader program failed to link" << endl;
        GLint  log_size;
        glGetProgramiv(new_program, GL_INFO_LOG_LENGTH, &log_size);
        char* log_msg = new char[log_size];
        glGetProgramInfoLog(new_program, log_size, NULL, log_msg);
        cerr << log_msg << endl;
        delete [] log_msg;
        exit( EXIT_FAILURE );
    }
    return new_program;
}

Program::~Program(){
    glUseProgram(0);

    glDetachShader(program_, vertex_shader_);
    glDetachShader(program_, fragment_shader_);
    glDetachShader(program_, tess_evaluation_shader_);
    glDetachShader(program_, tess_control_shader_);

    glDeleteShader(fragment_shader_);
    glDeleteShader(tess_control_shader_);
    glDeleteShader(tess_evaluation_shader_);
    glDeleteShader(vertex_shader_);

    glDeleteProgram(program_);

}

GLuint Program::LoadAndCompileShaderOrDie(const char * source_file, GLenum type){
    int file_size;
    char * shader_code;
    GLuint shader=glCreateShader(type);
    ifstream file (source_file, ios::in|ios::ate|ios::binary);
    if (file.is_open()) {
        file_size = file.tellg();
        shader_code = new char [file_size+1];
        file.seekg (0, ios::beg);
        file.read (shader_code, file_size);
        shader_code[file_size]='\0';
        file.close();

        glShaderSource(shader, 1, (const GLchar**) &shader_code, NULL);
        glCompileShader(shader);
        delete[] shader_code;

    }
    else{ //file was not opened
        cerr<<"Could not open the file "<<source_file<<endl;
        exit( EXIT_FAILURE );
    }

    GLint  compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        switch(type){
        case GL_VERTEX_SHADER:
            cerr << "vertex ";
        break;
        case GL_FRAGMENT_SHADER:
            cerr << "fragment ";
        break;
        case GL_GEOMETRY_SHADER:
            cerr << "geometry ";
        break;
        case GL_TESS_CONTROL_SHADER:
            cerr << "tessellation control ";
        break;
        case GL_TESS_EVALUATION_SHADER:
            cerr << "tessellation evaluation ";
        break;
        }
        cerr << "shader is failed to compile:" << endl;
        GLint  log_size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        char* log_msg = new char[log_size];
        glGetShaderInfoLog(shader, log_size, NULL, log_msg);
        cerr << log_msg << endl;
        delete [] log_msg;
        exit( EXIT_FAILURE );
    }
    return shader;
}


void Program::SetLight(const PointLight & light) const{
    glUniform4fv(light_locations_.ambient, 1, light.ambient);
    glUniform4fv(light_locations_.diffuse, 1, light.diffuse);
    glUniform4fv(light_locations_.specular, 1, light.specular);
    glUniform4fv(light_locations_.position, 1, light.position);
    glUniform3fv(light_locations_.attenuation, 1, light.attenuation);
}

void Program::SetMaterial(const Material & material) const{
    glUniform4fv(material_locations_.ambient, 1, material.ambient);
    glUniform4fv(material_locations_.emission, 1, material.emission);
    glUniform4fv(material_locations_.diffuse, 1, material.diffuse);
    glUniform4fv(material_locations_.specular, 1, material.specular);
    glUniform1f(material_locations_.shininess, material.shininess);
}


void Program::SetLambda(GLfloat lambda) const{
    glUniform1f(lambda_location_, lambda);
}

void Program::SetModelMatrix(const Mat4 & matrix) const{
    glUniformMatrix4fv(model_matrix_location_, 1, GL_FALSE, matrix);
}

void Program::SetProjectionMatrix(const Mat4 & matrix) const{
    glUniformMatrix4fv(projection_matrix_location_, 1, GL_FALSE, matrix);
}

void Program::SetViewMatrix(const Mat4 & matrix) const{
    glUniformMatrix4fv(view_matrix_location_, 1, GL_FALSE, matrix);
}

GLint Program::GetUniformLocationOrDie(const char* var_name){
    GLint location=-1;
    location = glGetUniformLocation(program_, var_name);
    if (location < 0){
        cerr << "ERROR: cannot find uniform location " << var_name << endl;
        exit( EXIT_FAILURE );
    }
    return location;
}
