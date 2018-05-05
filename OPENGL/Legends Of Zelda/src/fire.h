#include "main.h"

#ifndef FIRE_H
#define FIRE_H


class Fire {
public:
    Fire() {}
    Fire(float x, float y, float angle,color_t color,float sped);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    double speed;
    bounding_box_t bounding_box();
    float wind;
private:
    VAO *object;
};

#endif // FIRE_H
