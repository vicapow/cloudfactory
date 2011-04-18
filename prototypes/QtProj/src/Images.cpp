#include "Images.hpp"

/* rbg --> rgb? */
void bmp2rgb(GLubyte img[], int size) {
	int i;
	GLubyte temp;
	
	for (i=0; i<size; i+=3) {
		temp = img[i+2];
		img[i+2] = img[i+1];
		img[i+1] = temp;
		
	}
}

void load_bmp(char *fname, GLubyte img[], int size, GLuint *ptname) {
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	FILE *fp;
	
	fp = fopen(fname,"rb") ;
	if(fp == NULL) {
		fprintf(stderr,"unable to open texture file %s\n", fname) ;
		exit(1) ;
	}
	
	fseek(fp,8,SEEK_SET) ;
	fread(img,size*size*3,1,fp) ;
	bmp2rgb(img, size*size*3);
	fclose(fp) ;
	
	if (ptname) {
		// initialize the texture
		glGenTextures(1, ptname) ; 
		glBindTexture(GL_TEXTURE_2D, *ptname);
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT) ;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT) ; 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR) ;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR) ;
		
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,
					 size,size,
					 0,GL_RGB,GL_UNSIGNED_BYTE,img) ;
	}
}