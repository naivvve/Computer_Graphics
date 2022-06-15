#include <windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

GLsizei winWidth=600,winHeight=600;//初始化显示窗口的大小
const GLdouble pi=3.14159;
GLfloat xwcMin=0.0,xwcMax=225.0;//设置坐标系的显示范围
GLfloat ywcMin=0.0,ywcMax=225.0;
//定义二维点数据结构
class wcPt2D
{
public:
    GLfloat  x,y;
};

typedef GLfloat Matrix3x3 [3][3];

Matrix3x3 matComposite;//定义复合矩阵

void init (void)
{
    glClearColor(1.0,1.0,1.0,0.0);//白色
}

void matrix3x3SetIdentity(Matrix3x3 matIdent3x3)//构建3行3列的单位矩阵
{
    GLint row,col;
    for  (row=0; row<3; row++)
        for  (col=0; col<3; col++)
            matIdent3x3[row][col]=(row==col);
}

void  matrix3x3PreMultiply(Matrix3x3 m1, Matrix3x3 m2)//将m1与矩阵m2相乘,并将结果存放到m2中
{
    GLint  row, col;
    Matrix3x3  matTemp;
    for(row=0; row<3; row++)
        for(col=0; col<3; col++)
            matTemp[row][col]=m1[row][0]*m2[0][col]+m1[row][1]*m2[1][col]+m1[row][2]*m2[2][col];
    for(row=0; row<3; row++)
        for(col=0; col<3; col++)
            m2[row][col]=matTemp[row][col];
}

void translate2D(GLfloat tx,GLfloat ty)//平移变换
{
    Matrix3x3  matTransl;
    /* 初始化平移矩阵为单位矩阵 */
    matrix3x3SetIdentity(matTransl);
    matTransl[0][2]=tx;//tx、ty为平移量
    matTransl[1][2]=ty;
    matrix3x3PreMultiply(matTransl,matComposite);//将平移矩阵前乘到复合矩阵matComposite中
}

void rotate2D(wcPt2D pivotPt, GLfloat theta)//旋转变换，参数为中心点pivotPt和旋转角度theta
{
    Matrix3x3  matRot;
    matrix3x3SetIdentity(matRot);
    matRot[0][0]=cos(theta);
    matRot[0][1]=-sin(theta);
    matRot[0][2]=pivotPt.x*(1-cos(theta))+pivotPt.y*sin(theta);
    matRot[1][0]=sin(theta);
    matRot[1][1]=cos(theta);
    matRot[1][2]=pivotPt.y*(1-cos(theta))-pivotPt.x*sin(theta);
    matrix3x3PreMultiply(matRot,matComposite);//旋转矩阵前乘到复合矩阵matComposite中
}

void scale2D(GLfloat sx,GLfloat sy,wcPt2D fixedPt)//比例变换,参数为基准点fixedPt和缩放比例sx、sy
{
    Matrix3x3  matScale;
    matrix3x3SetIdentity(matScale);
    matScale[0][0]=sx;
    matScale[0][2]=(1-sx)*fixedPt.x;
    matScale[1][1]=sy;
    matScale[1][2]=(1-sy)*fixedPt.y;
    matrix3x3PreMultiply(matScale,matComposite);//缩放矩阵前乘到复合矩阵matComposite中

}

void  transformVerts2D(GLint nVerts,wcPt2D * verts)//利用复合矩阵计算变换后的坐标
{
    GLint k;
    GLfloat temp;
    for(k=0; k<nVerts; k++)
    {
        temp=matComposite[0][0]*verts[k].x+matComposite[0][1]*verts[k].y+matComposite[0][2];
        verts[k].y=matComposite[1][0]*verts[k].x+matComposite[1][1]*verts[k].y+matComposite[1][2];
        verts[k].x=temp;
    }
}

void triangle(wcPt2D * verts)//绘制三角形
{
    GLint  k;
    glBegin(GL_TRIANGLES);
    for(k=0; k<3; k++)
        glVertex2f(verts[k].x,verts[k].y);
    glEnd();
}

void myDisplay()
{
    int choice;
    GLint nVerts=3;
    wcPt2D verts[3]= {{50.0,25.0},{150.0,25.0},{100.0,100.0}};
    wcPt2D centroidPt;
    GLint k,xSum=0,ySum=0;
    for(k=0; k<nVerts; k++)
    {
        xSum+=verts[k].x;
        ySum+=verts[k].y;
    }
    centroidPt.x=GLfloat(xSum)/GLfloat(nVerts);
    centroidPt.y=GLfloat(ySum)/GLfloat(nVerts);
    wcPt2D pivPt,fixedPt;
    pivPt=centroidPt;
    fixedPt=centroidPt;
    GLfloat tx=0.0,ty=100.0;//计算平移参数
    GLfloat sx=0.5,sy=0.5;//计算缩放比例
    GLdouble theta=pi/2.0;
    glClear(GL_COLOR_BUFFER_BIT);//清空窗口
    glColor3f(0.0,0.0,1.0);//设置为蓝色
    triangle(verts);//绘制原蓝色三角形
    printf("请选择要进行的操作：\n1.缩放 \n2.旋转 \n3.平移\n4.缩放+平移 \n5.缩放+旋转 \n6.平移+旋转\n7.平移+缩放+旋转\n");
    scanf("%d",&choice);//进行操作选择
    matrix3x3SetIdentity(matComposite);//初始化
    if(choice==1)
        scale2D(sx,sy,fixedPt);//变换1:缩放变换
    else if(choice==2)
        rotate2D(pivPt,theta);//变换2:旋转变换
    else if(choice==3)
        translate2D(tx,ty);//变换3:平移变换
    else if(choice==4)//变换4:缩放+平移变换
    {
        scale2D(sx,sy,fixedPt);
        translate2D(tx,ty);
    }
    else if(choice==5)//变换5:缩放+旋转变换
    {
        scale2D(sx,sy,fixedPt);
        rotate2D(pivPt,theta);
    }
    else if(choice==6)//变换6:平移+旋转变换
    {
        rotate2D(pivPt,theta);
        translate2D(tx,ty);
    }
    else if(choice==7)//变换7:平移+缩放+旋转变换
    {
        scale2D(sx,sy,fixedPt);
        rotate2D(pivPt,theta);
        translate2D(tx,ty);
    }
    transformVerts2D(nVerts,verts);
    glColor3f(0.0,1.0,1.0); //重新设置变换后的颜色
    triangle(verts); //显示变换后的三角形
    glFlush();
}

void Reshape(GLint newWidth,GLint newHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xwcMin,xwcMax,ywcMin,ywcMax);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char ** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);//初始化显示模式
    glutInitWindowPosition(50,100);//设置初始窗口的位置
    glutInitWindowSize(winWidth,winHeight); //设置初始窗口的尺寸
    glutCreateWindow("Two dimensional graph transformation!");
    init();//初始化
    glutDisplayFunc(myDisplay);//调用myDisplay函数进行图形变换
    glutReshapeFunc(Reshape);
    glutMainLoop();//进入程序循环
    return 0;
}
