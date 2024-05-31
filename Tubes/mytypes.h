#ifndef __MYTYPES_H__
#define __MYTYPES_H__

typedef enum tagGameStates
{
   GSMenu           = 1,
   GSPlaying        = 2,
   GSControls       = 3,
   GSCredits        = 4,
   GSSplashWelcome  = 5,
   GSSplashEnd      = 6,
   GSQuit           = 7

}GameStates;

typedef enum tagGameErrors
{
    GE_NO_ERROR =0,
    GE_CHARSET_ERROR =1,
}GameErrors;

typedef class tagPlayer
{
    float pos_x,pos_y,pos_z;
    float xVel,yVel,zVel;
    bool isjumping;
public :
    tagPlayer();
    void handle_input(unsigned char key,int x,int y);
    void display();
    void update();

    void get_pos(float& x,float& y,float& z);
    void set_pos(float  x,float  y,float  z);

}Player;

typedef class tagArrow
{
    float pos_x,pos_y,pos_z;
    float xVel,yVel,zVel;
    int   my_lane;
    bool  isAlive;
public:
    tagArrow(int lane);


    bool get_Alive();
    void get_pos(float& x,float& y,float& z);
    int  get_lane();
    void set_pos(float x,float y,float z,bool bound_to_lane = true);
    void set_vel(float xv,float yv,float zv);
    void set_Alive(bool iA);
    void set_lane(int lane);

    void display();
    void update();

    bool isCollided(tagPlayer& p);

}Arrow;

typedef struct tagCamera
{
    float x,y,z;
    float angle;
    float a_x,a_y,a_z;
    tagCamera();
}Camera;

typedef class tagDonut
{
    float pos_x,pos_y,pos_z;
    bool isAlive;
public:
    tagDonut();
    bool taken(Player player);
    void display();

    void set_pos(float x,float y,float z);
    void set_Alive(bool alive);
    bool get_Alive();
    void get_pos(float& x,float& y,float& z);
}Donut;

#endif

