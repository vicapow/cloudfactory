/*
 *  MainScene.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/15/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */



#ifndef MAIN_SCENE
#define MAIN_SCENE

#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QPaintEngine>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>

#include <GLUT/GLUT.h>
#include <iostream>


#include "Wm5WindowApplication3.h"


//--local--//
#include "MetaballCanvas.hpp"
#include "BluePrintDetect.h"
#include "BluePrintHUD.h"

using namespace std;
using namespace Wm5;

class MainScene : public QGraphicsScene {
	
public:
	
	MainScene();
	
	void drawBackground(QPainter*, const QRectF& );
	void onEnterFrame();
	void onKeyPress(QKeyEvent* event);
	void onKeyRelease(QKeyEvent* event);
	
	float angle;
	
	int frame_time;
	float step_size;
	
	
	/* data structures */
	MetaballCanvas* canvas;
	vector<CloudModel*> user_guess;
	vector<CloudModel*> blueprint;
	list<struct m_cloud*> clouds;
	
	m_cloud* cur_cloud_a;
	m_cloud* cur_cloud_b;
	m_cloud* cur_cloud_c;
	
	GLuint texture;
	GLuint can_ind;
	QTimer* timer;//the timer that fires draw events
	QElapsedTimer elapsedTimer;
	float correctness;
	BluePrintHUD* blueprint_hud;
	
protected:
	void draw_GL();
	void remove_metaball(CloudModel* cloud);
	void create_scene();
	void load_image(const char * filename);
	void display_image(int width, int height);
	
};



class m_cloud {
	
public:
	//constructor
	m_cloud( float posX, float posY, float posZ, float radius ){
		cout << "m clouds created!" << endl;
		
		//there should only be oe metaball for each cloud but for now, this value isn't being used.
		match = false;
		model = new CloudModel(posX,posY,posZ,radius);
		model->velY = 1;
	};
	
	void update(float frame_time){
		model->posY+= frame_time * (100 / (model->getRadius()+50) );
		//cout << "frame time: " << frame_time << endl;
	}
	
	CloudModel* model;
	bool match;
	
	//desctuctor
	~m_cloud(){
		//cout << "m clouds destroied" << endl;
		delete model;
	}
	
};


#endif