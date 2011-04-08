#ifndef BLUE_PRINT_DETECT_H
#define BLUE_PRINT_DETECT_H

#include <vector>
#include <math.h>
#include <iostream>
#include <assert.h>
#include "float.h"
#include "CloudModel.h"

using namespace std;

class BluePrintDetect {
private:
	typedef struct{
		float x;
		float y;
		float z;
	} Point;
	static void PointInit(float x, float y, float z , Point& pointR );
	static void PointAdd(float x, float y, float z, Point& pointR );
	static void PointSubtract( const Point& pointA , const Point& pointB , Point& pointR );
	static void PointDivide(float denominator, Point& pointR );
public:
	BluePrintDetect();
	
	static float SizeTolerance;
	static float PosTolerance;
	
	static float CalculateError( vector<CloudModel*> blueprint, vector<CloudModel*> clouds );
	static void ComputeCenterOfMass( vector<CloudModel*> clouds , Point& weightedPosR );
	static float CloudsDeviation(CloudModel* cloud,CloudModel* closest, const Point& offset );
	static CloudModel* GetClosestNeighbor(CloudModel* cloud, vector<CloudModel*> clouds , const Point& offset );
	static float GetDistance(CloudModel* cloud1, CloudModel* cloud2 , const Point& offset );
};

#endif /*BLUE_PRINT_DETECT_H*/