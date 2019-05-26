#ifndef _MODEL_H
#define _MODEL_H

#include <GL/gl3w.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class Model {
public:
    void SetModelViewProjection(int screen_height, int screen_width);
    glm::mat4 GetMVP() const { return _mvp; }
private:
    float cam_distance = -4.0f;
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;
    glm::mat4 _mvp;
};

#endif
