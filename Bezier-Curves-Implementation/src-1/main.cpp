#include <iostream>
using std::cerr;
using std::endl;
#include <stdlib.h>
//using std::exit;
#include <GL/glut.h>                      // GLUT stuff, includes OpenGL headers as well
#include <windows.h>
#include <math.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
using namespace std;
#define STEP 0.01
int flag = 0;int p=0;
int SCREEN_HEIGHT = 480;

int NUMPOINTS = 0;                            // Keep track of times clicked, number of points plotted
float radius = 10.0;
                                              // Point type struct to store a 3d point
typedef struct{
	float x,y,z;                              // 3D coordinates of a point
}Point;
Point temp[2000];
Point abc[100];
int closed=1;
void myInit() {
		glClearColor(0.0,0.0,0.0,0.0);
		glColor3f(1.0,0.0,0.0);
		glPointSize(4.0);
		glMatrixMode(GL_PROJECTION);                       // setting up viewing
		glLoadIdentity();
		gluOrtho2D(0.0,640.0,0.0,480.0);

}
void PlotPixel(int x,int y)
{
		glColor3f(0,1,0);
		glBegin (GL_POINTS);
		glVertex2i (x, y);                                  // function to just plot a pixel similar to drawing dot
		glEnd ();
		glFlush ();
}
void drawDot(int x, int y)
{
    	glColor3f(1,0,0);
		glBegin(GL_POINTS);

		glVertex2i(x,y);
		glEnd();
		glFlush();                                             // function to draw a simple dot
}
void Circle8Points(int x, int y,int Xc,int Yc)
 {

		 PlotPixel(Xc + x, Yc + y);
		 PlotPixel(Xc - x, Yc + y);
		 PlotPixel(Xc + x, Yc - y);
		 PlotPixel(Xc - x, Yc - y);
		 PlotPixel(Xc + y, Yc + x);                            // points by symmetry
		 PlotPixel(Xc - y, Yc + x);
		 PlotPixel(Xc + y, Yc - x);
		 PlotPixel(Xc - y, Yc - x);

 }
                                                                 //Implement the Midpoint algorithm
void drawCircle(int Xc,int Yc)
 {
		 int x=0;
		 int y=radius;
		 int p=1-radius;                                                   //initial value of the decision parameter

		 PlotPixel(Xc,Yc);                                                 //Draw circle center for illustration
		 Circle8Points(x,y,Xc,Yc);                                         /* Plot the First point */
		while(x<y)
		 {
		 x++;
		 if(p<0)
		 p +=2 * x + 1;
		 else
		 {
		 y--;
		 p += 2 * (x -y) + 1;
		 }

		Circle8Points(x,y,Xc,Yc);
		 }
		 glEnd();
		 glFlush();
 }
void drawLine(Point p1, Point p2)
{
		glBegin(GL_LINES);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);           // function to draw lines between two coordinates

		glEnd();
		glFlush();
}

 Point lerp(Point p1,Point p2,float t){
 Point p;
 p.x = p1.x*(1-t) + p2.x*t;
 p.y = p1.y*(1-t)+p2.y*t;   // calculating linear interpolation between two points
 p.z = 0;
 return p ;
 }

Point dd[3000] ;
Point drawx(Point c[],int siz,float t){
if(siz==1){Point pp = c[0] ; return pp ;}
for(int i=0;i<siz-1;i++){
    temp[i] = lerp(c[i],c[i+1],t) ; // computing lerps again and again
}
for(int i=0;i<siz-1;i++){
  c[i] =  temp[i] ;
}
return drawx(c,siz-1,t) ;
}
/////end/////////
void drawCurve(Point p1,Point p2,float color){                   //draw the bezier curve
		glColor3f(1.0,0.0,0.0);
		glLineWidth(2.5);
		glBegin(GL_LINES);
			glVertex2f(p1.x,p1.y);                              // draw curves by drawing lines with some width
			glVertex2f(p2.x,p2.y);
		glEnd();
		glFlush();
}
double dis(float a,float b,float c,float d)
{
	    double r ;
		r =sqrt((float)(pow((float)(a-c),2)+pow((float)(b-d),2)));       // calculating distance between points
		return r;
}
int crit(float x,float y)                                                //checking whether the vertex(x,y) lies within critical region of previous points
{
		int j=0;
		int flag1 =-1;
		for(j=0;j<NUMPOINTS;j++)
		{
			if(dis(x,y,abc[j].x,abc[j].y )<= radius)
			{

				flag1=j;
				                                                           //returns the vertex under whose critical region this vertex falls
			 break ;
			}

		}

		return flag1;
}
void draw()
{
		   glClear(GL_COLOR_BUFFER_BIT);
		   glFlush();
		   int i;float t;
		   Point POld = abc[0];                             // initialisations
		   Point M;

		   glColor3f(1.0,1.0,1.0);


		   for(i=0;i<NUMPOINTS-1;i++)
			{
				drawDot(abc[i].x,abc[i].y);
			   drawCircle(abc[i].x,abc[i].y);                  // draw dots and circles for the points selected and draw lines between them
			   glColor3f(1.0,1.0,1.0);
				drawLine(abc[i],abc[i+1]);
			}
			drawDot(abc[i].x,abc[i].y);
			   drawCircle(abc[i].x,abc[i].y);

			for(t = 0;t <= 1.0; t += STEP) {
                    for(int i=0;i<NUMPOINTS;i++){dd[i] = abc[i] ;}
						 M = drawx(dd,NUMPOINTS,t);
						drawCurve(POld,M,t);                     // call drawBezier function to return a point and draw curves by forming lines
						POld = M;
					}

			glColor3f(1.0,0.0,0.0);
}
void myMouse(int button, int state, int x, int y) {
                                                                         // If left button was clicked
		 int i = 0,r;
		 float t;

		 if(button == GLUT_LEFT_BUTTON&&state == GLUT_DOWN ) {
			  r= crit(x, SCREEN_HEIGHT -y);                                     //crit function returns index of the vertex if it is clicked in its critical
																			  // region

		 if(flag == 1 && r != -1) {
			  p=r;
			  flag = 2;
			  closed=0;                                                        // if it is new vertex to which our curve is to be shifted start passive motion
			  return;
			}
		 else if(flag == 2 )  {

			  flag = 1;
			  closed=1;
			  abc[p].x = (float)x;                                          // replace the old dragged vertex with new vertex
			  abc[p].y = (float )(SCREEN_HEIGHT-y);
			  abc[p].z = 0;
			  draw();
			  return;
			}

		 else
		  {

			  abc[NUMPOINTS].x = (float)x;
			  abc[NUMPOINTS].y = (float)(SCREEN_HEIGHT - y);
			  abc[NUMPOINTS].z = 0;                                       // first control points of curve are done here
			  NUMPOINTS++;

																		  // Draw the red  dot and circle for the control points
			  drawDot(x, SCREEN_HEIGHT - y);

			  drawCircle(x, SCREEN_HEIGHT - y);


		  }
		  }



		  else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		  {

																		  // flag to indicate that right click is over and curve should appear
			   flag = 1;                                                      //  and then go to draw function
			   draw();

		  }

 }

void myPassiveMotion(int x, int y)
{
		 if(!closed){
		 abc[p].x = x;
		 abc[p].y = SCREEN_HEIGHT - y;                                     // passive motion required to dra with mouse
		 draw();
 }

}

void myDisplay() {

		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();                                                     //display function
}
void keyPressed (unsigned char key, int x, int y) {
    if(key=='d'){
       int r = crit(x, SCREEN_HEIGHT -y);
    for(int j=r+1;j<NUMPOINTS;j++){
        abc[j-1] = abc[j] ;
    }
    NUMPOINTS-- ;
    draw() ;
    }
}
int main(int argc, char *argv[]) {

		glutInit(&argc, argv);                                      //initialise toolkit

		glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);                  // set display mode.

		glutInitWindowSize(640,480);
		glutInitWindowPosition(100,150);                            // set window size and position with title "Bezier curve"
		glutCreateWindow("BezierCurve task1and 2");
        glutKeyboardFunc(keyPressed);
		glutMouseFunc(myMouse);
		glutPassiveMotionFunc(myPassiveMotion);                     // register the call back functions
		glutDisplayFunc(myDisplay);

		myInit();                                                  // go into a perpetual loop
		glutMainLoop();

		return 0;
}
