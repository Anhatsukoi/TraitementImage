#ifndef M_PI
 #define M_PI 3.14159265358979323846
#endif

#include "pgm.h"
#include <math.h>

float FFTGauss(int u, int v, int N, int M) {
  float sigma = 1;
  float res ;
  float ufloat = (float)u;
  float vfloat = (float)v;
  float Nfloat = (float)N;
  float Mfloat = (float)M;
  res = -2*pow((float)M_PI,2)*pow(sigma,2);
  float u1 = (2*ufloat-Nfloat)/(2*Nfloat);
  float v1 = (2*vfloat-Mfloat)/(2*Mfloat);
  res = res * (pow(u1,2) + pow(v1,2)) ;
  res = exp(res);
  return res;
}

	/*
		Question 1
 	*/
int main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nb,nl,nc, oldnl,oldnc; // Nombre lignes, nombre colonnes, ancien nombre lignes, ancien nombre colonnes
  unsigned char **im2=NULL,** im1=NULL;
  double** im4,** im5, ** im6, ** im7, **im8, **im9,**im10;

    // Pas assez d'arguments
    if (ac < 3) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }

	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
  im1=lectureimagepgm(av[1],&nl,&nc);
  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }

  // Passage en réels
  double**im3=imuchar2double(im1,nl,nc);
  oldnl=nl; oldnc=nc;
	/*  la fft demande des puissances de 2. On padde avec des 0, mais les dimensions nl et nc changent */
  im7=padimdforfft(im3,&nl,&nc); // Partie réelle de l'image pour la FFT
  /*
	On peut aussi directement utiliser
	im7=padimucforfft(im1,&nl,&nc);
	sans convertir im1 en image de réels
  */

  /* Creation des images pour les parties reelles et imaginaires des fft  */
  im4=alloue_image_double(nl,nc); // Partie imaginaire de l'image pour la FFT
  im5=alloue_image_double(nl,nc); // Partie réelle de l'image après FFT
  im6=alloue_image_double(nl,nc); // Partie imaginaire de l'image après FFT

	/* Calcul de la fft de im7,im4, avec shift */
  fft(im7,im4,im5,im6,nl,nc);
  // Multiplication par la FFT de la gaussienne
  /* A FAIRE : faire la gaussienne, prendre sa FFT, multiplier par le truc au-dessus puis faire FFT inverse (déjà fait en dessous ça)*/

  for (int i=0 ; i<nl; i++){
    for (int j=0; j<nc; j++){
      im5[i][j] = im5[i][j]*FFTGauss(i, j, nl, nc);
      im6[i][j] = im6[i][j]*FFTGauss(i, j, nl, nc);
    }

  }

	/* Creation des images pour les parties reelles et imagianires des fft inverses */
  im9=alloue_image_double(nl,nc); // Partie réelle de l'image après FFT et FFT inverse
  im10=alloue_image_double(nl,nc); // Partie imaginaire de l'image après FFT et FFT inverse
        /* Calcul de la fft inverse de im5,im6 */
  ifft(im5,im6,im9,im10,nl,nc);

	/* Conversion en entier8bits de la partie reelle de la fftinverse,
	   Suppresion des 0 qui ont servi a completer en utilisant la fonction crop
	   Sauvegarde au format pgm de cette image qui doit etre identique a 'linverse video
	   car on a realise la suite fftinv(fft(image))*/
  ecritureimagepgm(av[2],crop(imdouble2uchar(im9,nl,nc),0,0,oldnl,oldnc),oldnl,oldnc);
  return EXIT_SUCCESS;
}
