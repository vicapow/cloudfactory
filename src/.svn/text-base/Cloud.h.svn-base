/*
 *  Clouds.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 2/27/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 */

#include "Wm5WindowApplication3.h"
#include "CloudModel.h"

using namespace Wm5;

class Cloud: public Node, public CloudModel {

private:
	TransformController *tC;

public:
	
	Cloud(Transform trans, float radius); // constructor
	~Cloud(); // destructor
	TriMesh *CreateCloud(float radius); // create the sphere... metaball later

	void UpdatePosition(APoint scale, APoint trans, HMatrix rot); // move the cloud
	
	float getRadius() { return radius; }
	
	float getPos() { return posY; }
};