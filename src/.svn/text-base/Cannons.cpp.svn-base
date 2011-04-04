/*
 *  Cannons.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 2/27/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "Cannons.h"
#include <algorithm>
#include "ObjLoader.h"

using namespace std;

Cannons::Cannons(APoint trans, HMatrix rot, APoint scale) {

	this->AttachChild(CreateCannon(trans, rot, scale));

}

Cannons::~Cannons() {}

//----------------------------------------------------------------------------
TriMesh* Cannons::CreateCannon(APoint trans, HMatrix rot, APoint scale){
	
	ObjLoader *loader = new0 ObjLoader("../../resources/", "Smokestack.obj");
	
	VertexFormat* vformat = VertexFormat::Create(3,
												 VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
												 VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
												 VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    int vstride = vformat->GetStride();
	
	int numVertices = 0;
	
	std::vector<ObjLoader::Float3> positions;
	std::vector<ObjLoader::Float3> normals;
	std::vector<ObjLoader::Float2> texcoords;
	std::vector<int> indices;
	for(unsigned int i = 0; i < loader->GetGroups().size(); i++)
	{
		//int i = 0;
		for(unsigned int j = 0; j < loader->GetGroups().at(i).Meshes.size(); j++)
		{
			for(unsigned int k = 0; k < loader->GetGroups().at(i).Meshes.at(j).Faces.size(); k++)
			{
				if(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.size() > 3)
				{
					int n = loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.size();
					for(int l = 0; l < (n-2); l++)
					{
						//Insert Vertex 1
						ObjLoader::Float3 pos = loader->GetPositions().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(0).PosIndex);
						ObjLoader::Float3 norm = loader->GetNormals().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(0).NorIndex);
						ObjLoader::Float2 texc = loader->GetTCoords().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(0).TcdIndex);
						positions.push_back(pos);
						normals.push_back(norm);
						texcoords.push_back(texc);
						indices.push_back(numVertices);
						numVertices++;
						
						//Insert Vertex l+1
						pos = loader->GetPositions().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l+1).PosIndex);
						norm = loader->GetNormals().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l+1).NorIndex);
						texc = loader->GetTCoords().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l+1).TcdIndex);
						positions.push_back(pos);
						normals.push_back(norm);
						texcoords.push_back(texc);
						indices.push_back(numVertices);
						numVertices++;
						
						//Insert Vertex l+2
						pos = loader->GetPositions().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l+2).PosIndex);
						norm = loader->GetNormals().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l+2).NorIndex);
						texc = loader->GetTCoords().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l+2).TcdIndex);
						positions.push_back(pos);
						normals.push_back(norm);
						texcoords.push_back(texc);
						indices.push_back(numVertices);
						numVertices++;
					}
				}	
				else
				{
					for(int l = 0; l < 3; l++)
					{
						ObjLoader::Float3 pos = loader->GetPositions().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l).PosIndex);
						ObjLoader::Float3 norm = loader->GetNormals().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l).NorIndex);
						ObjLoader::Float2 texc = loader->GetTCoords().at(loader->GetGroups().at(i).Meshes.at(j).Faces.at(k).Vertices.at(l).TcdIndex);
						positions.push_back(pos);
						normals.push_back(norm);
						texcoords.push_back(texc);
						indices.push_back(numVertices);
						numVertices++;
					}
				}
			}
		}
	}
	Wm5::Float3 *poss = (Wm5::Float3 *)malloc(sizeof(Wm5::Float3)*positions.size());
	Wm5::Float3 *norms = (Wm5::Float3 *)malloc(sizeof(Wm5::Float3)*positions.size());
	Wm5::Float2 *tcords = (Wm5::Float2 *)malloc(sizeof(Wm5::Float2)*positions.size());
	
	for(unsigned int i = 0; i < positions.size(); i++)
	{
		poss[i] = Float3(positions[i].x, positions[i].y, positions[i].z);
		norms[i] = Float3(normals[i].x, normals[i].y, normals[i].z);
		tcords[i] = Float2(texcoords[i].x, texcoords[i].y);
	}
	
	IndexBuffer *ibuffer = new0 IndexBuffer((int)indices.size(), sizeof(int));
	VertexBuffer *vbuffer = new0 VertexBuffer((int)indices.size(), vstride);
	VertexBufferAccessor vba(vformat, vbuffer);
	
	int *ind = (int *)malloc(sizeof(int)*positions.size());
	for(unsigned int i = 0; i<positions.size(); i++)
	{
		vba.Position<Wm5::Float3>(i) = poss[i];
		vba.Normal<Wm5::Float3>(i) = norms[i];
		vba.TCoord<Wm5::Float2>(0, i) = tcords[i];
		
		ind[i] = i;
	}
	
	memcpy(ibuffer->GetData(), &ind[0], positions.size()*sizeof(int));
	TriMesh* mesh = new0 TriMesh(vformat, vbuffer, ibuffer);
	
	Environment::InsertDirectory("../../resources/");
	std::string baseName = Environment::GetPathR("My_Brick.wmtf");
	cout << "texture resource path: " << baseName << endl;
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
	mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(baseTexture, Shader::SF_LINEAR,Shader::SC_CLAMP_EDGE, Shader::SC_CLAMP_EDGE));
	
	mesh->LocalTransform.SetTranslate(trans);
	mesh->LocalTransform.SetScale(scale);
	
	m_trans.SetTranslate(trans);
	m_trans.SetScale(scale);
	m_trans.SetRotate(rot);
	
	mesh->SetName("Cannon");
	
	return mesh;
}