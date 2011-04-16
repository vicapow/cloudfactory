/*
 *  GraphicsView.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/15/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#ifndef GRAPHICS_VIEW
#define GRAPHICS_VIEW

#include <QtGui/QGraphicsView>
#include <QtGui/QResizeEvent>
#include <QtOpenGL>


#include "MainScene.h"


class GraphicsView : public QGraphicsView
{
public:
	GraphicsView();
    
	MainScene* mainScene;
	
protected:
	void resizeEvent(QResizeEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
};

#endif GRAPHICS_VIEW