/*
 *  Cannons.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 2/27/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "Wm5WindowApplication3.h"

using namespace Wm5;

class Cannons: public Node {

public:
	
	Cannons(APoint trans, HMatrix rot, APoint scale);
	~Cannons() ;
	Transform getTrans() {return m_trans;}

private:
	
	Transform m_trans;
	TriMesh *CreateCannon(APoint trans, HMatrix rot, APoint scale);

};