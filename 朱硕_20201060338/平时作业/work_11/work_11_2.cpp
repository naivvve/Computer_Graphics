#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define DEG_TO_RAD 0.017453  //角度转弧度

float theta=30.0; //直线与X轴正向夹角
float length=200.0;  //直线长度
float x=300.0, y=200.0; //直线端点

void init (void)
{
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, 640.0, 0.0, 480.0);
}

void display (void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    glColor3f (1.0, 0.0, 0.0);//设为红色
    glBegin (GL_POLYGON);
    glVertex2f (x, y);
    glVertex2f ( x + length*cos(DEG_TO_RAD*theta),
                 y + length*sin(DEG_TO_RAD*theta) );
    glVertex2f ( x + length*cos(DEG_TO_RAD* (theta+30) ),
                 y + length*sin(DEG_TO_RAD* (theta+30)) );
    glEnd ( );
    glFlush ( );
    //glutSwapBuffers ( );
}

void idleFunc()
{
    theta += 0.1;
    if (theta>360) theta -=360;

    glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//鼠标左键
        theta += 5.0;
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//鼠标右键
        theta -= 5.0;
    if (theta>360) theta -=360;
    if (theta<0) theta +=360;

    glutPostRedisplay();
}

void myKeyboard(unsigned char key,  int x, int y)
{
    if(key == 'a' || key == 'A')
        theta += 5.0;
    if(key == 's' || key == 'S')
        theta -= 5.0;
    if(key == 'c' || key == 'C')
        exit(0);
    if (theta>360) theta -=360;
    if (theta<0) theta +=360;

    glutPostRedisplay();
}

void mySpecialKeyboard( int key,  int x, int y)
{
    if(key == GLUT_KEY_F1 || key == GLUT_KEY_LEFT)
        theta += 5.0;
    x+=100;
    if(key == GLUT_KEY_F2 || key == GLUT_KEY_RIGHT)
        theta -= 5.0;
    if (theta>360) theta -=360;
    if (theta<0) theta +=360;

    glutPostRedisplay();
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);//单缓存
    //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);//双缓存
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (640, 480);
    glutCreateWindow ("Mouse and keyboard callback");
    init ( );
    glutDisplayFunc (display);
    //glutIdleFunc(idleFunc); //空闲回调函数
    glutMouseFunc(myMouse); //鼠标
    //glutKeyboardFunc( myKeyboard); //键盘
    //glutSpecialFunc( mySpecialKeyboard);//特殊键盘值回调
    glutMainLoop ( );
    return 0;
}
