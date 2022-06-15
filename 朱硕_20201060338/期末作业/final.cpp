#include <windows.h>
#include <GL/glut.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>

static GLfloat xrot = 0.0f;
static GLfloat yrot = 0.0f;
GLubyte stripeImage[30];//图像数据
GLfloat sgenparams[] = {2.0, 0.0, 0.0, 0.0};

void Texture(void)//纹理设置
{
    int j = 0;
    for (j = 0; j < 10; j++)
    {
        stripeImage[1*j+2] = 1000;
        stripeImage[2*j] = 400 * j;
        stripeImage[1*j+2] = 500;
    }
}

void  RenderScene(void)//渲染场景
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_BACK,GL_FILL);
    glLoadIdentity();
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glRotatef(xrot, 0.0f, 0.1f, 0.0f);
    xrot+=0.1f;
    glRotatef(yrot, 0.1f, 0.0f, 0.0f);
    glutSolidTeapot(50);
    glTranslatef(150,0,0);
    glRotatef(xrot, 0.0f, 0.1f, 0.0f);
    xrot+=0.1f;
    glRotatef(yrot, 0.1f, 0.0f, 0.0f);
    glPopMatrix();
    glutPostRedisplay();
    glutSwapBuffers();
}

void  Projection(GLsizei w,GLsizei h)//投影
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);//投影参数
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();//进行正交投影
    if (w <= h)
        glOrtho (-100.0, 100, -100.0*(GLfloat)h/(GLfloat)w,
                 100.0*(GLfloat)h/(GLfloat)w, -1000.0, 1000.0);
    else
        glOrtho (-100.0*(GLfloat)w/(GLfloat)h,
                 //设置模型参数--几何体参数
                 100.0*(GLfloat)w/(GLfloat)h, -100.0, 100.0, -1000.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Set(void)//设置渲染环境，预加载纹理等
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat light_position[] = { 400.0, 100.0, 500.0, 0.0};//光源的位置
    GLfloat light_ambient[]= {0.0, 0.0, 0.0, 1.0}; //光环境
    GLfloat light_diffuse[]= { 1.0, 1.0, 1.0, 1.0 }; //散射光
    GLfloat light_specular[]= { 1.0, 1.0, 1.0, 1.0 }; //镜面光
    GLfloat light_position1[] = { 0.0, 100.0, 0.0, 1.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    Texture();//调用纹理设置函数
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //纹理映射
    glTexImage1D(GL_TEXTURE_1D, 0, 3, 10, 0, GL_RGB, GL_UNSIGNED_BYTE, stripeImage);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_1D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);//设置材质
    glMaterialf (GL_FRONT, GL_SHININESS, 100.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_LIGHTING);//启用光照

    //远光源照射
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
}

void Spin(int key, int x, int y)//旋转参数
{
    if(key == GLUT_KEY_UP)
        yrot -= 2.0f;
    if(key == GLUT_KEY_DOWN)
        yrot += 3.0f;
    if(key == GLUT_KEY_LEFT)
        xrot -= 2.0f;
    if(key == GLUT_KEY_RIGHT)
        xrot += 3.0f;
}

int main(void)
{
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Teapot");
    glutReshapeFunc(Projection);
    glutSpecialFunc(Spin);
    glutDisplayFunc(RenderScene);
    Set();
    glutMainLoop();
}
