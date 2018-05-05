#include "boss.h"
#include "main.h"
#include "timer.h"
#include "ball.h"
#include "sea.h"
#include "rock.h"
#include "fire.h"
#include "monster.h"
#include <stdio.h>
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
/**************************
* Customizable functions *
**************************/
const color_t COLOR_BLUE    = { 20, 196, 253 };
const color_t COLOR_BROWN   = {110,  14,  14 };
const color_t COLOR_ORANGE  = {255, 193,   0 };

Ball ball;
Sea sea;
Rock rock[104];
Fire bullet;
Fire monster_bullet[4];
Fire boss_bullet;
Monster mnstr[4];
Boss boss;
float screen_zoom = 0.125, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
long long int score=0;
int lives=3;
int health=100;
int fireflag=0;
int monster_hit=0;
int width  = 900;
int height = 900;
float heli_angle=0;
float heli_height;
float heli_dist;
float boost=0;
int view_mode=1;
Timer t60(1.0 / 60);

char score_buffer[10]={'\0'};

void disp_score();
void GETGIFT(int i);
float eye_coord[3];
float target_coord[3];
float up_coord[3];

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    if(view_mode==5)
    {

    // Heli-Cam VIEW
        eye_coord[0]=ball.position.x+heli_dist*sin(heli_angle*M_PI/180.0f);
        eye_coord[1]=ball.position.y-heli_dist*cos(heli_angle*M_PI/180.0f);
        eye_coord[2]=heli_height;

        target_coord[0]=ball.position.x-4.0*sin(ball.rotation*M_PI/180.0f);
        target_coord[1]=ball.position.y+4.0*cos(ball.rotation*M_PI/180.0f);
        target_coord[2]=ball.position.z+4;

        up_coord[0]=0;
        up_coord[1]=0;
        up_coord[2]=1;

    }
    if(view_mode==4)
    {

    // TOWER VIEW

    eye_coord[0]=0;
    eye_coord[1]=0;
    eye_coord[2]=14;

    target_coord[0]=ball.position.x;
    target_coord[1]=ball.position.y;
    target_coord[2]=ball.position.z;

    up_coord[0]=ball.position.x;
    up_coord[1]=ball.position.y;
    up_coord[2]=0;

    }
    if(view_mode==3)
    {

    // Follow-Cam VIEW

    eye_coord[0]=ball.position.x+10.0*sin(ball.rotation*M_PI/180.0f);
    eye_coord[1]=ball.position.y-10.0*cos(ball.rotation*M_PI/180.0f);
    eye_coord[2]=ball.position.z+14;

    target_coord[0]=ball.position.x;
    target_coord[1]=ball.position.y;
    target_coord[2]=ball.position.z+4;

    up_coord[0]=0;
    up_coord[1]=0;
    up_coord[2]=1;

    }
    if(view_mode==2)
    {

    // BOAT VIEW

    eye_coord[0]=ball.position.x;
    eye_coord[1]=ball.position.y;
    eye_coord[2]=ball.position.z+4;

    target_coord[0]=ball.position.x-5.0*sin(ball.rotation*M_PI/180.0f);
    target_coord[1]=ball.position.y+5.0*cos(ball.rotation*M_PI/180.0f);
    target_coord[2]=ball.position.z+2;

    up_coord[0]=0;
    up_coord[1]=0;
    up_coord[2]=1;

    }
    if(view_mode==1)
    {
        // TOP VIEW
        eye_coord[0]=ball.position.x;
        eye_coord[1]=ball.position.y;
        eye_coord[2]=20;

        target_coord[0]=ball.position.x;
        target_coord[1]=ball.position.y;
        target_coord[2]=0;

        up_coord[0]=0;
        up_coord[1]=1;
        up_coord[2]=0;
    }
    glm::vec3 eye ( eye_coord[0],eye_coord[1],eye_coord[2]) ;
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (target_coord[0],target_coord[1],target_coord[2]);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (up_coord[0],up_coord[1],up_coord[2]);

    // Compute Camera matrix (view)
        Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball.draw(VP);
    sea.draw(VP);
    for(int i=0;i<100;i++)
        if(!rock[i].hitflag)
            rock[i].draw(VP);
    for(int i=0;i<monster_hit;i++)
        if(!rock[100+i].hitflag)
            rock[100+i].draw(VP);
    if(fireflag)
        bullet.draw(VP);
    for(int i=0;i<4;i++)
    {
        if(!mnstr[i].hitflag)
        {
            mnstr[i].draw(VP,ball);
            if(mnstr[i].fireflag)
                monster_bullet[i].draw(VP);
        }

    }
    if(monster_hit==4 && !boss.hitflag)
    {
        boss.draw(VP,ball);
        if(boss.fireflag)
            boss_bullet.draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up  = glfwGetKey(window, GLFW_KEY_UP);
    int down  = glfwGetKey(window, GLFW_KEY_DOWN);
    int fire = glfwGetKey(window, GLFW_KEY_F);
    int top_view = glfwGetKey(window, GLFW_KEY_1);
    int boat_view = glfwGetKey(window, GLFW_KEY_2);
    int flwcam_view = glfwGetKey(window, GLFW_KEY_3);
    int tower_view = glfwGetKey(window, GLFW_KEY_4);
    int heli_view = glfwGetKey(window, GLFW_KEY_5);
    int w  = glfwGetKey(window, GLFW_KEY_W);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int s  = glfwGetKey(window, GLFW_KEY_S);
    int d  = glfwGetKey(window, GLFW_KEY_D);
    int zi  = glfwGetKey(window, GLFW_KEY_I);
    int zo  = glfwGetKey(window, GLFW_KEY_O);
    if(top_view)
    {
        view_mode=1;
        reshapeWindow(window,width,height);
    }
    if(boat_view)
    {
        view_mode=2;
        reshapeWindow(window,width,height);
    }
    if(flwcam_view)
    {
        view_mode=3;
        reshapeWindow(window,width,height);
    }
    if(tower_view)
    {
        view_mode=4;
        reshapeWindow(window,width,height);
    }
    if(heli_view)
    {
        // heli_angle=ball.rotation;
        heli_height=ball.position.z+5;
        heli_dist=8.0;
        view_mode=5;
        reshapeWindow(window,width,height);
    }
    if (left) {
        if(/*ball.rotation<90*/1)
        ball.rotation += 0.5;
    }
    if (right) {
        if(/*ball.rotation>-90*/1)
        ball.rotation -= 0.5;
    }
    if (up) {
        ball.speed= 0.15 + boost;
    }
    if (down) {
//        ball.speed
        ball.position.y -= 0.15*cos(ball.rotation*M_PI/180.0f);
        ball.position.x += 0.15*sin(ball.rotation*M_PI/180.0f);
    }
    if(fire && !fireflag)
    {
        fireflag=1;
        bullet = Fire(ball.position.x,ball.position.y,ball.rotation,COLOR_ORANGE,1.5);
    }
    if(view_mode==5 && w)
    {
        if(heli_height<20)
            heli_height+=.1;
    }
    if(view_mode==5 && s)
    {
        if(heli_height>1.1)
            heli_height-=.1;
    }
    if(view_mode==5 && a)
    {
        heli_angle--;
    }
    if(view_mode==5 && d)
    {
        heli_angle++;
    }
    if(view_mode==5 && zi)
    {
        if(heli_dist>2.5)
            heli_dist-=0.05;
    }
    if(view_mode==5 && zo)
    {
        heli_dist+=0.1;
    }
}

void tick_elements() {
    ball.tick();
    if(boss.fireflag)
        boss_bullet.tick();

    if(monster_hit>=4)
    {
        if(boss.health<=0)
        {
            boss.hitflag=1;
        }
        if(detect_collision(bullet.bounding_box(),boss.bounding_box()) && !boss.hitflag && fireflag)
        {
            fireflag=0;
            boss.health-=20;
            score+=50;
            disp_score();
        }
        if(detect_collision(ball.bounding_box(),boss.bounding_box()) && !boss.hitflag)
        {
            health-=50;
            boost=0;
            ball.position.x=0;
            ball.position.y=0;
            disp_score();
        }
        if(fireflag && boss.fireflag && detect_collision(bullet.bounding_box(),boss_bullet.bounding_box()))
        {
            fireflag=0;
            boss.fireflag=0;
        }
        if(!boss.hitflag)
        {
            boss.tick();
            float x=boss.position.x-ball.position.x;
            float y=boss.position.y-ball.position.y;
            float dist=sqrt(x*x+y*y);
            if(dist<=70 && !boss.fireflag)
            {
                boss.fireflag=1;
                boss_bullet = Fire(boss.position.x,boss.position.y,(M_PI+boss.rotation)*180/M_PI ,COLOR_RED,1);
            }

            if(boss.fireflag)
            {
                float x1=boss_bullet.position.x;
                float y1=boss_bullet.position.y;
                float x2=boss.position.x;
                float y2=boss.position.y;
                float dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                if(dist>=70)
                    boss.fireflag=0;
                if(detect_collision(ball.bounding_box(),boss_bullet.bounding_box()))
                {
                    health-=30;
                    boss.fireflag=0;
                    disp_score();
                }
            }
        }
    }
    for(int i=0;i<4;i++)
    {
        if(detect_collision(bullet.bounding_box(),mnstr[i].bounding_box()) && !mnstr[i].hitflag && fireflag)
        {
            mnstr[i].hitflag=1;
            score+=50;
            fireflag=0;
            disp_score();
            rock[100+monster_hit]=Rock(mnstr[i].position.x,mnstr[i].position.y,0,0);
            monster_hit++;
        }
        if(detect_collision(ball.bounding_box(),mnstr[i].bounding_box()) && !mnstr[i].hitflag)
        {
            mnstr[i].hitflag=1;
            health-=50;
            boost=0;
            disp_score();
        }
        if(fireflag && mnstr[i].fireflag && detect_collision(bullet.bounding_box(),monster_bullet[i].bounding_box()))
        {
            fireflag=0;
            mnstr[i].fireflag=0;
        }
        if(!mnstr[i].hitflag)
        {
            mnstr[i].tick();
            float x=mnstr[i].position.x-ball.position.x;
            float y=mnstr[i].position.y-ball.position.y;
            float dist=sqrt(x*x+y*y);
            if(dist<=60 && !mnstr[i].fireflag)
            {
                mnstr[i].fireflag=1;
                monster_bullet[i] = Fire(mnstr[i].position.x,mnstr[i].position.y,(M_PI+mnstr[i].rotation)*180/M_PI ,COLOR_RED,0.75);
            }

            if(mnstr[i].fireflag)
            {
                float x1=monster_bullet[i].position.x;
                float y1=monster_bullet[i].position.y;
                float x2=mnstr[i].position.x;
                float y2=mnstr[i].position.y;
                float dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                if(dist>60)
                    mnstr[i].fireflag=0;

                monster_bullet[i].tick();
                if(detect_collision(ball.bounding_box(),monster_bullet[i].bounding_box()))
                {
                    health-=20;
                    mnstr[i].fireflag=0;
                    disp_score();
                }
            }
        }
    }
    if(fireflag)
    {
        bullet.tick();
        float x1=bullet.position.x;
        float y1=bullet.position.y;
        float x2=ball.position.x;
        float y2=ball.position.y;
        float dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
        if(dist>=70)
            fireflag=0;
    }
    for(int i=0;i<100;i++)
    {
        rock[i].tick();
        if(!rock[i].hitflag && detect_collision(ball.bounding_box(),rock[i].bounding_box()) )
        {
            if(rock[i].eflag)
            {
                health-=5;
                rock[i].hitflag=1;
                boost=0;
            }
            else
            {
                score+=10;
                rock[i].hitflag=1;
                GETGIFT(i);
            }
            disp_score();
        }
        if(detect_collision(bullet.bounding_box(),rock[i].bounding_box()) && !rock[i].hitflag)
        {
            if(rock[i].eflag)
            {
                rock[i].hitflag=1;
                score+=10;
                fireflag=0;
            }
            else
            {
                fireflag=0;
            }
            disp_score();
        }
    }
    if(health<=0)
    {
        lives-=1;
        health=100;
        ball.position.x=0;
        ball.position.y=0;
        disp_score();
    }
    for(int i=0;i<monster_hit;i++)
    {
        if(detect_collision(rock[100+i].bounding_box(),ball.bounding_box()) && !rock[100+i].hitflag)
        {
            GETGIFT(100+i);
            rock[100+i].hitflag=1;
            disp_score();
        }
    }
    if(lives==0)
    {
        quit(window);
    }

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball    = Ball(0, 0, COLOR_RED);
    sea     = Sea(0,0,COLOR_BLUE);
    boss    = Boss(0,80,COLOR_RED);
    srand(time(NULL));
    for(int i=0;i<25;i++)
    {
        if(i%9==0)
        rock[i] = Rock(5+rand()%200,5+rand()%200,-1,0);
        else
            rock[i] = Rock(5+rand()%200,5+rand()%200,-1,1);
    }
    for(int i=25;i<100;i++)
    {
        if(i%9==0)
        rock[i] = Rock(-5-rand()%200,5+rand()%200,-1,0);
        else
            rock[i] = Rock(-5-rand()%200,5+rand()%200,-1,1);
    }
    for(int i=50;i<75;i++)
    {
        if(i%9==0)
        rock[i] = Rock(-5-rand()%200,-5-rand()%200,-1,0);
        else
            rock[i] = Rock(-5-rand()%200,-5-rand()%200,-1,1);
    }
    for(int i=75;i<100;i++)
    {
        if(i%9==0)
        rock[i] = Rock(5+rand()%200,-5-rand()%200,-1,0);
        else
            rock[i] = Rock(5+rand()%200,-5-rand()%200,-1,1);
    }
    mnstr[0] = Monster( 30+rand()%100, 30+rand()%100,COLOR_BLACK);
    mnstr[1] = Monster( 30+rand()%100,-30-rand()%100,COLOR_BLACK);
    mnstr[2] = Monster(-30-rand()%100, 30+rand()%100,COLOR_BLACK);
    mnstr[3] = Monster(-30-rand()%100,-30-rand()%100,COLOR_BLACK);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv) {
    srand(time(0));

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t boat, bounding_box_t rock) {
    return (abs(boat.x - rock.x) * 2 < (boat.width + rock.width)) &&
           (abs(boat.y - rock.y) * 2 < (boat.height + rock.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    if(view_mode==1)
        Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    if(view_mode==2 || view_mode==3 || view_mode==4 || view_mode==5)
    {
        GLfloat fov = 90.0f;
        Matrices.projection = glm::perspective (fov, (GLfloat) width / (GLfloat) height, 0.1f, 500.0f);
    }
}
void disp_score()
{
    char prompt_buffer[100]={'S','c','o','r','e',' ','\0'};
    sprintf(score_buffer,"%lld",score);

    char Life_buffer[100]={' ','L','i','f','e',' ','\0'};
    char life_buffer[5]={'\0'};
    sprintf(life_buffer,"%d",lives);

    char health_buffer[100]={' ','H','e','a','l','t','h',' ','\0'};
    char hlth_buffer[5]={'\0'};
    sprintf(hlth_buffer,"%d",health);

    strcat(prompt_buffer,score_buffer);
    strcat(health_buffer,hlth_buffer);

    strcat(Life_buffer,life_buffer);
    strcat(prompt_buffer,Life_buffer);
    strcat(prompt_buffer,health_buffer);

    glfwSetWindowTitle(window,prompt_buffer);
}
void GETGIFT(int i)
{
    if(i==99)
        lives++;
    else if(health==100)
        boost+=.25;
    else
        health=100;
    return;
}
void helimode(int mode)
{
    if(mode==1 && view_mode==5)
        heli_height+=0.1;
    if(mode==2 && view_mode==5)
        heli_height-=0.1;
    if(mode==3 && view_mode==5)
        heli_angle +=5;
    if(mode==4 && view_mode==5)
        heli_angle-=5;
}
