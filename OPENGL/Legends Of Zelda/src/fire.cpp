#include "fire.h"
#include "main.h"

Fire::Fire(float x, float y,float angle ,color_t color,float sped) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = angle;
    this->speed = sped;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -0.25f,-0.5f,-0.25f, // triangle 1 : begin
        -0.25f,-0.5f, 0.25f,
        -0.25f, 0.5f, 0.25f, // triangle 1 : end
        -0.25f, 0.5f,-0.25f, // triangle 2 : begin
        -0.25f,-0.5f,-0.25f,
        -0.25f, 0.5f, 0.25f, // triangle 2 : end

        0.25f,-0.5f,-0.25f, // triangle 1 : begin
        0.25f,-0.5f, 0.25f,
        0.25f, 0.5f, 0.25f, // triangle 1 : end
        0.25f, 0.5f,-0.25f, // triangle 2 : begin
        0.25f,-0.5f,-0.25f,
        0.25f, 0.5f, 0.25f, // triangle 2 : end

        -0.25f,-0.5f, 0.25f,
        -0.25f, 0.5f, 0.25f,
         0.25f, 0.5f, 0.25f,
         0.25f, 0.5f, 0.25f,
        -0.25f,-0.5f, 0.25f,
         0.25f,-0.5f, 0.25f,

        -0.25f,-0.5f,-0.25f,
        -0.25f, 0.5f,-0.25f,
         0.25f, 0.5f,-0.25f,
         0.25f, 0.5f,-0.25f,
        -0.25f,-0.5f,-0.25f,
         0.25f,-0.5f,-0.25f,

        -0.25f, 0.5f,-0.25f,
        -0.25f, 0.5f, 0.25f,
         0.25f, 0.5f, 0.25f,
         0.25f, 0.5f,-0.25f,
        -0.25f, 0.5f,-0.25f,
         0.25f, 0.5f, 0.25f,

        -0.25f,-0.5f,-0.25f,
        -0.25f,-0.5f, 0.25f,
         0.25f,-0.5f, 0.25f,
         0.25f,-0.5f,-0.25f,
        -0.25f,-0.5f,-0.25f,
         0.25f,-0.5f, 0.25f,
    };


    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Fire::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_t Fire::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.5, 1.0 };
    return bbox;
}

void Fire::tick() {
    position.y += this->speed*cos(rotation*M_PI/180.0f);
    position.x -= this->speed*sin(rotation*M_PI/180.0f);
}

