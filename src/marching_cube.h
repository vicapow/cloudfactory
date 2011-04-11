#ifndef MARCHING_CUBE_H
#define MARCHING_CUBE_H

#include "Wm5WindowApplication3.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <list>

#include "marching_cube.h"
#include "Time_Counter.h"
#include "ObjLoader.h"
#include "marchingcubes.hpp"

using namespace Wm5;
using namespace std;

class Point3D
{
public:
   int x, y, z;
   Point3D(int x = 0, int y = 0, int z = 0)
   { this->x = x; this->y = y; this->z = z; return; }
};

class dPoint3D
{
public:
   double x, y, z;
   dPoint3D(double x = 0.0, double y = 0.0, double z = 0.0)
   { this->x = x; this->y = y; this->z = z; return; }
};


class Potential_Field
{
public:
	bool match;
	Potential_Field(int size) { Size=size;};
	virtual double Get_Potential( Point3D p) =0;
	dPoint3D Interpolate(double isolevel, Point3D p1, Point3D p2);
	int Get_Size() { return Size;};
protected :
	int Size;
};

TriMesh *Draw_Iso_Surface_Around_Point( Potential_Field * field  , double isolevel , int step , int x,int y,int z);
TriMesh* Box (VertexFormat* format, float xExtent, float yExtent, float zExtent);
void Marching_Cube_Begin( Potential_Field * field , int step);
void Marching_Cube_End( );

#endif
