#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLsizei winWidth=500,winHeight = 500;
char sixel;
float thera=0;
float x=0,y=0,z=0;

void init(void){
    glClearColor(1.0,1.0,1.0,1.0);
}

void displayWirePolyhedra(float x,float y,float z,float thera){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,1.0);
    gluLookAt(5.0,5.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);
    glScalef(1.0,1.0,1.0);
    glTranslatef(1.0,2.0,0.0);
    glutSolidTeapot(1.5);
    glScalef(1.0,1.0,1.0);
    glTranslatef(-1.0,-5.0,0.0);

    glRotatef(thera,x,y,z);
    glutWireTeapot(1.5);
    glFlush();
}

void display(){
    displayWirePolyhedra(x,y,z,thera);
}

void winReshapeFcn(GLint newWidth,GLint newHeight){
    glViewport(0,0,newWidth,newHeight);
    glMatrixMode(GL_PROJECTION);
    glFrustum(-1.0,1.0,-1.0,1.0,2.0,20.0);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);

}

int main(int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//初始化显示模式
    glutInitWindowPosition(100, 100);//设置初始窗口的位置
    glutInitWindowSize(winWidth, winHeight);//设置初始窗口的尺寸
    glutCreateWindow("茶壶");
    init();//初始化
    printf("请选择绕哪一个轴旋转：x,y,z\n");
    scanf("%c",&sixel);
    getchar();
    if(sixel=='x'){
        x=1.0;
        y=0.0;
        z=0.0;
        printf("请输入旋转的角度：\n");
        scanf("%f",&thera);
    }
    else if(sixel=='y'){
        x=0.0;
        y=1.0;
        z=0.0;
        printf("请输入旋转的角度：\n");
        scanf("%f",&thera);
    }
    else if(sixel=='z'){
        x=0.0;
        y=0.0;
        z=1.0;
        printf("请输入旋转的角度：\n");
        scanf("%f",&thera);
    }
    else{
        printf("输入有误\n");
    }
    glutDisplayFunc(display);
    glutReshapeFunc(winReshapeFcn);
    glutMainLoop();//进入程序循环
    return 0;
}
