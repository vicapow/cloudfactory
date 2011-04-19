#include "Images.hpp"



static GLubyte c_img[] = {	0xff,0x00,0x00,  0x00,0xff,0x00,
	0x00,0x00,0xff,  0xff,0x00,0xff };

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
	
	assert(ptname!=NULL);
	if (ptname) {
		//*ptname = 0xf0f0f0;
		// initialize the texture
		printf("ptname: %x:%x\n",ptname,ptname[0]);
		glGenTextures(1, ptname);
		printf("ptname: %x:%x\n",ptname,ptname[0]);
		assert( ptname[0] != 0xf0f0f0 );
		glBindTexture(GL_TEXTURE_2D, ptname[0] );

		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,
					 size,size,
					 0,GL_RGB,GL_UNSIGNED_BYTE,img);
		
		//glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,2,2,0,GL_RGB,GL_UNSIGNED_BYTE,c_img);
		
	}
}