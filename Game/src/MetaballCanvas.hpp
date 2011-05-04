/*
 *  MetaballCanvas.h
 *  CloudFactory
 *
 *  Created by Victor Powell on 4/10/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */


#ifndef METABALL_CANVAS
#define METABALL_CANVAS

//--C-STD--//
#include <list>
#include <vector>
#include <cstdio>
#include <assert.h>
#include <iostream>
#include <GLUT/glut.h>


using namespace std;

//--WM--//

//--local--//
#include "marchingcubes.hpp"
#include "CloudModel.hpp"

class CloudModel;

class MetaballCanvas {
	
public:
	
	MetaballCanvas();
	void draw();
	void init();
	void addMetaball(CloudModel* model);
	void removeMetaball(CloudModel* model);
	void removeAllMetaballs();
	void draw_gl();
	void setResolution( int res_x, int res_y, int res_z ){
		this->res_x = res_x;
		this->res_y = res_y;
		this->res_z = res_z;
	}
	
	float *** voxels;
	list<vertex> vertices;
	int SX;
	int SY;
	int SZ;
	int res_x;
	int res_y;
	int res_z;
	int MAX_INDICES;
	int num_indices;
	float ball1;
	float ball2;
	int t;
	
private:
	
	vector<CloudModel*> metaballs;

};

#endif /*METABALL_CANVAS*/