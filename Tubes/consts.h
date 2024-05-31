#ifndef __CONSTS_H__
#define __CONSTS_H__
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
using namespace std;

#include "text3d.h"
#define dist(x1,y1,z1,x2,y2,z2) (sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2)))

//Constants
#define GAME_FPS 30
#define BOARD_SIZE 6.0f
#define MAX_LANES 8
#define LVL_NUM_RAND 1000
#define POINTS_ON_ONE_ARROW 1
#define POINTS_ON_ONE_DONUT 20
#define MAX_LEVELS 4

const int POINTS_TO_PASS[MAX_LEVELS] = {300,580,880,1280};
const int LVL_NUM_PROB[MAX_LEVELS] = {22,17,14,10};

#define NUM_MENU_ITEMS 5
#define MENU_TEXT_HEIGHT 0.2
#define MENU_LINE_SPACING 0.07
#define MENU_RGB_R 0.7
#define MENU_RGB_G 0.1
#define MENU_RGB_B 0.1
#define MENU_SEL_RGB_R 0.2
#define MENU_SEL_RGB_G 0.2
#define MENU_SEL_RGB_B 0.7

#define P_START_X 0.0f
#define P_START_Y 0.0f
#define P_START_Z 1.5f
#define P_RADIUS 0.2f
#define P_X_VEL 0.13f
#define P_Y_VEL_MAX 0.40f
#define P_Z_VEL 0.14f
#define P_MAX_HEALTH BOARD_SIZE/2.0
#define P_DEDUCT_H_ON_ONE_ARROW P_MAX_HEALTH/10.0
#define GRAVITY -0.04f

#define P_RGB_R 1
#define P_RGB_G 0
#define P_RGB_B 0

#define P_SLICES 16

#define P_STACKS 16

#define CAM_START_X 0.0f
#define CAM_START_Y 0.0f
#define CAM_START_Z -8.0f
#define CAM_ANGLE 45.0f

#define CLR_CLR_R 0.2f
#define CLR_CLR_G 0.1f
#define CLR_CLR_B 0.4f
#define CLR_CLR_A 1.0f

#define CLR_ARR_R 0.2f
#define CLR_ARR_G 0.3f
#define CLR_ARR_B 0.9f

#define ARR_BASE 0.25f
#define ARR_HEIGHT 0.35f
#define ARR_SLICES 16
#define ARR_STACKS 16
#define ARR_VEL_Z 0.05f

#define D_IN_RADIUS 0.08f
#define D_OUT_RADIUS 0.18f
#define D_RGB_R 0.5f
#define D_RGB_G 0.1f
#define D_RGB_B 0.6f
#define D_SIDES 16
#define D_RINGS 16
#define D_MAX_NUM 1000
#define D_NUM_PROB 10

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 6.0f, 8.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

#endif
