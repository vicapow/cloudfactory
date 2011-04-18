#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* checkerboard texture */
#define stripeImageWidth 32
#define checkImageWidth  64
#define checkImageHeight 64
#define smallWidth      256
#define largeWidth     1024

void load_bmp(char *fname, GLubyte img[], int size, GLuint *ptname);
