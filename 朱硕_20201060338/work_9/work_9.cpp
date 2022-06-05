#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLsizei winWidth = 600, winHeight = 600;
GLint nVerts = 12;//点的个数
GLfloat tx=0, ty=1, tz=3;//平移距离
GLfloat angle = 60;//旋转角度
GLfloat scale = 1.5;//缩放比例

typedef GLfloat M4[4][4];
M4 matComposite;
class pt3D
{
public:
    GLfloat x, y, z;
};
pt3D verts[] =
{
    { 0.0, 1.0, 0.0 }, { -1.0, -1.0, 1.0 }, { 1.0, -1.0, 1.0 },
    { 0.0, 1.0, 0.0 }, { 1.0, -1.0, 1.0 }, { 1.0, -1.0, -1.0 },
    { 0.0, 1.0, 0.0 }, { 1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 },
    { 0.0, 1.0, 0.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, 1.0 }
};//存放三维物体的各个点坐标，
pt3D resultVerts[12];//存放变换后的矩阵
class color
{
public:
    GLfloat r, g, b;
};
color colors[] =
{
    { 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 1.0, 1.0 }
};//每个面的颜色

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
    glMatrixMode(GL_PROJECTION);
}

void m4SetIdentity(M4 matIdentity4x4)
{
    GLint col, row;
    for (row = 0; row < 4; row++)
        for (col = 0; col < 4; col++)
            matIdentity4x4[row][col] = (row == col);
}

void m4PreMultiply(M4 m1, M4 m2)
{
    GLint row, col;
    M4 matTemp;
    for (row = 0; row < 4; row++)
        for (col = 0; col < 4; col++)
            matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col] + m1[row][3] * m2[3][col];
    for (row = 0; row < 4; row++)
        for (col = 0; col < 4; col++)
            m2[row][col] = matTemp[row][col];
}

void translate3D(GLfloat tx, GLfloat ty, GLfloat tz)
{
    M4 matTranslate3D;
    m4SetIdentity(matTranslate3D);
    matTranslate3D[0][3] = tx;
    matTranslate3D[1][3] = ty;
    matTranslate3D[2][3] = tz;
    m4PreMultiply(matTranslate3D, matComposite);
}

void transformVerts3D()
{
    GLint k;
    for (k = 0; k < nVerts; k++)
    {
        resultVerts[k].x = matComposite[0][0] * verts[k].x + matComposite[0][1] * verts[k].y + matComposite[0][2] * verts[k].z + matComposite[0][3];
        resultVerts[k].y = matComposite[1][0] * verts[k].x + matComposite[1][1] * verts[k].y + matComposite[1][2] * verts[k].z + matComposite[1][3];
        resultVerts[k].z = matComposite[2][0] * verts[k].x + matComposite[2][0] * verts[k].y + matComposite[2][2] * verts[k].z + matComposite[2][3];
    }
}

void draw(pt3D *mat)//画三棱锥
{
    int j;
    for (int i = 0; i < 4; i++)
    {
        glColor3f(colors[i].r, colors[i].g, colors[i].b);
        for (j = i * 3; j < i * 3 + 3; j++)
            glVertex3f(mat[j].x, mat[j].y, mat[j].z);
    }
}

void displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(30, 0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);//y轴
    glVertex3f(0, 0, 0);
    glVertex3f(0, 4, 0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(4, 0, 0);//x轴
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 3);//z轴
    glEnd();
    glBegin(GL_TRIANGLES);
    draw(verts);
    glEnd();
    glLoadIdentity();
    GLfloat scaleBack = 1 / scale;
    glRotatef(angle, 0.0, 1.0, 0.0);
    glScalef(1.0, scale, 1.0);
    m4SetIdentity(matComposite);
    translate3D(tx, ty, tz);
    transformVerts3D();
    glBegin(GL_TRIANGLES);
    draw(resultVerts);
    glEnd();
    glScalef(1.0, scaleBack, 1.0);
    glLoadIdentity();
    glFlush();
}

void processSpecialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        ty += 0.1;
        break;
    case GLUT_KEY_DOWN:
        ty -= 0.1;
        break;
    case GLUT_KEY_LEFT:
        tz += 0.1;
        break;
    case GLUT_KEY_RIGHT:
        tz -= 0.1;
        break;
    case GLUT_KEY_PAGE_UP:
        scale += 0.1;
        break;
    case GLUT_KEY_PAGE_DOWN:
        scale -= 0.1;
        break;
    case GLUT_KEY_INSERT:
        angle += 10;
        break;
    case GLUT_KEY_END:
        tx = 0;
        ty = 1;
        tz = 3;
        angle = 60;
        scale = 2;
        break;
    default:
        break;
    }
    displayFunc();
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//初始化显示模式
    glutInitWindowPosition(50, 100);//设置初始窗口的位置
    glutInitWindowSize(winWidth, winHeight);//设置初始窗口的尺寸
    glutCreateWindow("三维图形几何变换");
    init();//初始化
    glutDisplayFunc(displayFunc);
    glutSpecialFunc(processSpecialKeys);
    glutMainLoop();//进入程序循环
    return 0;
}
