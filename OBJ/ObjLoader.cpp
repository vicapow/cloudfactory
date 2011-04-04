// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.1.0 (2010/03/30)

// Modified by Jay Soyer on 2/10/11.
// Copyright 2011 All rights reserved.
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
//
// Version 1.1


#include "ObjLoader.h"
#include <cassert>
#include <fstream>

// Disable Microsoft warning about unsafe functions (security).
#pragma warning(disable:4996)

//----------------------------------------------------------------------------
ObjLoader::ObjLoader (const string& path, const string& filename)
    :
    mCode(EC_SUCCESSFUL),
    mCurrentGroup(-1),
    mCurrentPos(-1),
    mCurrentTcd(-1),
    mCurrentNor(-1),
    mCurrentMtl(-1),
    mCurrentMesh(-1),
	mMatRead(false),
	mDefGroup(false),
	mRegGroup(false)
{
	
	
    mLogFile = fopen("ObjLogFile.txt", "wt");
    if (!mLogFile)
    {
        assert(false);
        mCode = EC_LOGFILE_OPEN_FAILED;
        return;
    }

    string filePath = path + filename;
    ifstream inFile(filePath.c_str());
    if (!inFile)
    {
        mCode = EC_FILE_OPEN_FAILED;
        fprintf(mLogFile, "%s: %s\n", msCodeString[mCode], filePath.c_str());
        fclose(mLogFile);
        assert(false);
        return;
    }

    string line;
    vector<string> tokens;

    while (!inFile.eof())
    {
        getline(inFile, line);

        // Skip blank lines.
        if (line == "") { continue; }

        // Skip comments.
        if (line[0] == '#') { continue; }

        GetTokens(line, tokens);
        if (tokens.size() == 0)
        {
            mCode = EC_NO_TOKENS;
            break;
        }

        // mtllib
        if (GetMaterialLibrary(path, tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // g default
        if (GetDefaultGroup(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // v x y z
        if (GetPosition(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // vt x y
        if (GetTCoord(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // vn x y z
        if (GetNormal(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // g groupname
        if (GetGroup(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // usemtl mtlname
        if (GetMaterialAndMesh(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        // f vertexList
        if (GetFace(tokens)) { continue; }
        if (mCode != EC_SUCCESSFUL) { break; }

        //Ignore unknown symbols
		fprintf(mLogFile, "%s: %s\n", "Unrecognized Token: ", tokens[0].c_str());
    }

    if (mCode != EC_SUCCESSFUL)
    {
        fprintf(mLogFile, "%s: %s\n", msCodeString[mCode], line.c_str());
		assert(false);
    }
    else
    {
        fprintf(mLogFile, "%s\n", msCodeString[EC_SUCCESSFUL]);
    }
    fclose(mLogFile);
    inFile.close();
}
//----------------------------------------------------------------------------
ObjLoader::~ObjLoader ()
{
}
//----------------------------------------------------------------------------
void ObjLoader::GetTokens (const string & line, vector<string>& tokens)
{
    tokens.clear();

    string::size_type begin, end = 0;
    string token;

    while ((begin = line.find_first_not_of(" \t", end)) != string::npos)
    {
        end = line.find_first_of(" \t", begin);
        token = line.substr(begin, end-begin);
        tokens.push_back(token);
    }
}
//----------------------------------------------------------------------------
unsigned char ObjLoader::GetVertexTokens (const string &line, vector<string>& tokens)
{
	unsigned char bitflag = 0;	
	// 111 = v vt vn
	// 110 = v vt
	// 101 = v    vn
	// 100 = v
	// 000 = ERROR
	
    tokens.clear();
    string::size_type begin, end = 0;
    string token;
	
	//Gets first vertex index
	begin = line.find_first_of("0123456789", end);
	if (begin == string::npos) { return bitflag; }
	end = line.find_first_not_of("0123456789", begin);
	tokens.push_back(line.substr(begin, end-begin));
	bitflag = 100;
	
    begin = line.find_first_of("0123456789", end);
	
	if (begin - end == 1)	//Vertex texture found
	{
		end = line.find_first_not_of("0123456789", begin);
		tokens.push_back(line.substr(begin, end-begin));
		bitflag += 10;
		
		begin = line.find_first_of("0123456789", end);
		if (begin == string::npos) { return bitflag; }		//No vertex normal index
	}
	else if (begin == string::npos) { return bitflag; } //No vertex texture or normal index
	
	//Handle vertex normal index
	end = line.find_first_not_of("0123456789", begin);
	tokens.push_back(line.substr(begin, end-begin));
	bitflag += 1;
	
	return bitflag;
}

//----------------------------------------------------------------------------
bool ObjLoader::GetMaterialLibrary (const string& path,
    const vector<string>& tokens)
{
    if (tokens[0] == "mtllib")
    {
        if (tokens.size() == 1)
        {
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 2)
        {
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        MtlLoader loader(path, tokens[1]);
        if (loader.GetCode() != MtlLoader::EC_SUCCESSFUL)
        {
            mCode = EC_SUCCESSFUL;
			mMatRead = false;
        }
		else
		{
			mMatRead = true;
			mMaterials = loader.GetMaterials();
		}
		return true;
		
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetDefaultGroup (const vector<string>& tokens)
{
    if (tokens[0] == "g" && tokens[1] == "default")
    {	
		InitDefaultGroup();
            
		return true;
	}
	
	return false;
}
//----------------------------------------------------------------------------
void ObjLoader::InitDefaultGroup ()
{
	mDefGroup = true;
	mCurrentPos = (int)mPositions.size();
	mCurrentTcd = (int)mTCoords.size();
	mCurrentNor = (int)mNormals.size();	
}

//----------------------------------------------------------------------------
bool ObjLoader::GetPosition (const vector<string>& tokens)
{
    if (tokens[0] == "v")
    {
		if (!mDefGroup)
		{
			InitDefaultGroup();
		}
		
        if (tokens.size() < 4)
        {
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 4)
        {
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        Float3 pos;
        pos.x = (float)atof(tokens[1].c_str());
        pos.y = (float)atof(tokens[2].c_str());
        pos.z = (float)atof(tokens[3].c_str());
        mPositions.push_back(pos);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetTCoord (const vector<string>& tokens)
{
    if (tokens[0] == "vt")
    {
        if (tokens.size() < 3)
        {
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 3)
        {
            // TODO.  Need to handle 3D texture coordinates.
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        Float2 tcd;
        tcd.x = (float)atof(tokens[1].c_str());
        tcd.y = (float)atof(tokens[2].c_str());
        mTCoords.push_back(tcd);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetNormal (const vector<string>& tokens)
{
    if (tokens[0] == "vn")
    {
        if (tokens.size() < 4)
        {
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 4)
        {
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }

        Float3 nor;
        nor.x = (float)atof(tokens[1].c_str());
        nor.y = (float)atof(tokens[2].c_str());
        nor.z = (float)atof(tokens[3].c_str());
        mNormals.push_back(nor);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetGroup (const vector<string>& tokens)
{
    if (tokens[0] == "g")
    {
        mCurrentGroup = (int)mGroups.size();
        mGroups.push_back(Group());
        Group& group = mGroups[mCurrentGroup];
		
		if (tokens.size() == 1)
		{
			group.Name = "Forgot_a_Group_Name";
		} 
		else
		{	
			group.Name = tokens[1];
			for (int i = 2; i < (int)tokens.size(); ++i)
			{
				group.Name += tokens[i];
			}
		}
        
        group.PosStart = mCurrentPos;
        group.TcdStart = mCurrentTcd;
        group.NorStart = mCurrentNor;
        mCurrentPos = (int)mPositions.size();
        mCurrentTcd = (int)mTCoords.size();
        mCurrentNor = (int)mNormals.size();
        mRegGroup = true;
		
        return true;
    }
	
    return false;
}
//----------------------------------------------------------------------------
void ObjLoader::InitGroup ()
{
	vector<string> gTokens;
	gTokens.push_back("g");
	gTokens.push_back("No_Group_Found");
	
	GetGroup(gTokens);
}
//----------------------------------------------------------------------------
bool ObjLoader::GetMaterialAndMesh (const vector<string>& tokens)
{
	static unsigned int count = 0;
	
    if (tokens[0] == "usemtl")
    {
		if (!mRegGroup) { InitGroup(); }
        if (tokens.size() == 1)
        {
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }
        if (tokens.size() > 2)
        {
            mCode = EC_TOO_MANY_TOKENS;
            return false;
        }
		
		int i;
		
		if (mMatRead)
		{
			for (i = 0; i < (int)mMaterials.size(); ++i)
			{
				if (tokens[1] == mMaterials[i].Name)
				{
					break;
				}
			}
			if (i == (int)mMaterials.size())
			{
				mCode = EC_FAILED_TO_FIND_MATERIAL;
				return false;
			}
			mCurrentMtl = i;	
		}
        else
		{
			mCurrentMtl = count++;
			tokens[1] == "default_material";
		}

        Group& group = mGroups[mCurrentGroup];
        for (i = 0; i < (int)group.Meshes.size(); ++i)
        {
            if (group.Meshes[i].MtlIndex == mCurrentMtl)
            {
                break;
            }
        }
        if (i == (int)group.Meshes.size())
        {
            // Mesh with this material does not yet exist.
            group.Meshes.push_back(Mesh());
            group.Meshes.back().MtlIndex = mCurrentMtl;
        }

        mCurrentMesh = i;
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
bool ObjLoader::GetFace (const vector<string>& tokens)
{
    if (tokens[0] == "f")
    {
		if (!mRegGroup) { InitGroup(); }
        if (tokens.size() < 4)
        {
            // A face must have at least three vertices.
            mCode = EC_TOO_FEW_TOKENS;
            return false;
        }

        Group& group = mGroups[mCurrentGroup];
        Mesh& mesh = group.Meshes[mCurrentMesh];
        mesh.Faces.push_back(Face());
        Face& face = mesh.Faces.back();

        /* A vertex is one of the following:
        v/vt/vn/
		 v/vt/vn
        v/vt/
		 v/vt
		 v//vn
        v//
		 v/
		 v
		 where / is any none space, none numeric character 
		*/
		
        const int numVertices = (int)tokens.size() - 1;
        face.Vertices.resize(numVertices);
        for (int i = 0; i < numVertices; ++i)
        {            
			vector<string> nTokens;			
			switch (GetVertexTokens(tokens[i+1], nTokens)) 
			{
				case 100:	//v
					face.Vertices[i].PosIndex = atoi(nTokens[0].c_str()) - 1;
					break;
					
				case 110:	//v vt
					face.Vertices[i].PosIndex = atoi(nTokens[0].c_str()) - 1;
					face.Vertices[i].TcdIndex = atoi(nTokens[1].c_str()) - 1;
					break;
					
				case 111:	//v vt vn
					face.Vertices[i].PosIndex = atoi(nTokens[0].c_str()) - 1;
					face.Vertices[i].TcdIndex = atoi(nTokens[1].c_str()) - 1;
					face.Vertices[i].NorIndex = atoi(nTokens[2].c_str()) - 1;
					break;
					
				case 101:	//v    vn
					face.Vertices[i].PosIndex = atoi(nTokens[0].c_str()) - 1;
					face.Vertices[i].NorIndex = atoi(nTokens[1].c_str()) - 1;
					break;
					
				default:	//Error
					mCode = EC_INVALID_VERTEX;
					return false;
			}
        }
		
        return true;
    }
	
    return false;
}
//----------------------------------------------------------------------------
