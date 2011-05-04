/*
 *  HomeScreen.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#ifndef HOME_SCREEN
#define HOME_SCREEN

#include <QGraphicsPixmapItem>

#include <iostream>

using namespace std;

class HomeScreen : public QObject, public QGraphicsPixmapItem {
	
	Q_OBJECT
	
public:
	
	HomeScreen();
	HomeScreen(const char* image);
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	
signals:
	void onPlay();
};


#endif 