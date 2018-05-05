#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speedx = 0.00;
    speedy = 0.00;
    g=-0.002;
    GLfloat vertex_buffer_data[10000];
        double pi=M_PI;
        double rad=pi/180;

        for(int i=0;i<360;i++)
        {
            vertex_buffer_data[9*i] = 0.0f;
            vertex_buffer_data[9*i+1]=0.0f;
            vertex_buffer_data[9*i+2]=0.0f;
            vertex_buffer_data[9*i+3]=cos(i*rad)*0.2;
            vertex_buffer_data[9*i+4]=sin(i*rad)*0.2;
            vertex_buffer_data[9*i+5]=0.0f;
            vertex_buffer_data[9*i+6]=cos((i+1)*rad)*0.2;
            vertex_buffer_data[9*i+7]=sin((i+1)*rad)*0.2;
            vertex_buffer_data[9*i+8]=0.0f;
        }
    this->object = create3DObject(GL_TRIANGLES, 360*3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    speedy+=g;
    this->position.y += speedy;
    this->position.x += speedx;

}
void Ball::checkheight() {
    if(this->position.y <= -2.8 && !this->onwater())
    {
        this->position.y=-2.8;
        reset_speedy();
    }
    if(this->onwater())
    {
        this->position.y=-3.0;
    }
    if(this->position.y >= 3.8)
    {
        this->position.y=3.8;
        this->speedy*=-0.5;
    }
    if(this->position.x >= 3.8)
    {
        this->position.x = 3.8;
        this->speedx*=-0.5;
    }
    if(this->position.x <= -3.8)
    {
        this->position.x = -3.8;
        this->speedx*=-0.5;
    }
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
void Ball::reset_speedy() {
    speedy*=-0.5;
    upflag=0;
}
bool Ball::onwater() {
    if(this->position.x <= 1.25 && this->position.x >=-1.25 && this->position.y <= -2.8)
    {
        this->speedy=0;
        return true;
    }
    else
        return false;
}
