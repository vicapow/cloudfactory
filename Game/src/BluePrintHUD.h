/*
 *  BluePrintHUD.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */


#ifndef BLUE_PRINT_HUD
#define BLUE_PRINT_HUD

#include <QtOpenGL>
#include <vector>

using namespace std;

#include "CloudModel.hpp"
#include "MetaballCanvas.hpp"

class BluePrintHUD : public QGLWidget {
	
public:
	
	BluePrintHUD( QWidget* parent = 0);
	void setBluePrint( vector<CloudModel*>& blueprint);
	
	vector<CloudModel*> blueprint;
	MetaballCanvas* canvas;
	
	
protected:
	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int w, int h);
	bool dirty;
};



#endif

