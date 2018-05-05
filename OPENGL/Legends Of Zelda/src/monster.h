#include "main.h"
#include "ball.h"

#ifndef MONSTER_H
#define MONSTER_H


class Monster {
public:
    Monster() {}
    Monster(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
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

#endif // MONSTER_H
