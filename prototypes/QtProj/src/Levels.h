/*
 *  Levels.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */


#ifndef LEVELS_H
#define LEVELS_H

#include <QDomDocument>
#include <QFile>

#include <iostream>

#include "CloudModel.hpp"

using namespace std;

class Level {
	
public:
	vector<CloudModel*> blueprint;
	int num_cannons;
};

class Levels {
	
public:
	Levels( QString filename);
	void print();
	
private:
	vector<Level> levels;
	
};

#endif