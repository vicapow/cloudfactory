/*
 *  Clouds.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 2/27/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "Cloud.h"

Cloud::Cloud(Transform trans, float r) {
	
	CloudModel::radius = r;
	
	CloudModel::posX = 0.0f;
	CloudModel::posY = 0.0f;
	CloudModel::posZ = 0.0f;

	this->AttachChild(CreateCloud(radius));
		
	tC = new0 TransformController(trans);
	this->AttachController(tC);
		
} // end constructor

Cloud::~Cloud(){}


void Cloud::UpdatePosition(APoint scale, APoint trans, HMatrix rot){
	
	Transform loc, cur;
	APoint curT, curS;
	
	cur = tC->GetTransform(); // get old transform
	 
	curT = cur.GetTranslate(); // get translate
	curS = cur.GetScale(); // get scale
	
	loc.SetScale(curS + scale);
	loc.SetTranslate(curT + trans);
	
	posX = curT[0] + trans[0]; // current position
	posY = curT[1] + trans[1]; // current position 
	posZ = curT[2] + trans[2]; // current position

	radius += scale[0]; // update the radius of the sphere
		
	tC->SetTransform(loc);
	
}
//----------------------------------------------------------------------------
TriMesh* Cloud::CreateCloud(float radius){
	
	VertexFormat* vformat;
    
	vformat = VertexFormat::Create(2,
								   VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
								   VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
	
    TriMesh* mesh = StandardMesh(vformat).Sphere(32, 32, radius);
    VertexBufferAccessor vba(mesh);
    for (int i = 0; i < vba.GetNumVertices(); ++i)
    {
        Float2& tcoord0 = vba.TCoord<Float2>(0, i);
        tcoord0[0] *= 4.0f;
        tcoord0[1] *= 4.0f;
    }
	
    std::string baseName = Environment::GetPathR("BTCloud.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();
	
	mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
																  baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
																  Shader::SC_REPEAT));
    
	
    return mesh;
}

