#ifndef __MY_FUNCS_H__
#define __MY_FUNCS_H__
#include"mytypes.h"

extern int slices;
extern int stacks;

void init_Rendering();
void handle_resize(int width, int height);
void mouse_motion(int x,int y);
void mouse_click(int x1,int y1,int x2,int y2);
void display(void);
void key(unsigned char key, int x, int y);
void idle();

#endif
