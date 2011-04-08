/**
 * Author: Erik Smistad
 * www.thebigblob.com
**/

#include <GLUT/glut.h>
#include "marchingcubes.hpp"
#include <iostream>

using namespace std;

// Global variables
GLfloat angle = 0.0;
vector<vertex> vertices;
int SX = 100;
int SY = 100;
int SZ = 25;
float *** metaballs = newFloatMatrix(SX,SY,SZ);
float ball1 = 0;
float ball2 = 0;
int t = 0;
void renderScene(void) {
	
	ball1 = floor(sin(t/10) * 50 + 0.5 );
	ball2 = SX - ball1;
	//cout << "ball1: " << ball1 << endl;
	t++;
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
     
    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };
     
    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
        
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    // Set material properties which will be assigned by glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glScalef( 1.0 / SX, 1.0 / SX , 1.0 / SX );
    glColor3f(0.0f, 0.6f, 0.0f);
    glTranslatef(-50.0f, -50.0f, -50.0f);
    //glRotatef(80.0f, 0.0f, 0.0f, 1.0f);

    // Draw the triangles
    vector<vertex>::iterator it;
    glBegin(GL_TRIANGLES);
        for(it = vertices.begin(); it < vertices.end(); it++) {
            glNormal3d(it->normal_x, it->normal_y, it->normal_z);
            glVertex3d(it->x, it->y, it->z);
        }
    glEnd();
    glPopMatrix();

	glFlush();
    glutSwapBuffers();
    angle += 0.1;
}

int main(int argc, char **argv) {
    
	// uchar *** raw = //parseRawFile("aneurism.raw", 256, 256, 256);
	// vertices = runMarchingCubes(raw, 
	//             200, 200, 200, 1, 1, 1, 50.0);
	// delete raw;
	
	initVoxels(metaballs,SX,SY,SZ);
	drawMetaball(metaballs,SX,SY,SZ,ball1,SY/2,SZ-5,10);
	drawMetaball(metaballs,SX,SY,SZ,ball2,SY/2,SZ-5,10);
	vertices = runMarchingCubes(metaballs,SX,SY,SZ,6,6,6,1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1024,1024);
    glutCreateWindow("Marching cubes");
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutMainLoop();

    return 0;
}
