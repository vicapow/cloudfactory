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
	
	QGLFormat fmt;
	QGLContext* context = new QGLContext(fmt);
	QGLWidget* glWidget = new QGLWidget(context);
	glWidget->makeCurrent();
	this->setViewport(glWidget);
	this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	mainScene = new MainScene(context);
	this->setScene(mainScene);
	connect( mainScene , SIGNAL(onLevelPassed()) , this , SLOT(advanceLevel()));
	
	homeScreen = new HomeScreen();
	mainScene->addItem( homeScreen );
	connect( homeScreen , SIGNAL(onPlay()) , this , SLOT(hideHomeScreen()) );
	
	storyScreen = new HomeScreen(":/resources/story.jpg");
	connect( storyScreen , SIGNAL(onPlay()) , this , SLOT(hideStoryScreen()) );
	
	hotToplay = new HomeScreen(":/resources/how-to-play.jpg");
	connect( howToplay , SIGNAL(onPlay()) , this , SLOT(hidePlay()) );

	levels = new Levels(":/resources/levels.xml");
	
	current_level = 0;
	advanceLevel();
}

void GraphicsView::loadLevel(const Level& level){
	mainScene->setBlueprint(level.blueprint);
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

void GraphicsView::hideStoryScreen(){
	
	cout << " hide the home scree " << endl;
	mainScene->removeItem(storyScreen);
	
	msCodeString->addItem(howToplay);
	
	
}

void GraphicsView::hidePlay(){
	
	cout << " hide the home scree " << endl;
	mainScene->removeItem(storyScreen);
	
	mainScene->addWidget(mainScene->hud);
	mainScene->hud->setContentsMargins(0, 0, 0, 0);
	mainScene->hud->setToolTip("Use this blueprint to create clouds");
	layout = new QVBoxLayout();
	layout->setContentsMargins(1,1,1,1);
	mainScene->hud->setLayout(layout);
	layout->addWidget(mainScene->blueprint_hud);
	
}


void GraphicsView::hideHomeScreen(){
	cout << " hide the home scree " << endl;
	mainScene->removeItem(homeScreen);
	
	mainScene->addItem(storyScreen);
	
}

void GraphicsView::advanceLevel(){
	if( current_level < levels->length() ){
		loadLevel(levels->getLevel(current_level++));
	}
	else {
		HomeScreen *victory = new HomeScreen(":/resources/happy.jpg");
		mainScene->addItem(victory);
		layout->removeWidget(mainScene->blueprint_hud);
		delete(mainScene->hud);
	}

}