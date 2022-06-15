#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

GLint winWidth = 600,winHeight=600;
GLfloat x0=100.0,y0=50.0, z0=50.0;
GLfloat xref=50.0,yref=50.0,zref=0.0;
GLfloat Vx=0.0,Vy=1.0,Vz=0.0;
GLfloat xwMin=-40.0,ywMin=-60.0,xwMax=40.0,ywMax=60.0;
GLfloat dnear=25.0,dfar=125.0;

void init(void){
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(x0,y0,z0,xref,yref,zref,Vx,Vy,Vz);
    glMatrixMode(GL_PROJECTION);
    glFrustum(xwMin,xwMax,ywMin,ywMax,dnear,dfar);

}

void displayFcn(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,1.0,0.0);
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_LINE);
    glBegin(GL_QUADS);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(100.0,0.0,0.0);
    glVertex3f(100.0,100.0,0.0);
    glVertex3f(0.0,100.0,0.0);
    glEnd();
    glFlush();
}

void reshapeFcn(GLint newWidth,GLint newHeight){
    glViewport(0,0,newWidth,newHeight);
    winWidth=newWidth;
    winHeight=newHeight;
}

int main (int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//初始化显示模式
    glutInitWindowPosition(50, 50);//设置初始窗口的位置
    glutInitWindowSize(winWidth, winHeight);//设置初始窗口的尺寸
    glutCreateWindow("Perspective View of A Square");
    init();//初始化
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutMainLoop();//进入程序循环
    return 0;
}
