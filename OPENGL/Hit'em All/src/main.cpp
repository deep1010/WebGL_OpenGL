#include "main.h"
#include "timer.h"
#include "ball.h"
#include "floor.h"
#include "enemy.h"
#include "trap.h"
#include "trampoline.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
void disp_score();
Ball ball1;
Tramp trampoline1;
Tramp trampoline2;
Tramp trampoline3;
Tramp trampoline4;
Enemy enemy[2][10];
Floor flr;
Trap trap1,trap2;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float speed=0.02;
float waterspeed=0.01;
Timer t60(1.0 / 60);
const color_t COLOR_TEAL={0,128,128};
int score=0;
int level=0;
int life=3;
char score_buffer[10]={'\0'};
/* Render the scene with openGL */
/* Edit this function according to your assignment */

void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    trap1.draw(VP);
    ball1.draw(VP);
    flr.draw(VP);
    trampoline1.draw(VP);
    trampoline2.draw(VP);
    if(level==1)
    {
        trampoline3.draw(VP);
        trampoline4.draw(VP);
        trap2.draw(VP);
    }
        for(int i=0;i<10;i++)
        if(!enemy[level][i].hitflag)
            enemy[level][i].draw(VP);

}

void tick_input(GLFWwindow *window) {

    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int lflag=0,rflag=0,dflag=0;
    if(detect_collision(ball1.bounding_box(),trampoline1.bounding_box()) || detect_collision(ball1.bounding_box(),trampoline2.bounding_box()) )
    {
        if(ball1.position.x<=trampoline1.position.x-0.789 || ball1.position.x<=trampoline2.position.x-0.389)
        {
            ball1.speedx*=-0.5;
        }
        else if(ball1.position.y>=trampoline1.position.y || ball1.position.y>=trampoline2.position.y)
        {
            ball1.speedy=0.16;
            ball1.upflag=1;
        }
        else
        {
            ball1.position.y=trampoline1.position.y-0.4;
            ball1.speedy*=-0.5;
        }
    }
    if(level==1 && (detect_collision(ball1.bounding_box(),trampoline3.bounding_box()) || detect_collision(ball1.bounding_box(),trampoline4.bounding_box()) ))
    {
       if(ball1.position.x>=trampoline3.position.x+0.789 || ball1.position.x>=trampoline4.position.x+0.389)
        {
            ball1.speedx*=-0.5;
        }
        else if(ball1.position.y>=trampoline3.position.y || ball1.position.y>=trampoline4.position.y)
        {
            ball1.speedy=0.16;
            ball1.upflag=1;
        }
        else
       {
           ball1.position.y=trampoline3.position.y-0.4;
            ball1.speedy*=-0.5;
       }
    }
    for(int i=0;i<10;i++){
        if(detect_collision(ball1.bounding_box(),enemy[level][i].bounding_box()) && !enemy[level][i].hitflag)
        {
            if(ball1.position.x<=enemy[level][i].position.x-0.389)
            {
                lflag=1;
                ball1.speedx*=-0.5;
                break;
            }
            else if(ball1.position.x>=enemy[level][i].position.x+0.389)
            {
                rflag=1;
                ball1.speedx*=-0.5;
                break;
            }
            else if(ball1.position.y>enemy[level][i].position.y)
            {
                ball1.speedy=0.09;
                enemy[level][i].hitflag=1;
                score+=50;
                disp_score();
                break;
            }
            else
            {
                ball1.position.y=enemy[level][i].position.y-0.4;
                dflag=1;
                ball1.speedy*=-0.5;
                break;
            }
        }
    }
    if (left && !rflag) {
        if(ball1.onwater())
            ball1.speedx = -waterspeed;
        else
            ball1.speedx= -speed;
    }
    if (right && !lflag) {
        if(ball1.onwater())
            ball1.speedx = +waterspeed;
        else
            ball1.speedx = +speed;
    }
    if(up && !ball1.upflag)
    {
        ball1.upflag=1;
        if(ball1.onwater())
            ball1.speedy=.09/2;
        else
            ball1.speedy=.09;
    }
    if(down)
    {
        ball1.speedx*=0.5;
    }
}

void tick_elements() {
    if(ball1.onwater())
    {
        if(ball1.speedx>0)
            ball1.speedx=waterspeed;
        else
            ball1.speedx=-waterspeed;
    }
    ball1.tick();
    ball1.checkheight();
    if(level==1)
    {
        if(enemy[1][9].position.x >= 3.8)
            for(int i=0;i<10;i++)
                enemy[1][i].speedx=-0.01;
        if(enemy[1][6].position.x <= -3.8)
            for(int i=0;i<10;i++)
                enemy[1][i].speedx=0.01;
        for(int i=0;i<10;i++)
            enemy[1][i].tick();
    }
    if(detect_collision(ball1.bounding_box(),trap1.bounding_box()) ||( detect_collision(ball1.bounding_box(),trap2.bounding_box()) && level==1))
    {
        if(life==0)
        {
            quit(window);
        }
        else
        {
            ball1.reset_speedy();
            ball1.position.y+=0.4;
            ball1.speedy=0.04;
            life--;
            disp_score();
        }
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    enemy[0][0]=Enemy(-3,-1.5);
    enemy[0][1]=Enemy(-1,-1.5);
    enemy[0][2]=Enemy(1,-1.5);
    enemy[0][3]=Enemy(3,-1.5);
    enemy[0][4]=Enemy(-2,-0.2);
    enemy[0][5]=Enemy(0,-0.2);
    enemy[0][6]=Enemy(2,-0.2);
    enemy[0][7]=Enemy(-1,1.1);
    enemy[0][8]=Enemy(1,1.1);
    enemy[0][9]=Enemy(0,2.4);
    enemy[1][0]=Enemy(0,-1.5);
    enemy[1][1]=Enemy(-1,-0.2);
    enemy[1][2]=Enemy(1,-0.2);
    enemy[1][3]=Enemy(0,1.1);
    enemy[1][4]=Enemy(2,1.1);
    enemy[1][5]=Enemy(-2,1.1);
    enemy[1][6]=Enemy(-3,2.4);
    enemy[1][7]=Enemy(-1,2.4);
    enemy[1][8]=Enemy(1,2.4);
    enemy[1][9]=Enemy(3,2.4);
    ball1 = Ball(-3, -2.8, COLOR_RED);
    trampoline1 = Tramp(3.8,-2.3,COLOR_TEAL);
    trampoline2 = Tramp(3.4,-2.3,COLOR_TEAL);
    trampoline3 = Tramp(-3.8,-2.3,COLOR_TEAL);
    trampoline4 = Tramp(-3.4,-2.3,COLOR_TEAL);
    flr = Floor(0,-3.5,COLOR_GREEN);
    trap1 = Trap(-2,-2.8,COLOR_BLACK);
    trap2 = Trap(2,-2.8,COLOR_BLACK);
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
    int width  = 1000;
    int height = 1000;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers
        //sprintf(score_buffer, "%d", score); //%d is for integers
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            if(score==500)
            {
                level=1;
                disp_score();
            }
            if(score==1000)
            {
                quit(window);
            }
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

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
void disp_score()
{
    char prompt_buffer[100]={'S','c','o','r','e',' ','\0'};
    sprintf(score_buffer,"%d",score);

    char level_buffer[100]={' ','L','e','v','e','l',' ','\0'};
    char lvl_buffer[5]={'\0'};
    sprintf(lvl_buffer,"%d",level+1);

    char Life_buffer[100]={' ','L','i','f','e',' ','\0'};
    char life_buffer[5]={'\0'};
    sprintf(life_buffer,"%d",life);

    strcat(prompt_buffer,score_buffer);
    strcat(level_buffer,lvl_buffer);
    strcat(Life_buffer,life_buffer);
    strcat(prompt_buffer,level_buffer);
    strcat(prompt_buffer,Life_buffer);
    glfwSetWindowTitle(window,prompt_buffer);
}
