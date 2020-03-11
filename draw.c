#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"


  double calcX(int radius, double item, double numSides,double off){
    double in = item/numSides * 2.0f*PI;
    return radius * cos(in+off);
  }
  double calcY(int radius, double item, double numSides,double off){
    double in = item/numSides * 2.0f*PI;
    return radius * sin(in +off);
  }
  void drawNgon(int x0, int y0,int radius, int numSides,int offset,screen s, color c){
    double off = offset * PI/180;
    int x = x0+ calcX(radius, -2, numSides,off);
    int y = y0+ calcY(radius, -2, numSides,off);
    int tx = x0+ calcX(radius, -1, numSides,off);
    int ty = y0+ calcY(radius, -1, numSides,off);
    int t2x = x0+ calcX(radius, 0, numSides,off);
    int t2y = y0+ calcY(radius, 0, numSides,off);
    int item = 0;
    while(item < numSides){
      x = x0+ calcX(radius, -2+item, numSides,off);
      y = y0+ calcY(radius, -2+item, numSides,off);
      tx = x0+ calcX(radius, -1+item, numSides,off);
      ty = y0+ calcY(radius, -1+item, numSides,off);
      t2x = x0+ calcX(radius, item, numSides,off);
      t2y = y0+ calcY(radius, item, numSides,off);
        add_curve(x,y,tx,ty,t2x,t2y,10,s,c);
        item++;
    }
  }
void add_circle( struct matrix *edges,
                 double cx, double cy, double cz,
                 double r, double step ) {
                   const double tau = M_PI*2;
                   double t = 0;
                   double tx = cx+r;
                   double ty = cy;
                   double nx,ny;
                   while(t < 1){
                     t+=1/step;
                     if(t>1)t=1;
                     nx = cx+r*cos(t*tau);
                     ny = cy+r*sin(t*tau);
                     add_edge(edges,tx,ty,0,nx,ny,0);
                     tx = nx;
                     ty = ny;
                   }
}

/*======== void add_curve() ==========
Inputs:   struct matrix *edges
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type

Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix edges
====================*/
void add_curve( struct matrix *edges,
                double x0, double y0,
                double x1, double y1,
                double x2, double y2,
                double x3, double y3,
                double step, int type ) {
                  const double iter = 1/step;
                  struct matrix* px = generate_curve_coefs( x0, x1,x2, x3, type);
                  struct matrix* py = generate_curve_coefs( y0, y1,y2, y3, type);
                  double t = 0;
                  double tx = x0;
                  double ty = y0;
                  double nx,ny;
                  while(t < 1){
                    t+=iter;
                    nx = px->m[3][0] + t * (px->m[2][0]+ t *(px->m[1][0]+t *px->m[0][0]));
                    ny = py->m[3][0] + t * (py->m[2][0]+ t *(py->m[1][0]+t *py->m[0][0]));
                    add_edge(edges,tx,ty,0,nx,ny,0);
                    tx = nx;
                    ty = ny;
                  }

}


/*======== void add_point() ==========
Inputs:   struct matrix * points
         int x
         int y
         int z
Returns:
adds point (x, y, z) to points and increment points.lastcol
if points is full, should call grow on points
====================*/
void add_point( struct matrix * points, double x, double y, double z) {

  if ( points->lastcol == points->cols )
    grow_matrix( points, points->lastcol + 100 );

  points->m[0][ points->lastcol ] = x;
  points->m[1][ points->lastcol ] = y;
  points->m[2][ points->lastcol ] = z;
  points->m[3][ points->lastcol ] = 1;
  points->lastcol++;
} //end add_point

/*======== void add_edge() ==========
Inputs:   struct matrix * points
          int x0, int y0, int z0, int x1, int y1, int z1
Returns:
add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
should use add_point
====================*/
void add_edge( struct matrix * points,
	       double x0, double y0, double z0,
	       double x1, double y1, double z1) {
  add_point( points, x0, y0, z0 );
  add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
Inputs:   struct matrix * points
         screen s
         color c
Returns:
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_lines( struct matrix * points, screen s, color c) {

 if ( points->lastcol < 2 ) {
   printf("Need at least 2 points to draw a line!\n");
   return;
 }

 int point;
 for (point=0; point < points->lastcol-1; point+=2)
   draw_line( points->m[0][point],
	      points->m[1][point],
	      points->m[0][point+1],
	      points->m[1][point+1],
	      s, c);
}// end draw_lines









void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {

  int x, y, d, A, B;
  //swap points if going right -> left
  int xt, yt;
  if (x0 > x1) {
    xt = x0;
    yt = y0;
    x0 = x1;
    y0 = y1;
    x1 = xt;
    y1 = yt;
  }

  x = x0;
  y = y0;
  A = 2 * (y1 - y0);
  B = -2 * (x1 - x0);

  //octants 1 and 8
  if ( abs(x1 - x0) >= abs(y1 - y0) ) {

    //octant 1
    if ( A > 0 ) {

      d = A + B/2;
      while ( x < x1 ) {
	plot( s, c, x, y );
	if ( d > 0 ) {
	  y+= 1;
	  d+= B;
	}
	x++;
	d+= A;
      } //end octant 1 while
      plot( s, c, x1, y1 );
    } //end octant 1

    //octant 8
    else {
      d = A - B/2;

      while ( x < x1 ) {
	//printf("(%d, %d)\n", x, y);
	plot( s, c, x, y );
	if ( d < 0 ) {
	  y-= 1;
	  d-= B;
	}
	x++;
	d+= A;
      } //end octant 8 while
      plot( s, c, x1, y1 );
    } //end octant 8
  }//end octants 1 and 8

  //octants 2 and 7
  else {

    //octant 2
    if ( A > 0 ) {
      d = A/2 + B;

      while ( y < y1 ) {
	plot( s, c, x, y );
	if ( d < 0 ) {
	  x+= 1;
	  d+= A;
	}
	y++;
	d+= B;
      } //end octant 2 while
      plot( s, c, x1, y1 );
    } //end octant 2

    //octant 7
    else {
      d = A/2 - B;

      while ( y > y1 ) {
	plot( s, c, x, y );
	if ( d > 0 ) {
	  x+= 1;
	  d+= A;
	}
	y--;
	d-= B;
      } //end octant 7 while
      plot( s, c, x1, y1 );
    } //end octant 7
  }//end octants 2 and 7
} //end draw_line
