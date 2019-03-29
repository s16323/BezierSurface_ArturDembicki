#ifndef WINDOW_H
#define WINDOW_H

#include <ctime>

#include "program.h"
#include "bezier.h"
#include "matma.h"

class Window{
 public:
    Window(const char*, int, int);
    void Initialize(int argc, char* argv[], int major_gl_version, int minor_gl_version);
    void Resize(int new_width, int new_height);
    void Render(void);
    void KeyPressed(unsigned char key, int x_coord, int y_coord);
    void SpecialKeyPressed(int key, int x_coord, int y_coord);
    void MouseButton(int button, int state, int x_coord, int y_coord);
    void MouseMove(int x_coord, int y_coord);
 private:
    int width_;
    int height_;
    char title_[256];

    Bezier bezier_;
    Bezier bezier2_; //dodane
	Bezier bezier_dno_; //dodane

    Program program_;

    int x_origin_;
    int y_origin_;

    clock_t last_time_;

    Mat4 view_matrix_;
    Mat4 projection_matrix_;

    void InitGlutOrDie(int argc, char* argv[], int major_gl_version, int minor_gl_version);
    void InitGlewOrDie();
    void InitModels();
    void InitTextures();
    void InitPrograms();

    void SetViewMatrix();
    void SetProjectionMatrix();

};


#endif // WINDOW_H

