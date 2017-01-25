#include<stdio.h>
#include<GL/gl.h>
#include<GL/glut.h>

 #include<bits/stdc++.h>
using namespace std;
int xx1,yy1,xx2,yy2 ;
char s[30];
#define PI 3.1415926535898
/* program to convert int to string*/

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
//end
/* Program entry point */ //everything declared globally
// declaration of node : lst type
typedef struct lst{
int item ; 								// value of the node
struct lst *left,*right,*parent ;		// pointers to left , right son and parent
int height ; 							// height of current node
int xx,yy,status=0,mod ;				// x,y coordinates of node
}lst ;

// function to insert value in Binary search tree
lst *insert(lst *root,int x){

lst *p = (lst *)malloc(sizeof(lst)) ; p->item = x ;
if(root==NULL){
    p->left = NULL ;p->right = NULL ;p->height=0 ;
     p->parent = NULL ;return p ;
}
lst *temp ;

if(x<=root->item){
    temp = insert(root->left,x) ;
    root->left = temp ;
    temp->parent = root;
    temp->height = (temp->parent)->height+1 ;
}
else{
    temp = insert(root->right,x) ;
    root->right = temp ;
    temp->parent = root;
    temp->height = (temp->parent)->height+1 ;
}
return root;
}
int modifier[20],next_pos[30],max_height,i,j,x,y,place ;
 lst *current = NULL ;
lst *root= NULL ;


/* Function that returns -1,0,1 depending on whether x */
/* is <0, =0, >0 respectively */
#define sign(x) ((x>0)?1:((x<0)?-1:0))

/* Function to plot a point */
void setPixel(GLint x, GLint y) {
  glBegin(GL_POINTS);
  glVertex2i(x,y);
  glEnd();
} //end of plotting a point
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
} */
// circle using midpoint
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

 // end ofcircle using midpoint
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
/* number printed in it */
void draw(void)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
/* my tree */
// traversal for coordinates ;
current=root ;
current->status = 0 ;
// postorder traversal of tree
while(current!=NULL){    // until complete tree is traversed
    x = current->status ;
 //  draw_circle(current->xx,current->yy);
 xx1 =  (current->xx)*20 + 150; yy1 = (current->yy)*20 +200;
Circle(5,xx1, yy1);
 itoa(current->item,s,10);
drawBitmapText(s,xx1+10,yy1+10,0);
    if(current!=NULL&&current->parent!=NULL){
       // draw(current->xx,current->yy,(current->parent->xx),(current->parent->yy)) ;
       xx2 = (current->parent->xx)*20+ 150; yy2 = (current->parent->yy)*20+ 200;
       bresenham(xx1,yy1,xx2,yy2); //implemented bresenham
          /* glBegin(GL_LINES); //draws a line between 2 vertex.
     glVertex2f(xx1,yy1); glVertex2f(xx2,yy2); //inbuilt line of open gl
glEnd();*/
       }

if(x==0){
    current->status=1;
    if(current->left!=NULL){ // iterating to left subtree
        current=current->left ;
        current->status = 0 ;
    }
}
else if(x==1){ // iterating to right subtree
    current->status=2 ;
   if(current->right!=NULL){
        current=current->right ;
        current->status = 0 ;
    }
}
else if(x==2){ // back to parent
    current=current->parent;
}
}
// end of traversal
glFlush();

}

void init() {
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("ALGORITHM WS");
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  gluOrtho2D(0,1000,0,1000);
}

int main(int argc, char **argv)
{
int t ;
scanf("%d",&t) ;
while(t--){ // inserting nodes in bst
   scanf("%d",&x) ;
    root=insert(root,x) ;
}
for(i=0;i<max_height;i++){
    modifier[i]=0; next_pos[i]=1;
}
 current = root ;
current->status = 0 ;//status 0:first_vis 1:left_vis 2:right_vis

while(current!=NULL){ // till complete tree is visited
x = current->status ;
max_height=(max_height<current->height)?current->height:max_height ;// cheking fpr maximum height
if(x==0){
    current->status = 1 ;
    if(current->left!=NULL){  // iterating to left subtree
        current=current->left ;
        current->status = 0 ;
    }
}
else if(x==1){
    current->status=2 ;
   if(current->right!=NULL){ // iterating to right subtree
        current=current->right ;
        current->status = 0 ;
    }
}
else if(x==2){
    x=current->height ;
    int is_leaf =(current->left==NULL)&&(current->right==NULL) ;
    // place and next_pos gives the next x coordinate available at height x
if(is_leaf){
    place=next_pos[x] ;// if leaf then next available place=next_pos at height x
}
else if(current->left==NULL){
    place = ((current->right)->xx) - 1 ;// no right child so next available place=right-1
}
else if(current->right==NULL){
    place = ((current->left)->xx)+1 ;// no right child so next available place=left+1
}
else{
    place = (current->right->xx+current->left->xx)/2 ; // place is avg of both childs
}
modifier[x]=(modifier[x]>(next_pos[x]-place))?modifier[x]:(next_pos[x]-place) ;// dispacement due to node x
if(is_leaf){
    current->xx =place ;
}
else{
        current->xx=place+modifier[x] ;
}
next_pos[x]=current->xx+2 ; // next available x coordinate  at height x
current->mod=modifier[x] ; // displacement for current node
current=current->parent;
}

}
current=root ;
current->status = 0 ;
int modifier_sum =0 ;

while(current!=NULL){ // till complete tree is visited
    x = current->status ;
    // modifier sum gives net displacement of the node
if(x==0){
    current->xx += modifier_sum ; // adding net displacement of current node
    modifier_sum += current->mod  ; // displacement due to current
        current->yy=2*(max_height - current->height)+1; // setting y coordinate of current
    current->status = 1 ;
    if(current->left!=NULL){ // left subtree
        current=current->left;
        current->status = 0 ;
    }
}
else if(x==1){
    current->status = 2 ;
    if(current->right!=NULL){ // right subtree
        current=current->right ;
        current->status=0;
    }
}
else if(x==2){
    modifier_sum = modifier_sum-current->mod  ; // net displacement changed
current=current->parent ; // iterating back to parent
}
}
  glutInit(&argc, argv);
  init();
 glutDisplayFunc(draw);
  glutMainLoop();
  return 0;
}
