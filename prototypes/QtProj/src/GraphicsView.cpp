/*
 *  GraphicsView.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */



#include "GraphicsView.h"


GraphicsView::GraphicsView()
{
	setWindowTitle(tr("Cloud Factory"));
	
	this->setViewport(new QGLWidget());
	this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	mainScene = new MainScene();
	this->setScene(mainScene);
	mainScene->addText("Hello World!");
	
	homeScreen = new HomeScreen();
	mainScene->addItem( homeScreen );
	connect( homeScreen , SIGNAL(onPlay()) , this , SLOT(hideHomeScreen()) );
	

}

void GraphicsView::keyPressEvent(QKeyEvent *event){
	mainScene->onKeyPress(event);
} // end create scene

void GraphicsView::keyReleaseEvent(QKeyEvent *event){
	mainScene->onKeyRelease(event);
} // end key release


void GraphicsView::resizeEvent(QResizeEvent *event) {
	if (scene()) scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
	QGraphicsView::resizeEvent(event);
	mainScene->hud->move(width()-mainScene->hud->width(),0);
}

// SLOTS

void GraphicsView::hideHomeScreen(){
	cout << " hide the home scree " << endl;
	mainScene->removeItem(homeScreen);
}