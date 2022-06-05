#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define NULL 0
#define WINDOW_HEIGHT 350
#define WINDOW_WIDTH 350

typedef struct dcPt{
    int x;
    int y;
}dcPt;

typedef struct tEdge{
    int yUpper;
    float xIntersect, dxPerScan;
    struct tEdge * next;
}Edge;

void init (void) //设置背景板函数
{
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glMatrixMode (GL_PROJECTION); //设置投影
    gluOrtho2D (0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT); //设置投影区域
}

void setPixel(GLint x,GLint y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}


void insertEdge (Edge *list, Edge *edge)
{
    Edge *p, *q = list;
    p = q->next;
    while (p != NULL)
    {
        if (edge->xIntersect < p->xIntersect)
            p = NULL;
        else{
            q = p;
            p = p->next;
        }
    }
    edge->next = q->next;
    q->next = edge;
}


int yNext (int k, int cnt, dcPt * pts){
    int j;

    if ((k+1) > (cnt-1))
        j = 0;
    else
        j = k + 1;
    while (pts[k].y == pts[j].y)
        if ((j+1) > (cnt-1))
            j = 0;
        else
            j++;
    return (pts[j].y);
}

void makeEdgeRec(dcPt lower, dcPt upper, int yComp, Edge * 	edge, Edge * edges[]){
    edge->dxPerScan =
        (float) (upper.x - lower.x) / (upper.y - lower.y);
    edge->xIntersect = lower.x;
    if (upper.y < yComp)
        edge->yUpper = upper.y - 1;
    else
        edge->yUpper = upper.y;
    insertEdge (edges[lower.y], edge);
}

void buildEdgeList (int cnt, dcPt * pts, Edge * edges[])
{
    Edge * edge;
    dcPt v1, v2;
    int i, yPrev = pts[cnt - 2].y;

    v1.x = pts[cnt-1].x;
    v1.y = pts[cnt-1].y;
    for (i=0; i<cnt; i++)
    {
        v2 = pts[i];
        if (v1.y != v2.y)
        {
            edge = (Edge *) malloc (sizeof (Edge));
            if (v1.y < v2.y)
                makeEdgeRec (v1, v2, yNext (i, cnt, pts), edge, edges);
            else
                makeEdgeRec (v2, v1, yPrev, edge, edges);
        }
        yPrev = v1.y;
        v1 = v2;
    }
}

void buildActiveList (int scan, Edge * active, Edge * edges[])
{
    Edge * p, * q;

    p = edges[scan]->next;
    while (p)
    {
        q = p->next;
        insertEdge (active, p);
        p = q;
    }
}

void fillScan (int scan, Edge * active)
{
    Edge * p1, * p2;
    int i;

    p1 = active->next;
    while (p1)
    {
        p2 = p1->next;
        for (i=p1->xIntersect; i<p2->xIntersect; i++)
            setPixel ((int) i, scan);
        p1 = p2->next;
    }
}

void deleteAfter (Edge * q)
{
    Edge * p = q->next;

    q->next = p->next;
    free (p);
}

void updateList (int scan, Edge * active)
{
    Edge * q = active, * p = active->next;

    while (p)
        if (scan >= p->yUpper)
        {
            p = p->next;
            deleteAfter (q);//删除已经完成的边
        }
        else //更新
        {
            p->xIntersect = p->xIntersect + p->dxPerScan;
            q = p;
            p = p->next;
        }
}

void resortList (Edge * active)
{
    Edge * q, * p = active->next;

    active->next = NULL;
    while (p)
    {
        q = p->next;
        insertEdge (active, p);
        p = q;
    }
}

void fillGraphics (int cnt, dcPt * pts)
{
    Edge *edges[WINDOW_HEIGHT], * active;
    int i, scan;

    for (i=0; i<WINDOW_HEIGHT; i++)
    {
        edges[i] = (Edge *) malloc (sizeof (Edge));
        edges[i]->next = NULL;
    }
    buildEdgeList (cnt, pts, edges);
    active = (Edge *) malloc (sizeof (Edge));
    active->next = NULL;

    for (scan=0; scan<WINDOW_HEIGHT; scan++)
    {
        buildActiveList (scan, active, edges);
        if (active->next)
        {
            fillScan (scan, active);
            updateList (scan, active);
            resortList (active);
        }
    }
}

void Paint (void)
{
    dcPt pts[] =
    {
        100, 50,
        300, 20,
        200, 300,
        200,300,
        200,120
    };

    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (0.0, 0.0, 1.0);
    fillGraphics (5, pts); // (填充图元的顶点数，顶点坐标数组)
    glFlush ( );
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); //初始化显示模式
    glutInitWindowPosition (50, 100); //设置初始窗口的位置
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); //设置初始窗口的尺寸
    glutCreateWindow ("多边形填充");
    init ( ); //初始化
    glutDisplayFunc (Paint);
    glutMainLoop ( );//进入程序循环
    return 0;
}
