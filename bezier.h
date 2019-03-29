#ifndef BEZIER_H
#define BEZIER_H

#include "program.h"
#include "matma.h"
#include "material.h"


class Bezier{
    public:
        Bezier();
        void Initialize(const GLfloat points [64][4]);
        void Draw(const Program & program) const;
        ~Bezier();
        void SetMaterial(const Material & m){material_ = m;}
        void Move(float dt);
        void SpeedUp(){velocity_ *= 1.09544511501;}
        void SlowDown(){velocity_/=1.09544511501;}
        void ToggleAnimated(){animating_ = !animating_;}
    private:
        GLuint vao_;
        GLuint vertex_buffer_;

        Mat4 model_matrix_;
        Material material_;

        float velocity_;
        float angle_;
        bool animating_;
};

#endif // BEZIER_H
