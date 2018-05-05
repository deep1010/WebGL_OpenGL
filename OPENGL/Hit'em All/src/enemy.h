#include "main.h"

#ifndef ENEMY_H
#define ENEMY_H


class Enemy {
public:
    Enemy() {}
    Enemy(float x, float y);
    glm::vec3 position;
    float rotation;
    int hitflag=0;
    color_t color[2]={COLOR_BLACK,COLOR_GREEN};
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedx;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // ENEMY_H
