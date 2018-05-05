#include "main.h"
#include "ball.h"

#ifndef BOSS_H
#define BOSS_H


class Boss {
public:
    Boss() {}
    Boss(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float health;
    void draw(glm::mat4 VP,Ball boat);
    bounding_box_t bounding_box();
    int hitflag;
    int fireflag;
    void tick();
    float speed;

private:
    VAO *object;
    VAO *object2;
//    VAO *object3;
};

#endif // BOSS_H
