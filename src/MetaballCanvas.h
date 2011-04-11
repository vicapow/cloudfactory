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
#include <cstdio>
using namespace std;

//--WM--//
#include "Wm5WindowApplication3.h"
using namespace Wm5;

//--local--//
#include "marchingcubes.hpp"


class MetaballCanvas : public Node {
	
public:
	
	MetaballCanvas();
	void draw();
	
	float *** metaballs;
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
	TriMesh* regenerate_metaballs_mesh(list<vertex>& vertices);
	TriMesh* getTriMeshFromVertexList( list<vertex>& vertices );
	//void updateTriMesh( VertexFormat* format, VertexBuffer* vbuffer, IndexBuffer* ibuffer, list<vertex>& vertices, int max_indices );
	TriMesh* myMesh;
};

#endif /*METABALL_CANVAS*/