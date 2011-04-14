#ifndef MYGLDRAWER_H
#define MYGLDRAWER_H

#include <QtOpenGL>
#include <GLUT/glut.h>
#include "marchingcubes.hpp"
class MyGLDrawer : public QGLWidget
{
    Q_OBJECT        // must include this if you use Qt signals/slots

public:
    MyGLDrawer( QWidget *parent = NULL )
        : QGLWidget(parent) {
        // Global variables
        angle = 0.0;
         SX = 200;
         SY = 200;
         SZ = 50;
       metaballs = newFloatMatrix(SX,SY,SZ);
        ball1 = 0;
        ball2 = 0;
        t = 0;
    }
    float angle;
    int SX;
    int SY;
    int SZ;
    float *** metaballs;
    float ball1;
    float ball2;
    int t;
protected:

    void initializeGL()
    {
        // Set up the rendering context, define display lists etc.:

        glClearColor( 0.0, 0.0, 0.0, 0.0 );
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
    }

    void resizeGL( int w, int h )
    {
        int side = (w < h)?w:h;
        glViewport((w - side) / 2, (h - side) / 2, side, side);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
#ifdef QT_OPENGL_ES_1
        glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
             glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
             glMatrixMode(GL_MODELVIEW);
    }

    void paintGL()
    {
        initVoxels(metaballs,SX,SY,SZ);
        drawMetaball(metaballs,SX,SY,SZ,ball1,SY/2,SZ-5,10);
        drawMetaball(metaballs,SX,SY,SZ,ball2,SY/2,SZ-5,10);

        drawMetaball(metaballs,SX,SY,SZ,SX/2,ball1,SZ-5,10);
        drawMetaball(metaballs,SX,SY,SZ,SX/2,ball2,SZ-5,10);

        clearVertexList();
        list<vertex> vertices = runMarchingCubes(metaballs,SX,SY,SZ,6,6,6,1);

        ball1 = sin(t/10.0) * 50 + 50 ;
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
        //glRotatef(angle, 0.0f, 1.0f, 0.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glScalef( 2.0 / SX, 2.0 / SX , 2.0 / SX );
        glColor3f(0.0f, 0.6f, 0.0f);
        glTranslatef( -SX/2.0 , -SY/2.0, -50.0f);
        //glRotatef(80.0f, 0.0f, 0.0f, 1.0f);

        // Draw the triangles
        list<vertex>::iterator it;
        glBegin(GL_TRIANGLES);
            for(it = vertices.begin(); it != vertices.end(); it++) {
                glNormal3d(it->normal_x, it->normal_y, it->normal_z);
                glVertex3d(it->x, it->y, it->z);
            }
        glEnd();
        //cout <<" num verts: " << vertices.size() << endl;
        glPopMatrix();

        glFlush();
        glutSwapBuffers();
        angle += 1;
    }

};


#endif // MYGLDRAWER_H
