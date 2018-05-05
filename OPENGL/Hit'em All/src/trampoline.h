#include "main.h"

#ifndef TRAMP_H
#define TRAMP_H


class Tramp {
public:
    Tramp() {}
    Tramp(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // TRAMP_H
