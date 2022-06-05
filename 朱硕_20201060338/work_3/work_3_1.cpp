#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>


void init(void)//设置背景板函数
{
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);//设置投影
    glLoadIdentity();
    gluOrtho2D(0.0,200.0,0.0,150.0);//设置投影区域
}
void Bresenham ()
{
    int x1 = 10,y1 = 10,x2 = 100,y2 = 500;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x,y;
    int temp1 = 2 * dy;
    int temp2 = 2 * (dy - dx);
    int p = temp1 - dx;
    if(x1 > x2)
    {
        x = x2;
        y = y2;
        x2 = x1;
    }
    else{
        x = x1;
        y = y1;
    }

    glClear(GL_COLOR_BUFFER_BIT);//清空颜色堆栈
    glColor3f(1.0,0.0,0.0); //设置画线颜色（红色）
    glBegin(GL_LINES);
        glVertex2i(x,y);
        while(x < x2)
        {
            x++;
            if(p < 0)
                p += temp1;
            else
            {
                y++;
                p += temp2;
            }
            glVertex2i(x,y);
        }
    glEnd();
    glFlush();//清空缓冲区指令
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//初始化显示模式
    glutInitWindowPosition(50,100);//设置初始窗口的位置
    glutInitWindowSize(400,300);//设置初始窗口的尺寸
    glutCreateWindow("Bresenham Draw Line");
    init();//初始化
    glutDisplayFunc(Bresenham);
    glutMainLoop();//进入程序循环
    return 0;
}
