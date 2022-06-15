#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

void init(void)//设置背景板函数
{
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D(-500, 500, -500, 500);//指定屏幕区域对应的模型坐标范围
}

void setPixel(int x,int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x,y);//指定顶点
    glEnd();
    glFlush();
}

void circlePoint(int x0,int y0,int x,int y)//八分法画圆
{
    setPixel(x+x0,y+y0);
    setPixel(y+y0,x+x0);
    setPixel(-y+y0,x+x0);
    setPixel(-x+x0,y+y0);
    setPixel(-x+x0,-y+y0);
    setPixel(-y+y0,-x+x0);
    setPixel(y+y0,-x+x0);
    setPixel(x+x0,-y+y0);
}

void MidBresenhamcircle(int x0,int y0,int r)
{
    int x,y,d;
    d=1.25-r;
    x=0;
    y=r;
    while(x<sqrt(2)/2*r)
    {
        circlePoint(x0,y0,x,y);
        if(d<0) //判断d的符号
            d+=2*x+3;
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
    }
}
/*改进中点算法
void MidBresenhamcircle(int x0,int y0,int r)
{
    int x,y,d;
    d=1-r;
    x=0;
    y=r;
    while(x<sqrt(2)/2*r)
    {
        circlePoint(x0,y0,x,y);
        if(d<=0) //判断d的符号
            d+=2*x+3;
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
    }
}
*/

void Paint (void)
{
    glClear (GL_COLOR_BUFFER_BIT);//清除屏幕颜色，即将屏幕的所有像素点都还原为底色
    glColor3f (0.0, 1.0, 1.0);// 设置颜色
    glBegin (GL_LINES);
    MidBresenhamcircle(1,1,300);//调用函数
    glEnd ( );
    glFlush ( );
}

int main(int argc, char **argv)
{
    glutInit (&argc, argv);//初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);//设置初始窗口的位置
    glutInitWindowSize(500, 500);//设置初始窗口的大小
    glutCreateWindow("An circlePoint Program");
    init();
    glutDisplayFunc (Paint);
    glutMainLoop();
    return 0;
}
