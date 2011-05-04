/*
 *  HomeScreen.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "HomeScreen.h"

HomeScreen::HomeScreen() : QGraphicsPixmapItem(QPixmap(":/resources/cloudfactory-homescreen.jpg")) {
	cout << "HomeScreen Constructor " << endl;
}

HomeScreen::HomeScreen(const char* image) : QGraphicsPixmapItem( QPixmap(image) ) {
	cout << "Victory Screen " << endl;
}

void HomeScreen::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ){
	//QGraphicsItem::mouseReleaseEvent(event);
	cout << "Home Screen Released" << endl;
	emit onPlay();
}

void HomeScreen::mousePressEvent ( QGraphicsSceneMouseEvent * event ){
	
	cout << " Home screen pressed event " << endl;
}