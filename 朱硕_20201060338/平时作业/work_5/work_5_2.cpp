#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


typedef float Color[3];

void setpixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
    glVertex2f(x, y);//画点
    glEnd();
}

void getpixel(GLint x, GLint y, Color color) //获取像素点颜色
{
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

int compareColor(Color color1, Color color2) //比较颜色是否相等
{
    if (color1[0] != color2[0] || color1[1] != color2[1] || color1[2] != color2[2]) { return 0; }
    else return 1;
}

void boundaryFill4(int x, int y, Color fillColor, Color boarderColor) {
    Color interiorColor;
    getpixel(x, y, interiorColor);
    if (compareColor(interiorColor, fillColor) == 0 && compareColor(interiorColor, boarderColor) == 0) {
        setpixel(x, y);
        boundaryFill4(x + 1, y, fillColor, boarderColor);
        boundaryFill4(x - 1, y, fillColor, boarderColor);
        boundaryFill4(x, y + 1, fillColor, boarderColor);
        boundaryFill4(x, y - 1, fillColor, boarderColor);
    }

}

void boundaryFill8(int x, int y, Color fillColor, Color boarderColor) {
    Color interiorColor, a, b, c, d;
    getpixel(x, y, interiorColor);
    getpixel(x + 1, y - 1, a);
    getpixel(x, y - 1, b);
    getpixel(x, y + 1, c);
    getpixel(x - 1, y, d);
    int i = 0;
    if (compareColor(a, boarderColor) == 1) i++;
    if (compareColor(b, boarderColor) == 1) i++;
    if (compareColor(c, boarderColor) == 1) i++;
    if (compareColor(d, boarderColor) == 1) i++;
    if (i <= 1) {
        if (compareColor(interiorColor, fillColor) == 0 && compareColor(interiorColor, boarderColor) == 0) {
            setpixel(x, y);
            boundaryFill8(x + 1, y, fillColor, boarderColor);
            boundaryFill8(x, y - 1, fillColor, boarderColor);
            boundaryFill8(x - 1, y, fillColor, boarderColor);
            boundaryFill8(x, y + 1, fillColor, boarderColor);
            boundaryFill8(x + 1, y - 1, fillColor, boarderColor);
            boundaryFill8(x - 1, y - 1, fillColor, boarderColor);
            boundaryFill8(x - 1, y + 1, fillColor, boarderColor);
            boundaryFill8(x + 1, y + 1, fillColor, boarderColor);

        }
    }

}

void polygon() {
    glBegin(GL_LINE_LOOP);
    glLineWidth(5);
    glVertex2f(100, 100);
    glVertex2f(100, 150);
    glVertex2f(150, 100);
    glEnd();
}

void Paint(void) {
    Color fillColor = { 0.0, 1.0, 1.0 };//填充颜色
    Color boarderColor = { 0.0, 1.0, 0.0 };//边界颜色
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 500, 500);
    glColor3fv(boarderColor);
    polygon();
    glColor3fv(fillColor);
    //boundaryFill4(150, 150, fillColor, boarderColor);//设置起点坐标及颜色
    boundaryFill8(110, 110, fillColor, boarderColor);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);//初始化
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RED);
    glutInitWindowSize(350, 350);//设置初始窗口的大小
    glutInitWindowPosition(100, 100);//设置初始窗口的位置
    glutCreateWindow("BoundaryFill");
    glClearColor(1, 1, 1, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glutDisplayFunc(Paint);
    glutMainLoop();
    return 0;

}
