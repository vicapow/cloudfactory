/*
 *  MetaballCanvas.cpp
 *  CloudFactory
 *
 *  Created by Victor Powell on 4/10/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "MetaballCanvas.hpp"


MetaballCanvas::MetaballCanvas(){
	
	//init
	SX = 200;
	SY = 250;
	SZ = 50;

	ball1 = 0;
	ball2 = 0;
	t = 0;
	voxels = newFloatMatrix(SX,SY,SZ);
	
	setResolution(9,9,9);//default resolution
}

void MetaballCanvas::init(){
	draw();
}


void MetaballCanvas::addMetaball(CloudModel* model){
	metaballs.push_back(model);
}

void MetaballCanvas::removeMetaball(CloudModel* model){
	for(unsigned int i = 0; i < metaballs.size();i++){
		if(metaballs[i]==model){
			metaballs.erase( metaballs.begin() + i );
			return;
		}
	}
}

void MetaballCanvas::removeAllMetaballs(){
	metaballs.erase( metaballs.begin() , metaballs.end() );
}

void MetaballCanvas::draw(){
	
	//cout << "MetaballCanvas::draw" << endl;
	
	initVoxels(voxels,SX,SY,SZ);//clear the voxel field
	
	for(int i = metaballs.size()-1; i>=0;i--){
		CloudModel* model = metaballs[i];
		drawMetaball(voxels,SX,SY,SZ,model->posX,model->posY,SZ-5,model->radius);
		//std::cout << "draw metaball : x: " << model->posX << " y: " << model->posY << " z: " << model->posZ << " r: " << model->radius << endl;
	}
	
	
	//--init metaball data
//	t++;
//	ball1 = sin(t/10.0) * 50 + 100 ;
//	ball2 = SX - ball1;
//	cout << "metaball t: " << t << endl;
//	
////	//this is just an example of drawing metaballs
//	drawMetaball(voxels,SX,SY,SZ,SX-SX/4+10,ball1,SZ-5,10);
//	drawMetaball(voxels,SX,SY,SZ,SX/2,ball2,SZ-5,10);
//	drawMetaball(voxels,SX,SY,SZ,SX/4-10,ball1,SZ-5,10);
	
	clearVertexList();
	vertices = runMarchingCubes(voxels,SX,SY,SZ,res_x,res_y,res_z,0.012);
	
	//cout << "update" << endl;
	draw_gl();
}


void MetaballCanvas::draw_gl(){
	
	list<vertex>::iterator it;
    glBegin(GL_TRIANGLES);
	for(it = vertices.begin(); it != vertices.end(); it++) {
		glNormal3d(it->normal_x, it->normal_y, it->normal_z);
		//cout << "draw vert at: x: " << it->x << " y: " << it->y << " z: " << it->z << endl ;
		glVertex3d(it->x, it->y, it->z);
	}
    glEnd();
}

