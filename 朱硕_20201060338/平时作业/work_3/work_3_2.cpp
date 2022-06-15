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
void Bresenham1()
{
    glClear(GL_COLOR_BUFFER_BIT);//清空颜色堆栈
    glColor3f(1.0f,0.0f,0.0f);
	glPointSize(2.0f);
	int x0 = 10,y0 = 10,x1 = 150,y1 = 50;
	int x = x0,y = y0;
	int dx = x1 - x0,dy = y1 - y0;
	int e = -dx;
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
	glFlush();
	while(x <= x1)
	{
		glBegin(GL_POINTS);
		glVertex2i(x,y);
		glEnd();
		glFlush();
		x++;
		e += 2 * dy;
		if (e > 0)
		{
			y++;
			e -= 2 * dx;
		}
	}

}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//初始化显示模式
    glutInitWindowPosition(50,100);//设置初始窗口的位置
    glutInitWindowSize(400,300);//设置初始窗口的尺寸
    glutCreateWindow("Bresenham Draw Line");
    init();//初始化
    glutDisplayFunc(Bresenham1);
    glutMainLoop();//进入程序循环
    return 0;
}
