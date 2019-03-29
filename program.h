#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <cstdlib>

#include "light.h"
#include "material.h"
#include "matma.h"

class Program{
public:
    void Initialize(const char* vertex_shader_file,
                    const char* fragment_shader_file,
                    const char* tess_control_shader_file,
                    const char* tess_evaluation_shader_file);
    operator GLuint() const{return program_;} // to be used in glUseFunction()
    void SetLight(const PointLight & light) const;
    void SetModelMatrix(const Mat4 &) const;
    void SetViewMatrix(const Mat4 &) const;
    void SetProjectionMatrix(const Mat4 &) const;
    void SetMaterial(const Material &material) const;
    void SetLambda(GLfloat lambda) const;
    ~Program();
private:
    GLuint program_;
    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint tess_control_shader_;
    GLuint tess_evaluation_shader_;

    GLuint lambda_location_;
    GLuint model_matrix_location_;
    GLuint projection_matrix_location_;
    GLuint view_matrix_location_;
    struct {
        GLuint emission;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
    } material_locations_;
    struct {
        GLuint ambient;
        GLuint attenuation;
        GLuint diffuse;
        GLuint position;
        GLuint specular;
    }light_locations_;


    GLuint LoadAndCompileShaderOrDie(const char* source_file, GLenum type);
    GLuint LinkProgramOrDie(GLint vertex_shader,
                            GLint fragment_shader,
                            GLint tess_control_shader,
                            GLint tess_evaluation_shader
                            );
    GLint GetUniformLocationOrDie(const char* var_name);
};

#endif // PROGRAM_H
