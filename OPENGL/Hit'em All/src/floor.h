#include "main.h"

#ifndef FLOOR_H
#define FLOOR_H

class Floor {
public:
    Floor() {}
    Floor(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void checkheight();
    double speedx;
    double speedy;
    double g;
    int upflag=0;
    bounding_box_t bounding_box();
private:
    VAO *object1;
    VAO *object2;
};

//class Floor{
//public:
//    Floor() {}
//    Floor(float height, color_t color);
//    float h;
//    glm::vec3 position;
//    float rotation;
//    void draw(glm::mat4 VP);

//private:
//    VAO *object;
//};

#endif // FLOOR_H
