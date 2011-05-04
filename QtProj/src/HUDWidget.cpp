/*
 *  HUDWidget.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "HUDWidget.h"



HUDWidget::HUDWidget (){

	resize(200,150);
	this->setStyleSheet("border-style: outset; border-width: 2px; border-color: blue;");
	//this->setFrameStyle(QFrame::Panel | QFrame::Raised);
	//this->setFrameShadow(QFrame::Raised);
}