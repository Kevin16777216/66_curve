#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main(int argc, char **argv) {

  screen s;
  struct matrix * edges;
  struct matrix * transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  // add_circle(edges,250,250,0,200,.01);
  // add_circle(edges,175,325,0,50,.01);
  // add_circle(edges,325,325,0,50,.01);
  // add_circle(edges,175,325,0,10,.01);
  // add_circle(edges,325,325,0,10,.01);
  // add_curve(edges,200,250,150,50,300,250,300,250,.01,1);
  /* print_matrix(make_rotX(M_PI/4)); */
  /* printf("\n"); */
  /* print_matrix(make_rotY(M_PI/4)); */
  /* printf("\n"); */
  /* print_matrix(make_rotZ(M_PI/4)); */
  /* printf("\n"); */

  if ( argc == 2 ){
    printf("%s\n", "hi");
    parse_file( argv[1], transform, edges, s );
  }else{
    parse_file( "stdin", transform, edges, s );
  }

  free_matrix( edges );
  free_matrix( transform );
}
