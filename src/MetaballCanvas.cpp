/*
 *  MetaballCanvas.cpp
 *  CloudFactory
 *
 *  Created by Victor Powell on 4/10/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "MetaballCanvas.h"

MetaballCanvas::MetaballCanvas(){
	
	//init
	SX = 200;
	SY = 200;
	SZ = 50;
	MAX_INDICES = 4096;
	ball1 = 0;
	ball2 = 0;
	t = 0;
	
	voxels = newFloatMatrix(SX,SY,SZ);
	
	format = VertexFormat::Create(3, VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0, VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0, VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);	
	
	vbuffer = new0 VertexBuffer(MAX_INDICES, format->GetStride());
	ibuffer = new0 IndexBuffer(MAX_INDICES, 4);
	
	cout << "MetaballCanvas mesh: " << myMesh << endl;

}



void MetaballCanvas::init(){
	
	myMesh = new0 TriMesh(format, vbuffer, ibuffer);
	
	//draw();
	
	myMesh->LocalTransform.SetScale(APoint(1.0 / SX, 1.0 / SX , 1.0 / SX ));
	myMesh->LocalTransform.SetRotate(HMatrix(AVector::UNIT_Y, Mathf::DEG_TO_RAD * 180 ));
	this->SetChild(0,myMesh);
	this->Update();
	
	string baseName = Environment::GetPathR("My_Brick.wmtf");
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
	myMesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(baseTexture, Shader::SF_LINEAR,Shader::SC_CLAMP_EDGE, Shader::SC_CLAMP_EDGE));
	
	//--hack i need to indicate the max length of the indices now or else it wont draw the additional indices
		for(int i = 0; i < MAX_INDICES; i++){
			vertex vert;
			vertices.push_back(vert);
		}
		updateMesh(vertices);
		myMesh->UpdateModelSpace(Visual::GU_USE_GEOMETRY);
		myMesh->Update();
	//--end-hack
	
}



void MetaballCanvas::addMetaball(CloudModel* model){
	metaballs.push_back(model);
}

void MetaballCanvas::removeMetaball(CloudModel* model){
	for(int i = 0; i < metaballs.size();i++){
		if(metaballs[i]==model){
			metaballs.erase( metaballs.begin() + i );
			return;
		}
	}
	cout << "WARNING: no metaball found to be removed. " << endl;
}

void MetaballCanvas::draw(){
	
	initVoxels(voxels,SX,SY,SZ);//clear the voxel field

	
	
    //if(metaballs.size()==0) return;	
	for(int i = metaballs.size()-1; i>=0;i--){
		CloudModel* model = metaballs[i];
		drawMetaball(voxels,SX,SY,SZ,model->posX,model->posY,SZ-5,model->radius);
		cout << "draw metaball : x: " << model->posX << " y: " << model->posY << " z: " << model->posZ << " r: " << model->radius << endl;
	}
	
	
//	//--init metaball data
	t++;
	ball1 = sin(t/10.0) * 50 + 100 ;
	ball2 = SX - ball1;
	
	drawMetaball(voxels,SX,SY,SZ,SX-SX/4-10,ball1,SZ-5,12);
	//drawMetaball(voxels,SX,SY,SZ,SX/2,ball2,SZ-5,12);
	//drawMetaball(voxels,SX,SY,SZ,SX/4+10,ball1,SZ-5,12);
	
	clearVertexList();
	vertices = runMarchingCubes(voxels,SX,SY,SZ,5,5,5,1);

	
	
	updateMesh(vertices);

	myMesh->UpdateModelSpace(Visual::GU_USE_GEOMETRY);
	myMesh->Update();
}








void MetaballCanvas::updateMesh(list<vertex>& vertices )
{

	
	int numVertices = vertices.size()<MAX_INDICES?vertices.size():MAX_INDICES;
    int numTriangles = numVertices/3;
    int numIndices = numVertices;
    int stride = format->GetStride();
	bool mHasNormals = false;
	bool mHasTCoords[VertexFormat::AM_MAX_TCOORD_UNITS];
	

	vbuffer->SetNumElements(numVertices);
	ibuffer->SetNumElements(numIndices);
	
	
	//textures depends on this. i pull this code out of Wm5StandardMesh.[cpp,h]
	for (int unit = 0; unit < VertexFormat::AU_TEXCOORD; ++unit)
    {
        mHasTCoords[unit] = false;
        int tcdIndex = format->GetIndex(VertexFormat::AU_TEXCOORD, unit);
        if (tcdIndex >= 0)
        {
            VertexFormat::AttributeType tcdType = format->GetAttributeType(tcdIndex);
			
            if (tcdType == VertexFormat::AT_FLOAT2)
            {
                mHasTCoords[unit] = true;
            }
        }
    }
	
	
	//normals depends on this. i pull this code out of Wm5StandardMesh.[cpp,h]
	int norIndex = format->GetIndex(VertexFormat::AU_NORMAL);
    if (norIndex >= 0)
    {
        VertexFormat::AttributeType norType = format->GetAttributeType(norIndex);
		
        if (norType == VertexFormat::AT_FLOAT3)
        {
            mHasNormals = true;
        }
    }
	
	
    // Create a vertex buffer.
    //VertexBuffer* vbuffer = new0 VertexBuffer(numVertices, stride);
    VertexBufferAccessor vba(format, vbuffer);
	
    // Generate geometry.
	list<vertex>::iterator it;
	int i = 0;
	for(it = vertices.begin(); it != vertices.end(); it++) {
		//it->normal_x, it->normal_y, it->normal_z
		vba.Position<Float3>(i) = Float3(it->x,it->y,it->z);
		vba.Normal<Float3>(i++) = Float3(it->normal_x,it->normal_y,it->normal_z);
	}
	
	// I'm not gonna worry about textures for now..
	
	for (int unit = 0; unit < VertexFormat::AU_TEXCOORD; ++unit)
    {
        if (mHasTCoords[unit])
        {
			//apply the texture to every triangle
			for(int i = 0; i < numIndices; i++){
				Float3 pos = vba.Position<Float3>(i);
				pos[0] /= SX;
				pos[1] /= SY;
				vba.TCoord<Float2>(unit, i) = Float2(pos[0],pos[1]);
			}
        }
    }
	
	
    // Generate indices (outside view).
    //IndexBuffer* ibuffer = new0 IndexBuffer(numIndices, 4);
    int* indices = (int*)ibuffer->GetData();
    for(int i = 0; i < numIndices; i++)
		indices[i] = i;
	
//    if (mHasNormals)
//    {
//        myMesh->UpdateModelSpace(Visual::GU_NORMALS);
//    }

}