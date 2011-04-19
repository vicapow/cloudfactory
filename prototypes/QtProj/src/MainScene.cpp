/*
 *  MainScene.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/15/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "MainScene.h"
#include "Images.hpp"
#include "Cannons.hpp"

GLenum __errCode;
const GLubyte* __errString;
#define ASSERT_GL_ERR { if((__errCode = glGetError()) != GL_NO_ERROR) { __errString = gluErrorString(__errCode); fprintf(stderr,"OpenGL Error: %s at %s \n\ton line: %d\n",__errString,__PRETTY_FUNCTION__,__LINE__); exit(EXIT_FAILURE); }  }

/* globals used to determine state of game */

static bool STATE_GROW_A, STATE_GROW_B, STATE_GROW_C;
static bool STOP_GROWTH_A, STOP_GROWTH_B, STOP_GROWTH_C;
static char CUR_KEY;

static Wm5::Float3 location_c, location_b, location_a;
static Wm5::Float3 incrementer_c, incrementer_b, incrementer_a;

GLfloat posX, posY, posZ;

void MainScene::print_gl_err(){
	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf (stderr, "OpenGL Error: %s\n", errString);
		exit(EXIT_FAILURE);
	}
}

MainScene::MainScene( QGLContext* _gl_context ) : gl_context( _gl_context ) {
	
	//setup the time for updating the GL scene
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(update()) );
	timer->start(0);
	angle = 0;
	correctness = 0;
	elapsedTimer.start();
	
	glutInitDisplayMode (GLUT_DOUBLE);
		
	hud = new HUDWidget();
	
	blueprint_hud = new BluePrintHUD();

	this->addWidget(hud);
	hud->setContentsMargins(0, 0, 0, 0);
	hud->setToolTip("Use this blueprint to create clouds");
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(1,1,1,1);
	hud->setLayout(layout);
	layout->addWidget(blueprint_hud);
	
	create_scene();

}

void MainScene::setBlueprint( const vector<CloudModel*>& blueprint ){
	this->blueprint = blueprint;
	blueprint_hud->setBluePrint( this->blueprint );
}

void MainScene::onEnterFrame(){

	int frame_time = elapsedTimer.restart();
	list<m_cloud*>::iterator it;
	for( it = clouds.begin(); it != clouds.end(); it++){
		
		m_cloud* cloud = (*it);//convient pointer
		
		//take a step
		cloud->update(frame_time/25.0f);
		
		//remove the cloud if it goes beyond the edge of the screen
		if( cloud->model->posY - cloud->model->getRadius() > canvas->SY ){
			
			this->remove_metaball(cloud->model);
			
			//remove the cloud from the clouds list
			it--;//update the intorator to reflect the missing cloud
			clouds.remove(cloud);
			delete cloud;
		}
	}
	
	if(STATE_GROW_A && cur_cloud_a ){
		cur_cloud_a->model->posY = 0;
		cur_cloud_a->model->incRadius(1);
	}
	
	if(STATE_GROW_B && cur_cloud_b ){
		cur_cloud_b->model->posY = 0;
		cur_cloud_b->model->incRadius(1);
	}
	
	if(STATE_GROW_C && cur_cloud_c ){
		cur_cloud_c->model->posY = 0;
		cur_cloud_c->model->incRadius(1);
	}
	
	//detect pattern
	correctness = BluePrintDetect::CalculateError( blueprint , user_guess );
	if(correctness>0.9){
		clearClouds();
		emit onLevelPassed();
	}
	ASSERT_GL_ERR;
	draw_GL();
	ASSERT_GL_ERR;
}

void MainScene::create_scene(){
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	//load_bmp("../../resources/bg2.bmp", tex_byte, 256, textures );
	
	textures[0] = gl_context->bindTexture(QImage("../../resources/bg2.jpg"));
	
//	set camera positions
	
//	
//	can_ind = glGenLists(3);
//	
//	/* cannon 1 */
//	glNewList(can_ind, GL_COMPILE);
//	
//	APoint trans = APoint(2.0, -4.0, 0.0);
//	APoint scale = APoint(1.0, 1.0, 1.0);
//	
//	CreateCannon(trans, scale);
//	
//	glEndList();
//	
//	/* cannon 2 */
//	
//	glNewList(can_ind+1, GL_COMPILE);
//	
//	trans = APoint(0.0, -4.0, 0.0);
//	scale = APoint(1.0, 1.0, 1.0);
//	
//	CreateCannon(trans, scale);
//	
//	glEndList();
//	
//	
//	/* cannon 2 */
//	
//	glNewList(can_ind +2, GL_COMPILE);
//	
//	trans = APoint(-2.0, -4.0, 0.0);
//	scale = APoint(1.0, 1.0, 1.0);
//	
//    glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//    glLoadIdentity();
//	glScalef( 100,100,100 );
//	glTranslatef(100,0,0);
//	CreateCannon(trans, scale);
//	glPopMatrix();
//	
//	glEndList();
//	
//	glEnable(GL_LIGHTING);
//	glEnable(GL_COLOR_MATERIAL);
//	
	
	canvas = new MetaballCanvas();
	canvas->init();
}

void MainScene::draw_GL(){
		
	ASSERT_GL_ERR;

	glViewport(0, 0, (GLsizei) width(), (GLsizei) height() );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat) width()/(GLfloat) height(), 1.0, 1000.0);
	gluLookAt(canvas->SX/2, canvas->SY/2, canvas->SZ*10 , canvas->SX/2, canvas->SY/2, 0, 0, 1, 0);
	
	glClearColor(0.21, 0.385, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LIGHTING);
	
	glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
	ASSERT_GL_ERR;
	
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 0.0, correctness - 0.5, 0.0, 1.0 };
	GLfloat light_specular[] = { 0.0, correctness - 0.5, 0.0, 1.0 };
	GLfloat light_position[] = { -500.0, 500.0, 500.0, 0.0 };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	
	ASSERT_GL_ERR;
	
	
		// Set material properties which will be assigned by glColor
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		float specReflection[] = { 0.8, 0.8, 0.8, 1.0f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
		glColor3f(1-correctness+0.5,1,1-correctness+0.5 );
	
		// draw background here
		display_image(970, 650);
		glMatrixMode(GL_MODELVIEW);
		
		
		//glColor3f(1,0,0);
		
		//cout << "correctness: " << correctness << endl;
		
	ASSERT_GL_ERR;
	
		// Draw the triangles
		canvas->draw();
	
	ASSERT_GL_ERR;
	glPopMatrix();//extra pop?
	ASSERT_GL_ERR;
	
	glFlush();
	glutSwapBuffers();
	
	ASSERT_GL_ERR;
	
}

void MainScene::drawBackground(QPainter *painter, const QRectF &)
{	
	ASSERT_GL_ERR;
	onEnterFrame();
	ASSERT_GL_ERR;
}

void MainScene::remove_metaball(CloudModel* model ){
	
	//remove the model from the guess vector
	for(unsigned int i = 0; i < user_guess.size();i++){
		if(user_guess[i] == model){
			user_guess.erase( user_guess.begin() + i );
			break;
		}
	}
	
	//remove the cloud from the metaball canvas
	canvas->removeMetaball(model);
}

void MainScene::display_image(int width, int height)
{	
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);  	
	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3f(0, 0,0);
			glTexCoord2f(1.0f,0.0f); glVertex3f(width,0,0);
			glTexCoord2f(1.0f,1.0f); glVertex3f(width,height,0);
			glTexCoord2f(0.0f,1.0f); glVertex3f(0,height,0);
		glEnd();
	glPopMatrix();
		
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}

void MainScene::onKeyPress(QKeyEvent* event){
	
	static float DEFAULT_RADIUS = 5;
	
	switch (event->key()) {
			
		case 'A': // cannon to the far right
		case 'a':
		{
			if(CUR_KEY != 'a'){
				
				//add a cloud from factory A
				clouds.push_back( cur_cloud_a = new m_cloud(canvas->SX/4,0,0,DEFAULT_RADIUS));
				
				//submit the added cloud to the user_guess list of clouds
				canvas->addMetaball(cur_cloud_a->model);
				
				
				STOP_GROWTH_A = false;
				STATE_GROW_A = true;
				CUR_KEY = 'a';
			}
			
			break;
		}
		case 'S': // cannon to the far right
		case 's':
		{
			if(CUR_KEY != 's'){
				
				clouds.push_back( cur_cloud_b = new m_cloud(canvas->SX/2,0,0,DEFAULT_RADIUS));
				canvas->addMetaball(cur_cloud_b->model);
				
				STOP_GROWTH_B = false;
				STATE_GROW_B = true;
				CUR_KEY = 's';
			}
			
			break;
		}
			
		case 'D': // cannon to the far right
		case 'd':
		{
			if(CUR_KEY != 'd'){
				
				clouds.push_back( cur_cloud_c = new m_cloud(canvas->SX - canvas->SX/4 ,0,0,DEFAULT_RADIUS));
				canvas->addMetaball(cur_cloud_c->model);
				
				STOP_GROWTH_C = false;
				STATE_GROW_C = true;
				CUR_KEY = 'd';
			}
			
			break;
		}
		
		case 'r': // reset
		case 'R':
			clearClouds();
			break;
			
		default:
			break;
	}
}

void MainScene::clearClouds(){
	user_guess.clear(); // clear guesses
	
	//delete all the clouds
	list<m_cloud*>::iterator it;
	for( it = clouds.begin(); it != clouds.end(); it++){
		canvas->removeMetaball( (*it)->model );
		delete (*it);
	}
	
	clouds.clear(); // clear clouds list
}

void MainScene::onKeyRelease(QKeyEvent* event){

	switch (event->key()) { // key was released
			
		case 'a': // cannot to the far left
		case 'A':
			
			user_guess.push_back(cur_cloud_a->model);
			cur_cloud_a = NULL;
			STATE_GROW_A = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 's': // middle cannon
		case 'S':

			user_guess.push_back(cur_cloud_b->model);
			cur_cloud_b = NULL;
			STATE_GROW_B = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 'd': // cannon to the far right
		case 'D':{
			user_guess.push_back(cur_cloud_c->model);
			cur_cloud_c = NULL;
			STATE_GROW_C = false; // set growing to false
			CUR_KEY = ' ';
			break;
		}
			
		default:
			break;
	}
}
