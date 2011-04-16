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
using namespace std;

//--WM--//
#include "Wm5WindowApplication3.h"
using namespace Wm5;

//--local--//
#include "marchingcubes.hpp"
#include "CloudModel.h"

class CloudModel;

class MetaballCanvas : public Node {
	
public:
	
	MetaballCanvas();
	void draw();
	void init();
	void addMetaball(CloudModel* model);
	void removeMetaball(CloudModel* model);
	
	float *** voxels;
	list<vertex> vertices;
	int SX;
	int SY;
	int SZ;
	int MAX_INDICES;
	int num_indices;
	float ball1;
	float ball2;
	int t;
	
	

	
private:
	void updateMesh( list<vertex>& vertices );
	//void updateTriMesh( VertexFormat* format, VertexBuffer* vbuffer, IndexBuffer* ibuffer, list<vertex>& vertices, int max_indices );
	TriMesh* myMesh;
	VertexFormat* format;
	VertexBuffer* vbuffer;
	IndexBuffer* ibuffer;
	vector<CloudModel*> metaballs;
};

#endif /*METABALL_CANVAS*/