#include<windows.h>
#include <stdlib.h>
#include<GL/glut.h>
#include<math.h>

void init(void)//设置背景板函数
{
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);//指定屏幕区域对应的模型坐标范围
}

inline int round (const float a) //取整函数
{
    return int (a+0.5);
}

void setPixel(int x,int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x,y);//指定顶点
    glEnd();
    glFlush();
}

void lineDDA(int x0, int y0, int xend, int yend) //DDA算法
{
    int dx = xend - x0, dy = yend - y0;
    int steps, k;
    float xIncrement, yIncrement, x = x0, y = y0;
    if(fabs(dx) > fabs(dy))
        steps = fabs(dx);
    else
        steps = fabs(dy);
    yIncrement = float(dy) / float(steps);
    xIncrement = float(dx) / float(steps);
    setPixel(round(x), round(y));
    for(k = 0; k < steps; k++)
    {
        y += yIncrement;
        x += xIncrement;
        setPixel(round(x), round(y));
    }
}

void Paint (void)
{
    glClear (GL_COLOR_BUFFER_BIT);  // 清除屏幕颜色，即将屏幕的所有像素点都还原为"底色 "
    glBegin (GL_LINES);
    glColor3f (1.0, 0.0, 0.0);  // 设置为红色
    lineDDA(0,0,100,50);//调用lineDDA函数
    glEnd ( );
    glFlush ( );
}

int main(int argc, char **argv)
{
    glutInit (&argc, argv);//初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);//设置初始窗口的位置
    glutInitWindowSize(400, 300);//设置初始窗口的大小
    glutCreateWindow("An Example OpenGL Program");
    init();
    glutDisplayFunc (Paint);
    glutMainLoop();
    return 0;
}
