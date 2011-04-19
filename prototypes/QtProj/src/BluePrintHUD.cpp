/*
 *  BluePrintHUD.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "BluePrintHUD.h"

BluePrintHUD::BluePrintHUD( QWidget* parent): QGLWidget(parent) {
	blueprint.push_back( new CloudModel( 100, 100, 100, 10 ) );
	canvas = new MetaballCanvas();
	canvas->init();
	dirty = true;
}

void BluePrintHUD::initializeGL(){
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	
	makeCurrent();
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { -500.0, 500.0, -100.0, 0.0 };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
}


void BluePrintHUD::paintGL(){
	
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	//cout << " BluePrintHUD: redraw " << endl;
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		
		// Set material properties which will be assigned by glColor
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		float specReflection[] = { 0.8, 0.8, 0.8, 1.0f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
		
		//static int t = 0;
		//glColor3f(0,sin(t++),1);
		glColor3f(0,0,1);
		
		if(dirty){
			canvas->draw();
			dirty = false;
		}else{
			canvas->draw_gl();
		}
	glPopMatrix();
	
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	
	glFlush();
	//swapBuffers();
}

void BluePrintHUD::setBluePrint( vector<CloudModel*>& blueprint){
	canvas->removeAllMetaballs();
	for(int i = 0; i < blueprint.size();i++){
		canvas->addMetaball(blueprint[i]);
	}
	dirty = true;
}


void BluePrintHUD::resizeGL( int w, int h )
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 1000.0);
	gluLookAt(canvas->SX/2, canvas->SY/2, canvas->SZ*10 , canvas->SX/2, canvas->SY/2, 0, 0, 1, 0);
	//gluLookAt(100,100,500,100,100,0,0,1,0);
	GLenum error = glGetError();
	if(error!=GL_NO_ERROR){
		cout << "ERROR: at " <<__PRETTY_FUNCTION__ << endl;
	}
}


