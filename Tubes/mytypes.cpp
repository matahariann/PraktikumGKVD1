#include"mytypes.h"
#include"consts.h"

tagPlayer::tagPlayer()
{
    pos_x=P_START_X;
    pos_y=P_START_Y;
    pos_z=P_START_Z;
    xVel=0;
    yVel=0;
    zVel=0;
    isjumping = false;
}

void tagPlayer::handle_input(unsigned char key,int x,int y)
{
    switch(key)
    {
    case 'd' :
        pos_x+=P_X_VEL;
        if(pos_x>((BOARD_SIZE/2.0)-(P_RADIUS)))
            pos_x = (BOARD_SIZE/2.0) - P_RADIUS;
        break;

    case 'a' :
        pos_x-=P_X_VEL;
        if((-pos_x)>((BOARD_SIZE/2.0)-(P_RADIUS)))
            pos_x = -((BOARD_SIZE/2.0) - P_RADIUS);
        break;

    case 's' :
        pos_z+=P_Z_VEL;
        if(pos_z>((BOARD_SIZE/2.0)-(P_RADIUS)))
            pos_z = (BOARD_SIZE/2.0) - P_RADIUS;
        break;

    case 'w' :
        pos_z-=P_Z_VEL;
        if((-pos_z)>((BOARD_SIZE/2.0)-(P_RADIUS)))
            pos_z = -((BOARD_SIZE/2.0) - P_RADIUS);
        break;

    case ' ':
        if(!isjumping)
        {
            yVel = P_Y_VEL_MAX;
            isjumping = true;
        }
        break;
    }

}

void tagPlayer::update()
{
    pos_y += yVel;
    if(isjumping)
    {
        yVel += GRAVITY;
        if(pos_y<(P_RADIUS))
        {
            isjumping = false;
            yVel = 0;
            pos_y = P_RADIUS;
        }
    }

}

void drawDisk(float radius, float height) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    gluDisk(quad, 0.0, radius, 50, 50);
    glTranslatef(0.0f, height, 0.0f);
    gluDisk(quad, 0.0, radius, 50, 50);
    gluCylinder(quad, radius, radius, height, 50, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void drawUFO() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f); 
    
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, -0.1f, 0.0f);
    drawDisk(0.5f, 0.1f);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluSphere(quad, 0.3f, 50, 50);
    glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
}


void tagPlayer::display() {
    glPushMatrix();
    glColor3f(P_RGB_R, P_RGB_G, P_RGB_B);
    glTranslatef(pos_x, pos_y, pos_z);
    glTranslatef(0, P_RADIUS / 2, 0);
    drawUFO();
    glPopMatrix();
}

void tagPlayer::get_pos(float& x,float& y,float& z)
{
    x = pos_x;
    y = pos_y;
    z = pos_z;
}

void tagPlayer::set_pos(float  x,float  y,float  z)
{
    pos_x = x;
    pos_y = y;
    pos_z = z;
}

tagArrow::tagArrow(int lane)
{
    pos_x =0;
    pos_y =0;
    pos_z =0;
    xVel  =0;
    yVel  =0;
    zVel  =0;
    isAlive = false;
    my_lane = lane;
}

bool tagArrow::get_Alive()
{
    return isAlive;
}

int tagArrow::get_lane()
{
    return my_lane;
}

void tagArrow::set_lane(int lane)
{
    my_lane = lane;
}

void tagArrow::set_pos(float x,float y,float z,bool bound_to_lane)
{
    pos_x = x;   
    pos_y = y;
    pos_z = z;

    if(bound_to_lane)
    {
        pos_x = ((float)(2*my_lane + 1)) * ((BOARD_SIZE/MAX_LANES)/2.0)  + (-BOARD_SIZE/2);
    }
}

void tagArrow::set_Alive(bool iA)
{
    isAlive = iA;
}

void tagArrow::set_vel(float xv,float yv,float zv)
{
    xVel = xv;
    yVel = yv;
    zVel = zv;
}

void drawArrow(){
	GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);     

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluSphere(quad, 0.3f, 50, 50);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.3f, 0.0f);
    gluSphere(quad, 0.1f, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.3f, 0.0f, 0.0f);
    gluSphere(quad, 0.1f, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, -0.3f, 0.0f);
    gluSphere(quad, 0.1f, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -0.3f);
    gluSphere(quad, 0.1f, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-0.3f, 0.0f, 0.0f);
    gluSphere(quad, 0.1f, 20, 20);
    glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void tagArrow::display()
{
    if(!isAlive)
        return;
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glTranslatef(pos_x,pos_y,pos_z);
    glScalef(1.5, 1.5, 1.5);
    drawArrow();
    glPopMatrix();
}

void tagArrow::get_pos(float& x,float& y,float &z)
{
    x = pos_x;
    y = pos_y;
    z = pos_z;
}

void tagArrow::update()
{
    if(!isAlive)
        return;
    pos_x+=xVel;
    pos_y+=yVel;
    pos_z+=zVel;
}

bool tagArrow::isCollided(tagPlayer& p)
{
    if(!isAlive)
        return false;
    float x,y,z;
    p.get_pos(x,y,z);
    if((pos_x + ARR_BASE) < (x - P_RADIUS))  
        return false;
    if((pos_x - ARR_BASE) > (x + P_RADIUS))   
        return false;
    if((pos_y + ARR_BASE) < (y - P_RADIUS)) 
        return false;
    if((pos_y - ARR_BASE) > (y + P_RADIUS)) 
        return false;
    if((pos_z + ARR_HEIGHT) < (z - P_RADIUS))  
        return false;
    if((pos_z) > (z + P_RADIUS))   
        return false;
    return true;
}

tagCamera::tagCamera()
{
    x=CAM_START_X;
    y=CAM_START_Y;
    z=CAM_START_Z;
    angle = CAM_ANGLE;
    a_x = 1.0f;
    a_y = 0.0f;
    a_z = 0.0f;
}

tagDonut::tagDonut()
{
    isAlive = false;
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
}

bool tagDonut::get_Alive()
{
    return isAlive;
}

void tagDonut::get_pos(float& x,float& y,float& z)
{
    x = pos_x;
    y = pos_y;
    z = pos_z;
}

void tagDonut::set_Alive(bool alive)
{
    isAlive = alive;
}

void tagDonut::set_pos(float x,float y, float z)
{
    pos_x = x;
    pos_y = y;
    pos_z = z;
}

bool tagDonut::taken(Player player)
{
    if(!isAlive)
        return false;
    float x,y,z;
    player.get_pos(x,y,z);
    float d = dist(pos_x,pos_y,pos_z,x,y,z);
    if(d<=(P_RADIUS + D_OUT_RADIUS))
    {
        isAlive = false;
        return true;
    }
    return false;
}

void tagDonut::display()
{
    if(!isAlive)
        return;
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.84f);
    glTranslatef(pos_x,pos_y,pos_z);
    glRotatef(90,1,0,0);
    glScalef(0.25, 0.25, 0.25);
    glutSolidIcosahedron();
    glPopMatrix();
}

