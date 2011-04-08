#include "BluePrintDetect.h"

float BluePrintDetect::SizeTolerance = 50;
float BluePrintDetect::PosTolerance = 50;

BluePrintDetect::BluePrintDetect(){
	//constructor
}


float BluePrintDetect::CalculateError( list<CloudModel*> blueprints, list<CloudModel*> clouds ){
	
	Point comBlueprints;
	BluePrintDetect::ComputeCenterOfMass(blueprints,comBlueprints);
	
	Point comClouds;
	BluePrintDetect::ComputeCenterOfMass(clouds,comClouds);
	
	Point diff;
	PointSubtract(comBlueprints,comClouds,diff);
	
	float score = 0;
	
	if(clouds.size()>=blueprints.size()){
		list<CloudModel*>::iterator it;
		for(it=blueprints.begin();it!=blueprints.end();it++){
			CloudModel* cloud = *it;
			CloudModel* closest = BluePrintDetect::GetClosestNeighbor(cloud,clouds,diff);
			clouds.remove(closest);
			score += BluePrintDetect::CloudsDeviation(cloud,closest,diff);
		}
		return score / blueprints.size();//?
	}else{
		//there are more blueprints than clouds, so it's impossible to be correct at all
		return 0;
	}
}

void BluePrintDetect::ComputeCenterOfMass( list<CloudModel*> clouds , Point& weightedPosR ){
	float totalMass = 0; //its not really "mass" per se, but it represents the weighting factor used in computing the weighted center (aka, the center-of-mass)
	PointInit(0,0,0,weightedPosR);
	list<CloudModel*>::iterator it;//itorator
	for(it=clouds.begin();it!=clouds.end();it++){
		CloudModel* cloud = *it;
		PointAdd(	cloud->posX * cloud->radius,
					cloud->posY * cloud->radius,
					cloud->posZ * cloud->radius,
					weightedPosR );
		assert(cloud->radius!=0);
		totalMass += cloud->radius;
	}
	PointDivide(totalMass,weightedPosR);
}

float BluePrintDetect::CloudsDeviation(CloudModel* cloud,CloudModel* closest , const Point& offset ){
	
	assert(cloud!=NULL);
	assert(closest!=NULL);
	
	//one is good, zero is bad
	float sizeDiff = fabs( cloud->radius - closest->radius );
	sizeDiff = sizeDiff / SizeTolerance;
	if(sizeDiff>1) sizeDiff = 1;
	sizeDiff = 1 - sizeDiff;
	
	float posDiff = GetDistance(cloud,closest,offset);
	posDiff = posDiff / PosTolerance;
	if(posDiff>1) posDiff = 1;
	posDiff = 1 - posDiff;
	
	return (sizeDiff + posDiff)/2;//noramlize
}

CloudModel* BluePrintDetect::GetClosestNeighbor(CloudModel* cloud, list<CloudModel*> clouds, const Point& offset ){
	
	CloudModel* closest = NULL;
	float min_d = FLT_MAX;
	list<CloudModel*>::iterator it;
	for(it=clouds.begin();it!=clouds.end();it++){
		float d = GetDistance(*it,cloud,offset);
		if(d < min_d ){
			closest = *it;
			min_d = d;
		}
	}
	return closest;
}

float BluePrintDetect::GetDistance(CloudModel* cloud1, CloudModel* cloud2 , const Point& offset){
	float x = cloud1->posX - (cloud2->posX + offset.x);
	float y = cloud1->posY - (cloud2->posY + offset.y);
	float z = cloud1->posZ - (cloud2->posZ + offset.z);
	return sqrt( x*x + y*y + z*z );
}

void BluePrintDetect::PointInit(float x, float y, float z , Point& pointR ){
	pointR.x = x;
	pointR.y = y;
	pointR.z = z;
}
void BluePrintDetect::PointAdd(float x, float y, float z, Point& pointR ){
	pointR.x += x;
	pointR.y += y;
	pointR.z += z;
}
/**
  * take pointA and subtract it from pointB returning the result in pointR
  */
void BluePrintDetect::PointSubtract( const Point& pointA , const Point& pointB , Point& pointR ){
	pointR.x = pointA.x - pointB.x;
	pointR.y = pointA.y - pointB.y;
	pointR.z = pointA.z - pointB.z;
}
void BluePrintDetect::PointDivide(float denominator, Point& pointR ){
	pointR.x /= denominator;
	pointR.y /= denominator;
	pointR.z /= denominator;
}