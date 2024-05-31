#include"myfuncs.h"
#include"consts.h"

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(640,480);
    glutInitWindowPosition(0,0);
    glutCreateWindow("GLUT Shapes");
    glutFullScreen();

    init_Rendering();
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutPassiveMotionFunc(mouse_motion);
    glutMouseFunc(mouse_click);
    glutReshapeFunc(handle_resize);
    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
