/*
 *  MainScene.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/15/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "MainScene.h"

#include "Cannons.hpp"

/* globals used to determine state of game */

static bool STATE_GROW_A, STATE_GROW_B, STATE_GROW_C;
static bool STOP_GROWTH_A, STOP_GROWTH_B, STOP_GROWTH_C;
static char CUR_KEY;

static Wm5::Float3 location_c, location_b, location_a;
static Wm5::Float3 incrementer_c, incrementer_b, incrementer_a;

GLfloat posX, posY, posZ;


MainScene::MainScene(){
	
	//setup the time for updating the GL scene
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(update()) );
	timer->start(0);
	angle = 0;
	correctness = 0;
	elapsedTimer.start();
	
	glutInitDisplayMode (GLUT_DOUBLE);
	
	create_scene();
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
	
	draw_GL();
	
}


void MainScene::create_scene(){
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	// set camera positions
	
	load_image("Night.raw");
	
	can_ind = glGenLists(3);
	
	/* cannon 1 */
	glNewList(can_ind, GL_COMPILE);
	
	APoint trans = APoint(2.0, -4.0, 0.0);
	APoint scale = APoint(1.0, 1.0, 1.0);
	
	CreateCannon(trans, scale);
	
	glEndList();
	
	/* cannon 2 */
	
	glNewList(can_ind+1, GL_COMPILE);
	
	trans = APoint(0.0, -4.0, 0.0);
	scale = APoint(1.0, 1.0, 1.0);
	
	CreateCannon(trans, scale);
	
	glEndList();
	
	
	/* cannon 2 */
	
	glNewList(can_ind +2, GL_COMPILE);
	
	trans = APoint(-2.0, -4.0, 0.0);
	scale = APoint(1.0, 1.0, 1.0);
	
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity();
	glScalef( 100,100,100 );
	glTranslatef(100,0,0);
	CreateCannon(trans, scale);
	glPopMatrix();
	
	glEndList();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	
	canvas = new MetaballCanvas();
	canvas->init();
	
	
//	glShadeModel(GL_SMOOTH);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//	
//	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
//	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat light_position[] = { -500.0, 500.0, -100.0, 0.0 };
//	
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
}


void MainScene::draw_GL(){
	
	glViewport(0, 0, (GLsizei) width(), (GLsizei) height() );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat) width()/(GLfloat) height(), 1.0, 1000.0);	
	gluLookAt(canvas->SX/2, canvas->SY/2, canvas->SZ*10 , canvas->SX/2, canvas->SY/2, 0, 0, 1, 0);
	
	glClearColor(1.0, 0.0, 0.0, 1.0);

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	
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
	
	
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// specify the lists to be drawn
	GLubyte lists[3]; 
	lists[0] = 0; lists[1] = 1; lists[2] = 2;
	
	// draw lists
	glListBase(can_ind);
	glCallLists(3, GL_UNSIGNED_BYTE, lists);
	
	//display_image(640, 480);
	
	
	
	// Set material properties which will be assigned by glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    float specReflection[] = { 0.8, 0.8, 0.8, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	
	glColor3f(1-correctness+0.5,1,1-correctness+0.5 );
	
	//cout << "correctness: " << correctness << endl;
	
	// Draw the triangles
    canvas->draw();
	
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
}


void MainScene::drawBackground(QPainter *painter, const QRectF &)
{	
	onEnterFrame();
}


void MainScene::remove_metaball(CloudModel* model ){
	
	//remove the model from the guess vector
	for(int i = 0; i < user_guess.size();i++){
		if(user_guess[i] == model){
			user_guess.erase( user_guess.begin() + i );
			break;
		}
	}
	
	//remove the cloud from the metaball canvas
	canvas->removeMetaball(model);
}


void MainScene::load_image(const char * filename){
	
    int width, height;
    char * data;
    FILE * file;
	
    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return;
	
    // allocate buffer
    width = 256;
    height = 256;
    data = (char*)malloc( width * height * 3 );
	
    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );
	
    // allocate a texture name
    glGenTextures( 1, &texture );
	
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
	
    // free buffer
    free( data );
}

void MainScene::display_image(int width, int height)
{
	// bind and draw the foreground texture
	
	glEnable(GL_TEXTURE_2D);   
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f); glVertex2f(0     , 0 +  0);
	glTexCoord2f(1.0f,0.0f); glVertex2f(1, 0 +  0);
	glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
	glTexCoord2f(0.0f,1.0f); glVertex2f(0     , 1);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
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
				user_guess.push_back(cur_cloud_a->model);
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
				user_guess.push_back(cur_cloud_b->model);
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
				user_guess.push_back(cur_cloud_c->model);
				canvas->addMetaball(cur_cloud_c->model);
				
				STOP_GROWTH_C = false;
				STATE_GROW_C = true;
				CUR_KEY = 'd';
			}
			
			break;
		}
		case 'r': // reset
		case 'R':
		{
			
			user_guess.clear(); // clear guesses
			
			for(unsigned int i = 0; i < clouds.size(); i++){
			}
			
			//delete all the clouds
			list<m_cloud*>::iterator it;
			for( it = clouds.begin(); it != clouds.end(); it++){
				canvas->removeMetaball( (*it)->model );
				delete (*it);
			}
			
			clouds.clear(); // clear clouds list
			
		}break;
			
		default:
			break;
	}
}


void MainScene::onKeyRelease(QKeyEvent* event){

	switch (event->key()) { // key was released
			
		case 'a': // cannot to the far left
		case 'A':
			
			cur_cloud_a = NULL;
			STATE_GROW_A = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 's': // middle cannon
		case 'S':
			
			cur_cloud_b = NULL;
			STATE_GROW_B = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 'd': // cannon to the far right
		case 'D':{
			cur_cloud_c = NULL;
			STATE_GROW_C = false; // set growing to false
			CUR_KEY = ' ';
			break;
		}
			
		default:
			break;
	}
}
