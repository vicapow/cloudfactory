/*
 *  Background.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 2/27/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "Background.h"

TriMesh* CreateSquare (std::string background)
{
    VertexFormat* vformat;

	vformat = VertexFormat::Create(2,
									VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
									VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    
    int vstride = vformat->GetStride();
	
    VertexBuffer* vbuffer = new0 VertexBuffer(4, vstride);
    VertexBufferAccessor vba(vformat, vbuffer);
    vba.Position<Vector3f>(0) = Vector3f(-1.0f, -1.0f, 0.0f);
    vba.Position<Vector3f>(1) = Vector3f(+1.0f, -1.0f, 0.0f);
    vba.Position<Vector3f>(2) = Vector3f(+1.0f, +1.0f, 0.0f);
    vba.Position<Vector3f>(3) = Vector3f(-1.0f, +1.0f, 0.0f);
	
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(1.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(0.0f, 1.0f);
    
	
    IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
    int* indices = (int*)ibuffer->GetData();
    indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
    indices[3] = 0;  indices[4] = 2;  indices[5] = 3;
	
    TriMesh* mesh = new0 TriMesh(vformat, vbuffer, ibuffer);
	
    std::string baseName = Environment::GetPathR(background);
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();
	
	mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
																baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
																Shader::SC_REPEAT));
    
	
    return mesh;
}

TriMesh *CreateBlueprint(std::string blueprint){
	VertexFormat* vformat;
	
	vformat = VertexFormat::Create(2,
								   VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
								   VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
    
    int vstride = vformat->GetStride();
	
    VertexBuffer* vbuffer = new0 VertexBuffer(4, vstride);
    VertexBufferAccessor vba(vformat, vbuffer);
    vba.Position<Vector3f>(0) = Vector3f(-1.0f, -1.0f, 0.0f);
    vba.Position<Vector3f>(1) = Vector3f(+1.0f, -1.0f, 0.0f);
    vba.Position<Vector3f>(2) = Vector3f(+1.0f, +1.0f, 0.0f);
    vba.Position<Vector3f>(3) = Vector3f(-1.0f, +1.0f, 0.0f);
	
	vba.TCoord<Float2>(0, 0) = Float2(0.0f, 0.0f);
	vba.TCoord<Float2>(0, 1) = Float2(1.0f, 0.0f);
	vba.TCoord<Float2>(0, 2) = Float2(1.0f, 1.0f);
	vba.TCoord<Float2>(0, 3) = Float2(0.0f, 1.0f);
    
	
    IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
    int* indices = (int*)ibuffer->GetData();
    indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
    indices[3] = 0;  indices[4] = 2;  indices[5] = 3;
	
    TriMesh* mesh = new0 TriMesh(vformat, vbuffer, ibuffer);
	
    std::string baseName = Environment::GetPathR(blueprint);
    Texture2D* baseTexture = Texture2D::LoadWMTF(baseName);
    baseTexture->GenerateMipmaps();
	
	mesh->SetEffectInstance(Texture2DEffect::CreateUniqueInstance(
																  baseTexture, Shader::SF_LINEAR_LINEAR, Shader::SC_REPEAT,
																  Shader::SC_REPEAT));
    
    mesh->LocalTransform.SetScale(APoint(0.1f, 0.1f, 0.1f));
	mesh->LocalTransform.SetTranslate(APoint(1.6f, 0.73f, 0.0f));
	return mesh;

}