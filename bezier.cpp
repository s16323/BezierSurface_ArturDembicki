#include "bezier.h"

#include <iostream>

Bezier::Bezier(){
    velocity_ = 90; //30
    angle_ = 10;
    animating_ = true;
}

Bezier::~Bezier(){
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteVertexArrays(1, &vao_);
}

void Bezier::Initialize(const GLfloat points[16][4]){
    glGenVertexArrays(1,&vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, 64*sizeof(GLfloat), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Bezier::Move(float dt){
    if(!animating_) return;
    angle_ += dt* velocity_;
    if (angle_ > 360) {angle_ -=360;}
    if (angle_ < -360) {angle_ +=360;}

    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutX(angle_);
 //   model_matrix_.RotateAboutY(angle_);
}


void Bezier::Draw(const Program &program) const{
    glBindVertexArray(vao_);

    glUseProgram(program);
    program.SetModelMatrix(model_matrix_);
    program.SetMaterial(material_);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    program.SetLambda(1);
    glFrontFace(GL_CW);    //glFrontFace(GL_CCW);

    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(GL_PATCHES, 0, 16);

    program.SetLambda(-1);
    glFrontFace(GL_CCW);

    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(GL_PATCHES, 0, 16);


    glDisable(GL_CULL_FACE);
    glBindVertexArray(0);
    glUseProgram(0);
}
