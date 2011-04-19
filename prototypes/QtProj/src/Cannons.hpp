/*
 *  Cannons.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 2/27/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include <algorithm>
#include "ObjLoader.h"
#include "Wm5WindowApplication3.h"
#include <GLUT/glut.h>

#ifndef CANNONS_HPP
#define CANNONS_HPP

using namespace Wm5;
using namespace std;

//----------------------------------------------------------------------------
void CreateCannon(APoint trans, APoint scale){
	
	ObjLoader *loader = new0 ObjLoader("", "../../resources/Smokestack.obj");
	
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
	
	glBegin(GL_TRIANGLES);
	
	for(unsigned int i = 0; i < positions.size(); i++)
	{
		glVertex3f(scale[0] * (positions[i].x + trans[0]),scale[1] * (positions[i].y + trans[1]), scale[2] * (positions[i].z + trans[2]) );
	}
	
	glEnd();
}

#endif