

#include<stdio.h>
#include<GL/gl.h>
#include<GL/glut.h>

 #include<bits/stdc++.h>
using namespace std;
int xx1,yy1,xx2,yy2 ;
char s[30];
int mheight = -1;
#define PI 3.1415926535898

/* Function to plot a point */
void setPixel(GLint x, GLint y) {
  glBegin(GL_POINTS);
  glVertex2i(x,y);
  glEnd();
}
/* program to draw character at a co ordinate*/
void drawBitmapText(string s,float x,float y,float z)
{
	int i;
	glRasterPos3f(x, y,z);

	for (i=0; i<s.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, s[i]);
	}
}
/* function to draw circle
void MyCircle(GLfloat cx,GLfloat cy,GLfloat r){
GLint i;
GLdouble theta;
glBegin(GL_POLYGON);
for(i=0;i<100;i++){
    theta = 2*PI*i/100; // angle in radians
    glVertex2f(cx+r*cos(theta), cy+r*sin(theta));
}
glEnd();
} */ //end of circle drawing using polygon
 // start of circle using midpoint
void Circle(int Radius,int xC,int yC)
{
    int P;
    int x,y;
    void Drawc(int x,int y,int xC,int yC);
    P = 1 - Radius;
    x = 0;
    y = Radius;
    Drawc(x,y,xC,yC);
    while (x<=y)
    {
        x++;
        if (P<0)
        {
            P += 2 * x + 1;
        }
        else
        {
            P += 2 * (x - y) + 1;
            y--;
        }
        Drawc(x,y,xC,yC);
    }

}
void Drawc(int x,int y,int xC,int yC)
{
    setPixel(xC + x,yC + y);
    setPixel(xC + x,yC - y);
    setPixel(xC - x,yC + y);
    setPixel(xC - x,yC - y);
    setPixel(xC + y,yC + x);
    setPixel(xC - y,yC + x);
    setPixel(xC + y,yC - x);
setPixel(xC - y,yC - x);
}
//end of circle using midpoint


/* function for bresenham*/
 void bresenham (int x0, int y0, int x1, int y1)
  {
  int dx = abs (x1 - x0);
  int dy = abs (y1 - y0);
  int x, y;
  if (dx >= dy)
  {
   int d = 2*dy-dx;
   int ds = 2*dy;
   int dt = 2*(dy-dx);
       if (x0 < x1)
       {
            x = x0;
            y = y0;
       }
        else
        {
             x = x1;
             y = y1;
             x1 = x0;
             y1 = y0;
        }
       setPixel(x, y);
   while (x < x1)
   {          if (d < 0)
        d += ds;
       else {
             if (y < y1) {
                 y++;
              d += dt;
}
                else {
              y--;
             d += dt;
             }
            }
   x++;
        setPixel(x, y);
       }
       }
       else {
             int d = 2*dx-dy;
             int ds = 2*dx;
             int dt = 2*(dx-dy);
             if (y0 < y1) {
             x = x0;
             y = y0;
             }
                else {
             x = x1;
             y = y1;
             y1 = y0;
             x1 = x0;
             }
            setPixel (x, y);
        while (y < y1)
        {
              if (d < 0)
                 d += ds;
             else {
                      if (x > x1){
                      x--;
                      d += dt;
                   } else {
                      x++;
                      d += dt;
                   }
              }
              y++;
              setPixel (x, y);
        }
       }
  }
//end of bresenham
//program entry point
int minsep = 1;
typedef struct node{
    int info;
    struct node* llink;
    struct node*rlink;
    int xcoord,ycoord;
    int offset;
    int thread;

} node;

typedef struct extreme{
    struct node* addr;
    int off;
    int lev;


}extreme;
/* to get max height*/
void getheight(node* temp,int height)
{
    if(temp == NULL)
    {
        if(height > mheight)
            mheight = height;
        return;
    }
    getheight(temp->llink,height+1);
    getheight(temp->rlink,height+1);
}
//end of get height;
node *root= NULL ;


node *insert(node *root,int x){

node *p = (node *)malloc(sizeof(node)) ; p->info = x ;
if(root==NULL){
    p->llink = NULL; p->rlink = NULL;
     return p;
}
node *temp ;

if(x<=root->info){
    temp = insert(root->llink,x) ;
    root->llink = temp ;

}
else{
    temp = insert(root->rlink,x) ;
    root->rlink = temp ;

}
return root;
}

void setup(node* T,int level,extreme* rmost,extreme* lmost)
{
    /*this procedure implements algorithm TR,assigning relative*/
    /*positionings to all nodes in the tree pointed to by parameter T*/
    node* L;//left son
    node* R;//right son
    extreme *lr = (extreme*)malloc(sizeof(extreme)),*ll = (extreme*)malloc(sizeof(extreme)),*rr = (extreme*)malloc(sizeof(extreme)),*rl = (extreme*)malloc(sizeof(extreme));
    /*lr-> right most node on lowest level of left subtree*/

    int cursep,rootsep,loffsum,roffsum;
    /*cursep->seperation on current level
    current separation at node T
    offset from L & R to T
    */

    if(T == NULL)//avoid selecting as extreme
    {
        lmost->lev = -1;
        rmost->lev = -1;
    }
    else
    {
        T->ycoord = level;
        L = T->llink;//follows contour of left subtree
        R = T->rlink;//follows contour of right subtree
        setup(L,level+1,lr,ll);//position subtrees recursively
        setup(R,level+1,rr,rl);

        if(R == NULL && L == NULL)//leaf
        {   /*a leaf is both leftmost
            and rightmost node on the
            lowest level of the subtree
            consisting of itself*/
            rmost->addr = T;
            lmost->addr = T;
            rmost->lev = level;
            lmost->lev = level;
            rmost->off = 0;
            lmost->off = 0;
            T->offset = 0;

        }
        else//not a leaf
        {
            /*set up for subtree pushing, place roots of
            subtrees minimum distance apart*/
            cursep = minsep;
            rootsep = minsep;
            loffsum = 0;
            roffsum = 0;
            /*now consider each level in turn until one
            subtree is exhausted pushing the subtrees
            apart when necessary*/
            while(L != NULL && R != NULL)
            {
                if(cursep < minsep)//push?
                {
                    rootsep = rootsep + (minsep-cursep);
                    cursep = minsep;
                }
                if(L->rlink != NULL)
                {
                    loffsum = loffsum +  L->offset;
                    cursep = cursep-L->offset;
                    L = L->rlink;

                }
                else
                {
                    loffsum = loffsum-L->offset;
                    cursep = cursep + L->offset;
                    L = L->llink;

                }
                if(R->llink != NULL)
                {
                    roffsum = roffsum - R->offset;
                    cursep = cursep - R->offset;
                    R = R->llink;
                }
                else
                {
                    roffsum = roffsum+R->offset;
                    cursep = cursep+R->offset;
                    R = R->rlink;

                }

            }
            /*set the offset in node T,and include it in
            accumulated offset for L or R*/
            T->offset = (rootsep+1)/2;
            loffsum = loffsum - T->offset;
            roffsum = roffsum+ T->offset;
            /*Update extreme descendents information*/

            if((rl->lev > ll->lev)||(T->llink == NULL))
            {
                lmost = rl;
                lmost->off = lmost->off + T->offset;
            }
            else{

                lmost = ll;
                lmost->off = lmost->off - T->offset;


            }


            if((lr->lev>rr->lev) || T->rlink == NULL)
            {
                rmost = lr;
                rmost->off = rmost->off - T->offset;
            }
            else
            {
                rmost = rr;
                rmost->off = rmost->off + T->offset;
            }



            if(L != NULL && L != T->llink)
            {
                rr->addr->thread = 1;
                rr->addr->offset = abs((rr->off + T->offset)-loffsum);
                if(loffsum-T->offset <= rr->off)
                    rr->addr->llink = L;
                else
                {
                    rr->addr->rlink = L;
                }

            }
            else if(R != NULL && R != T->rlink)
            {
                ll->addr->thread = 1;
                ll->addr->offset = abs((ll->off - T->offset)-roffsum);
                if(roffsum + T->offset >= ll->off)
                    ll->addr->rlink = R;
                else
                    ll->addr->llink = R;
            }

        }
    }
    /*if subtrees of T were of uneven heights,check
    to see if threading is necessary. At most one thread
    needs to be inserted*/


}

void petrify(node* T,int xpos)  /*THIS PROCEDURE PERFORMS A PREORDER TRAVERSAL OF THE TREE*/
                                /* CONVERTING THE RELATIVE OFFSETS TO ABSOLUTE COORDINATES.*/
{
    if(T != NULL)
    {
        T->xcoord = xpos;
        if(T->thread == 1)
        {
            T->thread = 0;
            T->rlink = NULL;/*threaded node must be a leaf*/
            T->llink = NULL;

        }
        petrify(T->llink,xpos-T->offset);
        petrify(T->rlink,xpos+T->offset);

    }
}
void traverse(node* temp)
{
    if(temp == NULL)
        return;
    traverse(temp->llink);
    int xx1 = (temp->xcoord)*25+150;
    int yy1 = (mheight- (temp->ycoord))*25+200; //drawing parallely while traversing
    Circle(3,xx1, yy1);
    itoa(temp->info,s,10);
    drawBitmapText(s,xx1+5,yy1+15,0);
    if(temp->llink != NULL)
    {
        int xx2 = (temp->llink->xcoord)*25+150;
        int yy2 =  (mheight -(temp->llink->ycoord))*25+200;
      // draw line between xx1,yy1 and xx2,yy2
             bresenham(xx1,yy1,xx2,yy2);
       /*glBegin(GL_LINES); //draws a line between 2 vertex.
     glVertex2f(xx1,yy1); glVertex2f(xx2,yy2);
glEnd();*/

    }
     if(temp->rlink != NULL)
    {
        int xx2 = (temp->rlink->xcoord)*25+150;
        int yy2 = (mheight -(temp->rlink->ycoord))*25+200;
 // draw line between xx1,yy1 and xx2,yy2
        bresenham(xx1,yy1,xx2,yy2);
  /*glBegin(GL_LINES); //draws a line between 2 vertex.
     glVertex2f(xx1,yy1); glVertex2f(xx2,yy2);
glEnd();*/
    }
    traverse(temp->rlink);
}
//function for drawing
void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
/* my tree */
traverse(root);
glFlush();
}
//end of drawing function
void init() {
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("ALGORITHM TR");
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  gluOrtho2D(0,1000,0,1000);

}
int main(int argc, char **argv)
{
    int t,x ;
scanf("%d",&t) ;
while(t--){
   scanf("%d",&x) ;
    root=insert(root,x) ;
}
  extreme* lmost = (extreme*)malloc(sizeof(extreme));
  extreme* rmost = (extreme*)malloc(sizeof(extreme));
  //traverse(root);
  setup(root,0,rmost,lmost);
  petrify(root,10);
  glutInit(&argc, argv);
  traverse(root);
  getheight(root,0);
  init();
 glutDisplayFunc(draw);
  glutMainLoop();
  return 0;

}
