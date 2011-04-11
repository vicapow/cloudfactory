

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#include "marching_cube.h"


typedef struct {
   dPoint3D p[3];
} TRIANGLE;

typedef struct {
   Point3D p[8];
} GRIDCELL;


unsigned char * visited;


/* get the particular cube configuration that corisponds to the grid points. 
 this should work indipendently of the size of the potential field */
int Get_Index( Potential_Field * field , GRIDCELL *grid,double isolevel )
{
	int cubeindex=0;
   if (field->Get_Potential(grid->p[0]) < isolevel) cubeindex |= 1;         //1
   if (field->Get_Potential(grid->p[1]) < isolevel) cubeindex |= 2;        //10
   if (field->Get_Potential(grid->p[2]) < isolevel) cubeindex |= 4;       //100
   if (field->Get_Potential(grid->p[3]) < isolevel) cubeindex |= 8;      //1000
   if (field->Get_Potential(grid->p[4]) < isolevel) cubeindex |= 16;    //10000
   if (field->Get_Potential(grid->p[5]) < isolevel) cubeindex |= 32;   //100000
   if (field->Get_Potential(grid->p[6]) < isolevel) cubeindex |= 64;  //1000000
   if (field->Get_Potential(grid->p[7]) < isolevel) cubeindex |= 128;//10000000

   return cubeindex;

}
/* produce the marching cube triangles for the provided grid cell */
int Polygonise(Potential_Field * field , GRIDCELL *grid,double isolevel,TRIANGLE *triangles , int step)
{
   int cubeindex,i,ntriang;
   dPoint3D vertlist[12];
   
   cubeindex= Get_Index(field,grid,isolevel);

   /* Cube is entirely in/out of the surface */
   if (edgeTable[cubeindex] == 0)
   {      
      return(0);
   }
   
   /* Find the vertices where the surface intersects the cube */
   if (edgeTable[cubeindex] & 1)
      vertlist[0] =
         field->Interpolate(isolevel,grid->p[0],grid->p[1]);
   if (edgeTable[cubeindex] & 2)
      vertlist[1] =
         field->Interpolate(isolevel,grid->p[1],grid->p[2]);
   if (edgeTable[cubeindex] & 4)
      vertlist[2] =
         field->Interpolate(isolevel,grid->p[2],grid->p[3]);
   if (edgeTable[cubeindex] & 8)
      vertlist[3] =
         field->Interpolate(isolevel,grid->p[3],grid->p[0]);
   if (edgeTable[cubeindex] & 16)
      vertlist[4] =
         field->Interpolate(isolevel,grid->p[4],grid->p[5]);
   if (edgeTable[cubeindex] & 32)
      vertlist[5] =
         field->Interpolate(isolevel,grid->p[5],grid->p[6]);
   if (edgeTable[cubeindex] & 64)
      vertlist[6] =
         field->Interpolate(isolevel,grid->p[6],grid->p[7]);
   if (edgeTable[cubeindex] & 128)
      vertlist[7] =
         field->Interpolate(isolevel,grid->p[7],grid->p[4]);
   if (edgeTable[cubeindex] & 256)
      vertlist[8] =
         field->Interpolate(isolevel,grid->p[0],grid->p[4]);
   if (edgeTable[cubeindex] & 512)
      vertlist[9] =
         field->Interpolate(isolevel,grid->p[1],grid->p[5]);
   if (edgeTable[cubeindex] & 1024)
      vertlist[10] =
         field->Interpolate(isolevel,grid->p[2],grid->p[6]);
   if (edgeTable[cubeindex] & 2048)
      vertlist[11] =
         field->Interpolate(isolevel,grid->p[3],grid->p[7]);

   /* Create the triangle */
   ntriang = 0;
   for (i=0;triTable[cubeindex][i]!=-1;i+=3)
   {
      triangles[ntriang].p[0] = vertlist[triTable[cubeindex][i  ]];
      triangles[ntriang].p[1] = vertlist[triTable[cubeindex][i+1]];
      triangles[ntriang].p[2] = vertlist[triTable[cubeindex][i+2]];
      ntriang++;
   }

   return(ntriang);
}

dPoint3D Potential_Field::Interpolate(double isolevel, Point3D p1, Point3D p2)
{
   double mu;
   dPoint3D p;

   double valp1 = Get_Potential( p1 );
   double valp2 = Get_Potential( p2 );

   if (ABS(isolevel-valp1) < 0.00001)
   {
  // 	cout << "a result = p1 \n";
      return(dPoint3D(p1.x,p1.y,p1.z));
   }
   if (ABS(isolevel-valp2) < 0.00001)
   {
   //	cout << "b result = p2 \n";
      return(dPoint3D(p2.x,p2.y,p2.z));
   }
   if (ABS(valp1-valp2) < 0.00001)
   {
   //	cout << "c result = p1 \n";
      return(dPoint3D(p1.x,p1.y,p1.z));
    }

   mu = (isolevel - valp1) / (valp2 - valp1);
   p.x = p1.x + mu * (p2.x - p1.x);
   p.y = p1.y + mu * (p2.y - p1.y);
   p.z = p1.z + mu * (p2.z - p1.z);

   return(p);
}

void Generate(Potential_Field *field ,std::vector<Wm5::Float3> *pos,
										std::vector<Wm5::Float3> *norms ,std::vector<Wm5::Float2> *tex, 
										double isolevel , int step , int x, int y , int z, int& num_verts)
{
	
	GRIDCELL grid;
	TRIANGLE triangles[5];
	int n;

	if( x<0 || (x+step) > field->Get_Size() ||
		y<0 || (y+step) > field->Get_Size() ||
		z<0 || (z+step) > field->Get_Size() ) return;

	int si = field->Get_Size() / step;
	if( visited[(x/step)+(y/step)*si+(z/step)*si*si] )
		return;
   	else
		visited[(x/step)+(y/step)*si+(z/step)*si*si] = 1;

	grid.p[0] = Point3D( x      ,y ,z);
	grid.p[1] = Point3D( x+step ,y ,z);
	grid.p[2] = Point3D( x+step ,y ,z+step);
	grid.p[3] = Point3D( x      ,y ,z+step);
	grid.p[4] = Point3D( x      ,y+step ,z);
	grid.p[5] = Point3D( x+step ,y+step ,z);
	grid.p[6] = Point3D( x+step ,y+step ,z+step);
	grid.p[7] = Point3D( x      ,y+step ,z+step);

	int cubeindex= Get_Index(field,&grid,isolevel);

	if (edgeTable[cubeindex] == 0) { return; }

	n = Polygonise(field , &grid, isolevel,triangles,step);
	//where n is the number of triangles created during polygonisation 

	float norm[3];
	float l;

	for(int i = 0 ; i < n ; i++)
	{
		norm[0] = ( triangles[i].p[1].y - triangles[i].p[0].y ) * ( triangles[i].p[2].z - triangles[i].p[0].z )
		 	- ( triangles[i].p[1].z - triangles[i].p[0].z ) * ( triangles[i].p[2].y - triangles[i].p[0].y );
		norm[1] = ( triangles[i].p[1].z - triangles[i].p[0].z ) * ( triangles[i].p[2].x - triangles[i].p[0].x )
		 	- ( triangles[i].p[1].x - triangles[i].p[0].x ) * ( triangles[i].p[2].z - triangles[i].p[0].z );
		norm[2] = ( triangles[i].p[1].x - triangles[i].p[0].x ) * ( triangles[i].p[2].y - triangles[i].p[0].y )
		 	- ( triangles[i].p[1].y - triangles[i].p[0].y ) * ( triangles[i].p[2].x - triangles[i].p[0].x );
		l = sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
		norm[0] = norm[0] / l;
		norm[1] = norm[1] / l;
		norm[2] = norm[2] / l;
		
		pos->push_back(Wm5::Float3(triangles[i].p[0].x ,triangles[i].p[0].y , triangles[i].p[0].z));
		pos->push_back(Wm5::Float3(triangles[i].p[2].x , triangles[i].p[2].y, triangles[i].p[2].z));
		pos->push_back(Wm5::Float3(triangles[i].p[1].x ,triangles[i].p[1].y , triangles[i].p[1].z));
		

		norms->push_back(Wm5::Float3(-1.0, 0.0, -1.0));
		norms->push_back(Wm5::Float3(-1.0, 0.0, -1.0));
		norms->push_back(Wm5::Float3(-1.0, 0.0, -1.0));

//		norms->push_back(Wm5::Float3(norm[0], norm[1], norm[2]));
//		norms->push_back(Wm5::Float3(norm[0], norm[1], norm[2]));
//		norms->push_back(Wm5::Float3(norm[0], norm[1], norm[2]));
		
		num_verts += 3;
		
	}

	// neighbors
	Generate(field, pos, norms, tex, isolevel,step,x+step,y,z, num_verts);
	Generate(field, pos, norms, tex, isolevel,step,x-step,y,z, num_verts);
	Generate(field, pos, norms, tex, isolevel,step,x,y+step,z, num_verts);
	Generate(field, pos, norms, tex, isolevel,step,x,y-step,z, num_verts);
	Generate(field, pos, norms, tex, isolevel,step,x,y,z+step, num_verts);
	Generate(field, pos, norms, tex, isolevel,step,x,y,z-step, num_verts);
 	
	return;
}

void Marching_Cube_Begin( Potential_Field * field , int step)
{
	int si = field->Get_Size() / step;
	visited = new unsigned char[si*si*si];
	memset( visited , 0 , si*si*si);
}

void Marching_Cube_End( )
{
	delete visited;
}













/**
  * @param field the Potential_Field to query
  */



TriMesh *Draw_Iso_Surface_Around_Point( Potential_Field * field, double isolevel , int step , int x, int y, int z)
{
	Time_Counter counter;
	counter.restart();

	VertexFormat* vformat = VertexFormat::Create(2, VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0, VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0 );
	int vstride = vformat->GetStride();
	
	std::vector<Wm5::Float3> pos;
	std::vector<Wm5::Float3> norm;
	std::vector<Wm5::Float2> tex;
	
	int num_verts = 0;
	
	int si = field->Get_Size() / step;
	int ind = (y/step)*si+(z/step)*si*si;
	int cubeindex=0;
	GRIDCELL grid;
	
	bool way = true;//is it out of the grid?
	if( x < 0 )
	{
		x=0;  
		way = true;
	}

	if( x >= field->Get_Size() )
		{
		x= field->Get_Size()-step;
		way = false;
		}

	if( way)
	for(int i=x;i<field->Get_Size();i+=step)
	{
		if( i < 0 || i >= field->Get_Size() )
			break;  

		if( visited[ind+i/step] )
			return NULL;

		grid.p[0] = Point3D( i      ,y ,z);
		grid.p[1] = Point3D( i+step ,y ,z);
		grid.p[2] = Point3D( i+step ,y ,z+step);
		grid.p[3] = Point3D( i      ,y ,z+step);
		grid.p[4] = Point3D( i      ,y+step ,z);
		grid.p[5] = Point3D( i+step ,y+step ,z);
		grid.p[6] = Point3D( i+step ,y+step ,z+step);
		grid.p[7] = Point3D( i      ,y+step ,z+step);

		cubeindex= Get_Index(field,&grid,isolevel); //return the particular marching cube for this current grid location
   		
		if (edgeTable[cubeindex] != 0) //if the current cube isn't at the edge of the field
		{
			Generate(field , &pos, &norm, &tex, isolevel, step , i , y , z, num_verts);
			counter.stop();

			if(num_verts > 0){
				IndexBuffer *ibuffer = new0 IndexBuffer(num_verts, sizeof(int));
				VertexBuffer *vbuffer = new0 VertexBuffer(num_verts, vstride);
				VertexBufferAccessor vba(vformat, vbuffer);
				
				int *indx = (int *)malloc(sizeof(int)*pos.size());
				
				Wm5::Float3 *poss = (Wm5::Float3 *)malloc(sizeof(Wm5::Float3)*pos.size());
				Wm5::Float3 *norms = (Wm5::Float3 *)malloc(sizeof(Wm5::Float3)*norm.size());
				
				for(unsigned int i = 0; i<pos.size(); i++)
				{
					
					poss[i][0] = pos[i][0] * 0.0005;
					poss[i][1] = pos[i][1] * 0.0005;
					poss[i][2] = pos[i][2] * 0.0005;
					
					vba.Position<Wm5::Float3>(i) = poss[i];					
					indx[i] = i;
				}
				
				for(unsigned int i = 0; i < norm.size(); i++){
					
					norms[i][0] = norm[i][0] * 0.0005;
					norms[i][1] = norm[i][1] * 0.0005;
					norms[i][2] = norm[i][2] * 0.0005;
					
					vba.Normal<Wm5::Float3>(i) = norms[i];

				}
				
				memcpy(ibuffer->GetData(), &indx[0], pos.size()*sizeof(int));
				TriMesh* mesh = new0 TriMesh(vformat, vbuffer, ibuffer);
				
				Float4 black(0.0f, 0.0f, 0.0f, 1.0f);
				Float4 white(1.0f, 1.0f, 1.0f, 1.0f);
				Float4 red(1.0, 0.0, 0.0, 1.0);
				Float4 orange(1.0, 0.55, 0.0f, 1.0f); 
				
				Material* redMaterial = new0 Material();
				
				if(field->match)
					redMaterial->Emissive = red;
				else
					redMaterial->Emissive = orange;
				redMaterial->Ambient = Float4(0.25f, 0.25f, 0.25f, 1.0f);
				redMaterial->Diffuse = Float4(1.0f, 0.0f, 0.0f, 1.0f);
				redMaterial->Specular = white;
				
				// A light for the effects.
				Light* light = new0 Light(Light::LT_DIRECTIONAL);
				light->Ambient = red;
				light->Diffuse = white;
				light->Specular = white;
				light->SetDirection(AVector::UNIT_Z);
				
				VisualEffectInstancePtr mIntersectEffect;
				
				LightDirPerVerEffect* effect = new0 LightDirPerVerEffect();
				mIntersectEffect = effect->CreateInstance(light, redMaterial);
				
				mesh->SetEffectInstance(mIntersectEffect);
				
				delete(indx);
				
				return mesh;
			}
		}
	
	}
	
	return NULL;
}

