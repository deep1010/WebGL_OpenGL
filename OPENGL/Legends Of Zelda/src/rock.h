#include "main.h"

#ifndef ROCK_H
#define ROCK_H


class Rock {
public:
    Rock() {}
    Rock(float x, float y,float z,float eflg);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    int hitflag;
    int eflag;
    color_t colour[2];
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // ROCK_H
