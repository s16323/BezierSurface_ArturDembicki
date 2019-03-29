#include "window.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <cstring>

#include "glerror.h"
#include "material.h"
#include "light.h"

const char* kVertexShader="Shader.vertex.glsl";
const char* kFragmentShader="Shader.fragment.glsl";
const char* kTessControlShader="Shader.tess.control.glsl";
const char* kTessEvaluationShader="Shader.tess.evaluation.glsl";

const Material  kYellowMaterial={
    {0.6f, 0.6f, 0.6f, 1.0f}, //Ambient
    {1.0f, 1.0f, 0.0f, 1.0f}, //Diffuse
    {0.6f, 0.6f, 0.6f, 1.0f}, //Specular
    {0.0f, 0.0f, 0.0f, 1.0f}, //Emission
    60.0f //shininess
  };


const PointLight kPointLight={
    {0.0f, 9.5f, 3.0f, 1.0f}, //position
    {0.1f, 0.1f, 0.1f, 1.0f}, //ambient
    {1.0f, 1.0f, 1.0f, 1.0f}, //diffuse
    {1.0f, 1.0f, 1.0f, 1.0f}, //specular
    {0.5f, 0.005f, 0.0125f}	//attenuation
};

const GLfloat kBezierOne[16][4] ={

    
    {-3, 1, 0, 1}, //-2
        {-2.0f/3.0f, 1.0f/3.0f, 4.0f/3.0f, 1.0f/3.0f},
        { 2.0f/3.0f, 1.0f/3.0f, 4.0f/3.0f, 1.0f/3.0f},
    {2, 1, 0, 1},
    {-1.5, 0.5, 0,1},
        {-0.5, 1.0f/6.0f, 1, 1.0f/3.0f},
        { 0.5, 1.0f/6.0f, 1, 1.0f/3.0f},
    {1.5, 0.5, 0, 1},
    {-3, 0, 0,1},
        {-1, 0, 2, 1.0f/3.0f},
        { 1, 0, 2, 1.0f/3.0f},
    {3, 0, 0, 1},
    {-2, -1, 0, 1},
        {-2.0f/3.0f, -1.0f/3.0f, 4.0f/3.0f, 1.0f/3.0f},
        { 2.0f/3.0f, -1.0f/3.0f, 4.0f/3.0f, 1.0f/3.0f},
    {2, -1, 0, 1},
    
};

const GLfloat kBezierTwo[16][4] ={
    
    {2, 1, 0, 1},	//x,y,z,w a konkretnie
        { 2.0f/3.0f, 1.0f/3.0f, -4.0f/3.0f, 1.0f/3.0f},
        {-2.0f/3.0f, 1.0f/3.0f, -4.0f/3.0f, 1.0f/3.0f},
    {-3, 1, 0, 1}, //-2
    {1.5, 0.5, 0,1},
        {0.5, 1.0f/6.0f, -1, 1.0f/3.0f},
        {-0.5, 1.0f/6.0f, -1, 1.0f/3.0f},
    {-1.5, 0.5, 0, 1},
    {3, 0, 0,1},
        {1, 0, -2, 1.0f/3.0f},
        {-1, 0, -2, 1.0f/3.0f},
    {-3, 0, 0, 1},
    {2, -1, 0, 1},
        {2.0f/3.0f, -1.0f/3.0f, -4.0f/3.0f, 1.0f/3.0f},
        {-2.0f/3.0f, -1.0f/3.0f, -4.0f/3.0f, 1.0f/3.0f},
    {-2, -1, 0, 1}

};

const GLfloat kBezierDno[16][4] ={
    //x,y,z,w
	{-2,-1,0,1}, //A
	{-2.0f/3.0f,-1.0f/3.0f,-4.0f/3.0f,1.0f/3.0f}, //B
	{2.0f/3.0f,-1.0f/3.0f,-4.0f/3.0f,1.0f/3.0f}, //C
{2,-1,0,1}, //D
{-2,-1,0,1}, //E
	{-2.0f/3.0f,-1.0f/3.0f,4.0f/3.0f,1.0f/3.0f}, //F
	{2.0f/3.0f,-1.0f/3.0f,4.0f/3.0f,1.0f/3.0f}, //G
{2,-1,0,1}, //H


{-2,-1,0,1}, //A
	{-2.0f/3.0f,-1.0f/3.0f,-4.0f/3.0f,1.0f/3.0f}, //B
	{2.0f/3.0f,-1.0f/3.0f,-4.0f/3.0f,1.0f/3.0f}, //C
{2,-1,0,1}, //D
{-2,-1,0,1}, //E
	{-2.0f/3.0f,-1.0f/3.0f,4.0f/3.0f,1.0f/3.0f}, //F
	{2.0f/3.0f,-1.0f/3.0f,4.0f/3.0f,1.0f/3.0f}, //G
{2,-1,0,1} //H

};


Window::Window(const char * title, int width, int height){
    strcpy(title_, title);
    width_ = width;
    height_ = height;
    last_time_ = 0;
    x_origin_ = y_origin_ = -1;
}

void Window::Initialize(int argc, char * argv[], int major_gl_version, int minor_gl_version){

    InitGlutOrDie(argc, argv, major_gl_version, minor_gl_version);
    InitGlewOrDie();

    std::cout << "OpenGL initialized: OpenGL version: " << glGetString(GL_VERSION) << " GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

//    InitTextures();
    InitModels();

    InitPrograms();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void Window::InitGlutOrDie(int argc, char * argv[], int major_gl_version, int minor_gl_version){
    glutInit(&argc, argv);

    glutInitContextVersion(major_gl_version, minor_gl_version);
    glutInitContextProfile(GLUT_CORE_PROFILE);
#ifdef DEBUG
    glutInitContextFlags(GLUT_DEBUG);
#endif

    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    glutInitWindowSize(width_, height_);

    glutInitDisplayMode(GLUT_DEPTH| GLUT_DOUBLE | GLUT_RGBA);

    int window_handle = glutCreateWindow(title_);

    if( window_handle < 1) {
        std::cerr << "ERROR: Could not create a new rendering window" << std::endl;
        exit(EXIT_FAILURE);
    }


}

void Window::InitGlewOrDie(){
    GLenum glew_init_result;
    glewExperimental = GL_TRUE;
    glew_init_result = glewInit();

    if (GLEW_OK != glew_init_result) {
        std::cerr << "Glew ERROR: " << glewGetErrorString(glew_init_result) << std::endl;
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    if(glDebugMessageCallback){
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC)OpenglCallbackFunction, NULL);
        GLuint unused_ids = 0;
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            &unused_ids,
            GL_FALSE);
    }
    else
        std::cout << "glDebugMessageCallback not available" << std::endl;
#endif

}

void Window::InitModels(){
    bezier_.Initialize(kBezierOne);
    bezier_.SetMaterial(kYellowMaterial);
    bezier2_.Initialize(kBezierTwo);
    bezier2_.SetMaterial(kYellowMaterial);
    bezier_dno_.Initialize(kBezierDno);
    bezier_dno_.SetMaterial(kYellowMaterial);
}


void Window::InitPrograms(){
    view_matrix_.Translate(0, 0, -10);
    projection_matrix_ = Mat4::CreateProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);

    //point
    program_.Initialize(kVertexShader,
                        kFragmentShader,
                        kTessControlShader,
                        kTessEvaluationShader);
    glUseProgram(program_);
    program_.SetLight(kPointLight);
    program_.SetProjectionMatrix(projection_matrix_);
    program_.SetViewMatrix(view_matrix_);

    glUseProgram(0);
}

void Window::Resize(int new_width, int new_height){
    width_ = new_width;
    height_ = new_height;
    projection_matrix_ = Mat4::CreateProjectionMatrix(60,
                (float)width_/(float)height_, 0.1f, 100.0f);
    SetProjectionMatrix();
    glViewport(0, 0, width_, height_);
    glutPostRedisplay();
}

void Window::KeyPressed(unsigned char key, int /*x_coord*/, int /*y_coord*/){
    switch (key){
    case 27:
      glutLeaveMainLoop();
    break;
    case ' ':
        bezier_.ToggleAnimated();
        bezier2_.ToggleAnimated();
        bezier_dno_.ToggleAnimated();
    break;
    default:
    break;
    }
}

void Window::SpecialKeyPressed(int key, int /*x_coord*/, int /*y_coord*/){
    switch (key){
    case GLUT_KEY_LEFT:
        bezier_.SlowDown();
        bezier2_.SlowDown();
        bezier_dno_.SlowDown();
    break;
    case GLUT_KEY_RIGHT:
        bezier_.SpeedUp();
        bezier2_.SpeedUp();
        bezier_dno_.SpeedUp();
    break;
    default:
    break;
    }
}

void Window::SetViewMatrix(){
    glUseProgram(program_);
    program_.SetViewMatrix(view_matrix_);
    glUseProgram(0);
}

void Window::SetProjectionMatrix(){
    glUseProgram(program_);
    program_.SetProjectionMatrix(projection_matrix_);
    glUseProgram(0);
}


void Window::MouseMove(int x_coord, int y_coord){	// this will only be true when the left button is down
    float delta_x_angle=0;
    float delta_y_angle=0;
    if (x_origin_ >= 0 && y_origin_ >=0) {
        // update deltaAngle
        delta_x_angle = (x_coord - x_origin_) * 0.1f;
        delta_y_angle = (y_coord - y_origin_) * 0.1f;
        x_origin_=x_coord;
        y_origin_=y_coord;

        // update camera's direction
        view_matrix_.RotateAboutY(delta_x_angle);
        view_matrix_.RotateAboutX(delta_y_angle);
        SetViewMatrix();
        glutPostRedisplay();
   }
}


#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif

void Window::MouseButton(int button, int state, int x_coord, int y_coord) {
    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
          // when the button is released
          if (state == GLUT_UP) {
              x_origin_ = -1;
              y_origin_ = -1;
          }
          else  {// state = GLUT_DOWN
              x_origin_ = x_coord;
              y_origin_ = y_coord;
          }
    }
    else if ( button == GLUT_WHEEL_UP ){
        view_matrix_.Translate(0, 0, -0.1f);
        SetViewMatrix();
        glutPostRedisplay();
    }
    else if( button == GLUT_WHEEL_DOWN ){
        view_matrix_.Translate(0, 0, 0.1f);
        SetViewMatrix();
        glutPostRedisplay();
    }
}



void Window::Render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    clock_t now = clock();
    if (last_time_ == 0) last_time_ = now;
    bezier_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
    bezier2_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
    bezier_dno_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
    last_time_ = now;

    bezier_.Draw(program_);
	bezier2_.Draw(program_);
	bezier_dno_.Draw(program_);

    glutSwapBuffers();
    glutPostRedisplay();

}


