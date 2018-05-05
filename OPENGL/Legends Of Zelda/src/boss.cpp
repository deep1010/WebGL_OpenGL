#include "boss.h"
#include "main.h"

Boss :: Boss(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 1);
    this->rotation = 0;
    hitflag=0;
    fireflag=0;
    speed=0.05;
    health=100;
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
        1.0f,-1.0f, 1.0f,

        -0.25f,-0.25f, 1.0f, // triangle 1 : begin
        -0.25f,-0.25f, 0.25f+1.25f,
        -0.25f, 0.25f, 0.25f+1.25f, // triangle 1 : end
        0.25f, 0.25f,-0.25f+1.25f, // triangle 2 : begin
        -0.25f,-0.25f,-0.25f+1.25f,
        -0.25f, 0.25f,-0.25f+1.25f, // triangle 2 : end
        0.25f,-0.25f, 0.25f+1.25f,
        -0.25f,-0.25f,-0.25f+1.25f,
        0.25f,-0.25f,-0.25f+1.25f,
        0.25f, 0.25f,-0.25f+1.25f,
        0.25f,-0.25f,-0.25f+1.25f,
        -0.25f,-0.25f,-0.25f+1.25f,
        -0.25f,-0.25f,-0.25f+1.25f,
        -0.25f, 0.25f, 0.25f+1.25f,
        -0.25f, 0.25f,-0.25f+1.25f,
        0.25f,-0.25f, 0.25f+1.25f,
        -0.25f,-0.25f, 0.25f+1.25f,
        -0.25f,-0.25f,-0.25f+1.25f,
        -0.25f, 0.25f, 0.25f+1.25f,
        -0.25f,-0.25f, 0.25f+1.25f,
        0.25f,-0.25f, 0.25f+1.25f,
        0.25f, 0.25f, 0.25f+1.25f,
        0.25f,-0.25f,-0.25f+1.25f,
        0.25f, 0.25f,-0.25f+1.25f,
        0.25f,-0.25f,-0.25f+1.25f,
        0.25f, 0.25f, 0.25f+1.25f,
        0.25f,-0.25f, 0.25f+1.25f,
        0.25f, 0.25f, 0.25f+1.25f,
        0.25f, 0.25f,-0.25f+1.25f,
        -0.25f, 0.25f,-0.25f+1.25f,
        0.25f, 0.25f, 0.25f+1.25f,
        -0.25f, 0.25f,-0.25f+1.25f,
        -0.25f, 0.25f, 0.25f+1.25f,
        0.25f, 0.25f, 0.25f+1.25f,
        -0.25f, 0.25f, 0.25f+1.25f,
        0.25f,-0.25f, 0.25f+1.25f,


        -0.5f,-0.5f,-0.5f+2.0f, // triangle 1 : begin
        -0.5f,-0.5f, 0.5f+2.0f,
        -0.5f, 0.5f, 0.5+2.0f, // triangle 1 : end
        0.5f, 0.5f,-0.5f+2.0f, // triangle 2 : begin
        -0.5f,-0.5f,-0.5f+2.0f,
        -0.5f, 0.5f,-0.5f+2.0f, // triangle 2 : end
        0.5f,-0.5f, 0.5f+2.0f,
        -0.5f,-0.5f,-0.5f+2.0f,
        0.5f,-0.5f,-0.5f+2.0f,
        0.5f, 0.5f,-0.5f+2.0f,
        0.5f,-0.5f,-0.5f+2.0f,
        -0.5f,-0.5f,-0.5f+2.0f,
        -0.5f,-0.5f,-0.5f+2.0f,
        -0.5f, 0.5f, 0.5f+2.0f,
        -0.5f, 0.5f,-0.5f+2.0f,
        0.5f,-0.5f, 0.5f+2.0f,
        -0.5f,-0.5f, 0.5f+2.0f,
        -0.5f,-0.5f,-0.5f+2.0f,
        -0.5f, 0.5f, 0.5f+2.0f,
        -0.5f,-0.5f, 0.5f+2.0f,
        0.5f,-0.5f, 0.5f+2.0f,
        0.5f, 0.5f, 0.5f+2.0f,
        0.5f,-0.5f,-0.5f+2.0f,
        0.5f, 0.5f,-0.5f+2.0f,
        0.5f,-0.5f,-0.5f+2.0f,
        0.5f, 0.5f, 0.5f+2.0f,
        0.5f,-0.5f, 0.5f+2.0f,
        0.5f, 0.5f, 0.5f+2.0f,
        0.5f, 0.5f,-0.5f+2.0f,
        -0.5f, 0.5f,-0.5f+2.0f,
        0.5f, 0.5f, 0.5f+2.0f,
        -0.5f, 0.5f,-0.5f+2.0f,
        -0.5f, 0.5f, 0.5f+2.0f,
        0.5f, 0.5f, 0.5f+2.0f,
        -0.5f, 0.5f, 0.5f+2.0f,
        0.5f,-0.5f, 0.5f+2.0f,
    };

    static const GLfloat vertex_buffer_data2[] = {
        0.375f, -0.5f, 2.125f,
        0.375f, -0.5f, 2.375f,
        0.125f, -0.5f, 2.125f,
        0.125f, -0.5f, 2.375f,
        0.375f, -0.5f, 2.125f,
        0.125f, -0.5f, 2.125f,

       -0.375f, -0.5f, 2.125f,
       -0.375f, -0.5f, 2.375f,
       -0.125f, -0.5f, 2.125f,
       -0.125f, -0.5f, 2.375f,
       -0.375f, -0.5f, 2.125f,
       -0.125f, -0.5f, 2.125f,
    };

    this->object = create3DObject(GL_TRIANGLES, 12*3*3, vertex_buffer_data, COLOR_RED, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data2, COLOR_BACKGROUND, GL_FILL);

}

void Boss::draw(glm::mat4 VP,Ball boat) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    float x1=position.x-boat.position.x;
    float y1=position.y-boat.position.y;
    this->rotation=atan2(y1,x1) - M_PI/2;
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object2);
}

bounding_box_t Boss::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 2.0, 2.0 };
    return bbox;
}
void Boss::tick()
{
    if(position.z>=1 || position.z<=-3.5)
        speed*=-1;
    position.z+=speed;
}

