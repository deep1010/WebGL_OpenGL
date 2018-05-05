#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0.1);
    this->rotation = 0;
    speed = 0.2;
    wind=0.01;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end

        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end

        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,

        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,

        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,

        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    static const GLfloat vertex_buffer_data2[] = {
        1.0f, 1.0f, 1.0f, // triangle 1 : begin
        1.0f, 1.0f, -1.0f,
        0.0f, 4.0f, 1.0f, // triangle 1 : end
        -1.0f, 1.0f, 1.0f, // triangle 2 : begin
        -1.0f,1.0f,-1.0f,
        0.0f, 4.0f, 1.0f, // triangle 2 : end
        -1.0f,1.0f, -1.0f,
        1.0f,1.0f,-1.0f,
        0.0f,4.0f,1.0f,
        1.0f, 1.0f,1.0f,
        -1.0f,1.0f,1.0f,
        0.0f, 4.0f, 1.0f,

        1.0f, -1.0f, 1.0f, // triangle 1 : begin
        1.0f, -1.0f, -1.0f,
        0.0f, -4.0f, 1.0f, // triangle 1 : end
        -1.0f, -1.0f, 1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        0.0f, -4.0f, 1.0f, // triangle 2 : end
        -1.0f,-1.0f, -1.0f,
        1.0f,-1.0f,-1.0f,
        0.0f,-4.0f,1.0f,
        1.0f, -1.0f,1.0f,
        -1.0f,-1.0f,1.0f,
        0.0f, -4.0f, 1.0f,
    };
//    static const GLfloat vertex_buffer_data[] = {
//        1.0f, 1.0f, 1.0f,
//        1.0f, 1.0f, -1.0f,
//        0.0f, 4.0f, 1.0f,
//        -1.0f, 1.0f, 1.0f,
//        -1.0f,1.0f,-1.0f,
//        0.0f, 4.0f, 1.0f,

//        1.0f, -1.0f, 1.0f,
//        1.0f, -1.0f,-1.0f,
//        0.0f, -4.0f, 1.0f,
//       -1.0f, -1.0f, 1.0f,
//       -1.0f, -1.0f,-1.0f,
//        0.0f, -4.0f, 1.0f,

//       -1.0f, -1.0f, 1.0f,
//       -1.0f, -1.0f,-1.0f,
//       -1.0f,  1.0f, 1.0f,
//       -1.0f, -1.0f,-1.0f,
//       -1.0f,  1.0f,-1.0f,
//       -1.0f,  1.0f, 1.0f,

//       1.0f, -1.0f, 1.0f,
//       1.0f, -1.0f,-1.0f,
//       1.0f,  1.0f, 1.0f,
//       1.0f, -1.0f,-1.0f,
//       1.0f,  1.0f,-1.0f,
//       1.0f,  1.0f, 1.0f,

//    };
//    static const GLfloat vertex_buffer_data2[] = {

//        1.0f, -1.0f,-1.0f,
//        1.0f,  1.0f,-1.0f,
//       -1.0f, -1.0f,-1.0f,
//       -1.0f,  1.0f,-1.0f,
//        1.0f,  1.0f,-1.0f,
//       -1.0f, -1.0f,-1.0f,

//        1.0f,  1.0f, -1.0f,
//       -1.0f,  1.0f, -1.0f,
//        0.0f,  4.0f,  1.0f,

//        1.0f, -1.0f, -1.0f,
//       -1.0f, -1.0f, -1.0f,
//        0.0f, -4.0f,  1.0f,

//        };
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 8*3, vertex_buffer_data2, COLOR_BLACK, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object2);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 2.0, 8.0 };
    return bbox;
}

void Ball::tick() {
    float a=this->wind*cos(rotation*M_PI/180.0f);
    position.y += a>0?a:-1*a;

    position.y += speed*cos(rotation*M_PI/180.0f);
    position.x -= speed*sin(rotation*M_PI/180.0f);

    if(speed>=0.001)
        speed-=0.0125;
}

