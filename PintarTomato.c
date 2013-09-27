#include <stdio.h>
#include <math.h>
#include "GL/glut.h"

// algunes constants per a simplificar el problema
// i per a mes simplificacio considero la regio cubica i equiespaiada

#define ORIG 0
#define DIM 256
#define INC 1.0


/* variables per al viewport de la imatge */
int FimaX=512;
int FimaY=512;

int MinX, MinY, MinZ, ResX, ResY, ResZ;
float IncX, IncY, IncZ;

typedef unsigned char MonVoxels[DIM][DIM][64];

MonVoxels MonActual;
GLenum  doubleBuffer, mode;
int     PLA;
struct punto{
	int x;
	int y;
};


void iniValors() {
  int i,j,k;

  MinX=ORIG;  MinY=ORIG;  MinZ=ORIG;
  IncX=INC;  IncY=INC;  IncZ=INC;
  ResX=DIM;  ResY=DIM;  ResZ=DIM;
  PLA=0;    // per quina llesca comencem a visualitzar

}

void llegirMonVoxels(char* nomFitxer) {
  FILE  *f;
  int i, j, k;
int  vmin, vmax, freqVmin;

  vmin=255;
  vmax=0;
  freqVmin=0;
  f=fopen(nomFitxer,"rb");
  if (!f)    {
      printf("no puc obrir el fitxer %s\n", nomFitxer);
      exit(1);
    }

      for (k=0; k<64; k++)
  for (i=0; i<ResX; i++)
    for (j=0; j<ResY; j++) {
	    fread(&MonActual[i][j][k], sizeof(unsigned char),1,f);
		 if (MonActual[i][j][k]<vmin) vmin=MonActual[i][j][k];
	    if (MonActual[i][j][k]>vmax) vmax=MonActual[i][j][k];
	    if (MonActual[i][j][k]==0) freqVmin++;
    }

  fclose(f);
printf("Model de Punts de Calor. Valor minim %d valor maxim %d freq vmin %d\n",vmin, vmax, freqVmin);
}

void Init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  mode = GL_FALSE;
}

void runlength (int minx,int miny,int maxx,int maxy,int PLANO,int interval){
	int i,j,colormax,colormin,k,tipo;
	float incremento=0;
	int max_filas=4000;
	int muestreo[max_filas][interval];

	// limpio estructura muestreo
	for (i=0;i<max_filas;i++)
		for(j=0;j<interval;j++)
			muestreo[i][j]=0;

	// busco topes de los valores
	colormax=colormin=MonActual[0][0][PLANO];
	for (i=minx;i<maxx;i++)
		for(j=miny;j<maxy;j++){
			if (MonActual[i][j][PLANO]<colormin) colormin =MonActual[i][j][PLANO];
			if (MonActual[i][j][PLANO]>colormax) colormax =MonActual[i][j][PLANO];
		}
	//busco los intervalos
	incremento = (colormax-colormin)/interval;
	tipo =0;
	k=0;

	struct punto pini,pfin;
	pini.x=0;
	pini.y=0;
	pfin.x=0;
	pfin.y=0;

	for (i=minx;i<maxx;i++)
		for(j=miny;j<maxy;j++){
			//				if (MonActual[i][j][PLANO]<15){
			//					if(tipo>0){
			//						k++;
			//						pfin.x=i;
			//						pfin.y=j;
			//					}
			//					muestreo[k][0]++;
			//					tipo=0;
			//				}
			//				if (MonActual[i][j][PLANO]>=15){
			//					if(tipo==0){
			//						// comprobar los extremos de las lineas y dibujar dos lineas si toca
			//						glBegin(GL_LINE_STRIP);
			//						  glColor3f(1.0,1.0,1.0);
			////						  do {
			////							  glVertex3f((float)pini.x,(float)pini.y,0.0);
			////							  pini.x++;
			////						  } while (pini.x < pfin.x);
			//						  glVertex3f((float)pini.x,(float)pini.y,0.0);
			//						  glVertex3f((float)pfin.x,(float)pfin.y,0.0);
			//						glEnd();
			//						 glFlush();
			//						pini.x=i;
			//						pini.y=j; //debo recoger los max i min para pintar
			//					}
			//					muestreo[k][1]++;
			//					tipo=1;
			//				}

			if (MonActual[i][j][PLANO]<colormin+incremento && interval>0){
				if(tipo>0){
					k++;


				}
				muestreo[k][0]++;
				tipo=0;
			}
			if (tipo==0){
				pfin.x=i;
				pfin.y=j;
				glBegin(GL_LINE_STRIP);
				glColor3f(0.2,0.2,0.2);
				glVertex3f((float)pini.x,(float)pini.y,0.0);
				glVertex3f((float)pfin.x,(float)pfin.y,0.0);
				glEnd();
				glFlush();
				pini.x=i;
				pini.y=j; //debo recoger los max i min para pintar
			}
			if (MonActual[i][j][PLANO]>=colormin+incremento && MonActual[i][j][PLANO]<colormin+incremento*2 && interval>1){
				if(tipo>1) {
					k++;

				}
				muestreo[k][1]++;
				tipo=1;
			}
			if (tipo==1){

				pfin.x=i;
				pfin.y=j;
				glBegin(GL_LINE_STRIP);
				glColor3f(0.4,0.4,0.4);
				glVertex3f((float)pini.x,(float)pini.y,0.0);
				glVertex3f((float)pfin.x,(float)pfin.y,0.0);
				glEnd();
				glFlush();
				pini.x=i;
				pini.y=j; //debo recoger los max i min para pintar
			}
			if (MonActual[i][j][PLANO]>=colormin+incremento*2 && MonActual[i][j][PLANO]<colormin+incremento*3 && interval>2){
				if(tipo>2){
					k++;

				}
				muestreo[k][2]++;
				tipo=2;
			}
			if (tipo==2){
				pfin.x=i;
				pfin.y=j;
				glBegin(GL_LINE_STRIP);
				glColor3f(0.6,0.6,0.6);
				glVertex3f((float)pini.x,(float)pini.y,0.0);
				glVertex3f((float)pfin.x,(float)pfin.y,0.0);
				glEnd();
				glFlush();
				pini.x=i;
				pini.y=j; //debo recoger los max i min para pintar

			}
			if (MonActual[i][j][PLANO]>=colormin+incremento*3 && MonActual[i][j][PLANO]<colormin+incremento*4 && interval>3){
				if(tipo>3){
					k++;


				}
				muestreo[k][3]++;
				tipo=3;
			}
			if (tipo==3){
				pfin.x=i;
				pfin.y=j;
				glBegin(GL_LINE_STRIP);
				glColor3f(0.8,0.8,0.8);
				glVertex3f((float)pini.x,(float)pini.y,0.0);
				glVertex3f((float)pfin.x,(float)pfin.y,0.0);
				glEnd();
				glFlush();
				pini.x=i;
				pini.y=j; //debo recoger los max i min para pintar
			}
			if (MonActual[i][j][PLANO]>=colormin+incremento*4 && MonActual[i][j][PLANO]<colormin+incremento*5 && interval>4){
				muestreo[k][4]++;
				tipo=4;
			}

		}

	//	glBegin(GL_LINE_STRIP);
	//	glColor3f(1.0,1.0,1.0);
	//    glVertex3f((float)pini.x,(float)pini.y,0.0);
	//	glVertex3f((float)pfin.x,(float)pfin.y,0.0);
	//	glEnd();
	//	glFlush();

	//	for (i=0;i<450;i++)
	//			printf("%i,%i,%i\n",muestreo[i][0],muestreo[i][1],muestreo[i][2]);
	//	printf(" Valor maximo de cambios : %i \n",k);


	//printf("%i,%i,%i,%i,%i\n",muestreo[i][0],muestreo[i][1],muestreo[i][2],muestreo[i][3],muestreo[i][4]);

}

void Pintar( void) {
	int i,j,figura=0,jmax,jmin,imax,imin;
	// float k;
	float color, px, py;

	glViewport(0,0,FimaX,FimaY);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(MinX,ResX,MinY,ResY);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	printf("          Repinto amb valor de PLA %d \n",PLA);
	imax=jmax=0;
	int tope=15;

	for (j=0; j<ResX-1; j++)
		for (i=0; i<ResY-1; i++)
		{
			glBegin(GL_POLYGON);
			color=(float)MonActual[i][j][PLA];

			if (color>tope){
				if (figura!=1){
					imin=i;
					jmin=j;
				}
				figura=1;
				if(j<jmin) jmin=j;
				if(i>imax) imax=i;
				if(i<imin) imin=i;
				if(j>jmax) jmax=j;
			}

			color=color/150.0;
			glColor3f(color,color,color);
			px=MinX+IncX*i;
			py=MinY+IncY*j;
			glVertex3f((float)px,(float)py,0.0);
			color=(float)MonActual[i+1][j][PLA];
			color=color/150.0;
			glColor3f(color,color,color);
			px=MinX+IncX*(i+1);
			py=MinY+IncY*j;
			glVertex3f((float)px,(float)py,0.0);
			color=(float)MonActual[i+1][j+1][PLA];
			color=color/150.0;
			glColor3f(color,color,color);
			px=MinX+IncX*(i+1);
			py=MinY+IncY*(j+1);
			glVertex3f((float)px,(float)py,0.0);
      color=(float)MonActual[i][j+1][PLA];
      color=color/150.0;
      glColor3f(color,color,color);
      px=MinX+IncX*i;
      py=MinY+IncY*(j+1);
      glVertex3f((float)px,(float)py,0.0);
      glEnd();
    }

  glBegin(GL_LINE_STRIP);
  glColor3f(1.0,1.0,1.0);
  glVertex3f((float)imin,(float)jmin,0.0);
  glVertex3f((float)imax,(float)jmin,0.0);
  glVertex3f((float)imax,(float)jmax,0.0);
  glVertex3f((float)imin,(float)jmax,0.0);
  glVertex3f((float)imin,(float)jmin,0.0);
  glEnd();

  glFlush();

  int restotal=ResX*ResY;
   int resfora=restotal - ((imax-imin)*(jmax-jmin));
   int resdins=((imax-imin)*(jmax-jmin));
   printf("Pixels dins caixa: %i , pixels fora : %i \n",resdins,resfora);

 runlength(imin,jmin,imax,jmax,PLA,3);




}

void Tecles( unsigned char tecla, int x, int y) {
  switch(tecla) {
  case 'a':
    PLA++;    if (PLA>=64) PLA=0;    printf("El PLA val %d \n",PLA);    break;
  case 'z':
    PLA--;    if (PLA<0) PLA=64-1;   printf("El PLA val %d \n",PLA);    break;
  case 27:
    exit(0);    break;
  }
  glutPostRedisplay();
}

void CanviFinestra(int width, int height) {

   if (width>height) width=height;
   else height=width;

   FimaX=width;
   FimaY=height;
   glViewport(0,0,FimaX,FimaY);
}

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
