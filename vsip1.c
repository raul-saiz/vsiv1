/*
 * mapa.c
 *
 *  Created on: 29/09/2013
 *      Author: lulz
 */

#include <stdio.h>
#include <math.h>
#include "GL/glut.h"
#include "PintarTomato.h"


//extern intFimaX,FimaY;

int main(int argc, char *argv[]) {
  char *fVolum;
  GLenum  type;

  if (argc<2)    {
      printf("Nom executable model_a_pintar \n");
      return(1);
    }
  else    {
      fVolum = argv[1];
    }
  iniValors();
  llegirMonVoxels(fVolum);

  glutInit(&argc, argv);
  doubleBuffer=GL_FALSE;
  type = GLUT_RGB;
  type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;
  glutInitDisplayMode(type);
  glutInitWindowSize(FimaX,FimaY);
  glutCreateWindow("Model de voxels");
  Init();
  glutKeyboardFunc( Tecles);
  glutDisplayFunc( Pintar);
  glutReshapeFunc(CanviFinestra);

  glutMainLoop();
  return 0;
}
