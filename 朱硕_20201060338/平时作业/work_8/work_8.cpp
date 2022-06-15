#include<windows.h>
#include<gl/glut.h>
#include<stdio.h>
#include<stdlib.h>

#define TOP_EDGE 8
#define LEFT_EDGE 1
#define BOTTOM_EDGE 4
#define RIGHT_EDGE 2

int x0, y0, x1, y1;
typedef struct Rectangle {
	double xmin, xmax, ymin, ymax;
};
struct Rectangle rect;

void LineGL(int x0, int y0, int x1, int y1) {
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(x0, y0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(x1, y1);
	glEnd();
}

int CompCode(int x, int y, struct Rectangle rect) {
	int code = 0x00;
	if (y < rect.ymin)
		code = code | 4;
	if (y > rect.ymax)
		code = code | 8;
	if (x > rect.xmax)
		code = code | 2;
	if (x < rect.xmin)
		code = code | 1;
	return code;
}

int cohensutherland(struct Rectangle rect, int& x1, int& y1, int& x2, int& y2) //sutherland算法
{
	int accept=0, done=0, code0, code1, codeout;
	float x, y;
	code0 = CompCode(x1, y1, rect);
	code1 = CompCode(x2, y2, rect);
	do {
		if (!(code0 | code1)) {
			accept = 1;
			done = 1;
		}
		else if (code0 & code1)
			done = 1;
		else {
			if (code0 != 0)
				codeout = code0;
			else
				codeout = code1;
			if (codeout & LEFT_EDGE) {
				y = y1 + (y2 - y1) * (rect.xmin - x1) / (x2 - x1);
				x = (float)rect.xmin;
			}
			else if (codeout & RIGHT_EDGE) {
				y = y1 + (y2 - y1) * (rect.xmax - x1) / (x2 - x1);
				x = (float)rect.xmax;
			}
			else if (codeout & BOTTOM_EDGE) {
				x = x1 + (x2 - x1) * (rect.ymin - y1) / (y2 - y1);
				y = (float)rect.ymin;
			}
			else if (codeout & TOP_EDGE) {
				x = x1 + (x2 - x1) * (rect.ymax - y1) / (y2 - y1);
				y = (float)rect.ymax;
			}
			if (codeout == code0) {
				x1 = x; y1 = y;
				code0 = CompCode(x1, y1, rect);
			}
			else
			{
				x2 = x; y2 = y;
				code1 = CompCode(x2, y2, rect);
			}
		}
	} while (!done);
	if (accept)
		LineGL(x1, y1, x2, y2);
	else {
		x1 = 0; y = 0; x2 = 0; y2 = 0;
		LineGL(x1, y1, x2, y2);
	}
	return accept;
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 0.0f);//设置颜色
	glRectf(rect.xmin, rect.ymin, rect.xmax, rect.ymax);
	LineGL(x0, y0, x1, y1);
	glFlush();
}

void Init() { //初始化函数
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glShadeModel(GL_FLAT);
	x0 = 300, y0 = 100, x1 = 100, y1 = 350;
	rect.xmin = 150;
	rect.xmax = 350;
	rect.ymin = 150;
	rect.ymax = 350;
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void keyboard(unsigned char key, int x, int y) {//键盘输入选择功能函数
	switch (key) {
	case 'c'://进行直线段的裁剪
		cohensutherland(rect, x0, y0, x1, y1);
		glutPostRedisplay();
		break;
	case 'i'://初始化，重新进行裁剪
		Init();
		glutPostRedisplay();
		break;
	case 'e'://退出
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//初始化显示模式
	glutInitWindowPosition(100, 100);//设置初始窗口的位置
	glutInitWindowSize(640, 480);//设置初始窗口的大小
	glutCreateWindow("Cut Straight segment");
	Init();//初始化
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();//进入程序循环
	return 0;
}
