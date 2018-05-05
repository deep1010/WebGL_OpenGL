#include "main.h"

#ifndef TRAP_H
#define TRAP_H


class Trap {
public:
    Trap() {}
    Trap(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // TRAP_H
