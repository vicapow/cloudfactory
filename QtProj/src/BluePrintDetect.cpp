#include "BluePrintDetect.h"

float BluePrintDetect::SizeTolerance = 50;
float BluePrintDetect::PosTolerance = 50;

BluePrintDetect::BluePrintDetect(){
	//constructor
}

float BluePrintDetect::CalculateError( vector<CloudModel*> blueprints, vector<CloudModel*> clouds ){
	
	if(blueprints.size() > clouds.size()) return 0;//you can never have a matching pattern with too few clouds
	
	Point comBlueprints;
	BluePrintDetect::ComputeCenterOfMass(blueprints,comBlueprints);
	
	Point comClouds;
	BluePrintDetect::ComputeCenterOfMass(clouds,comClouds);
	
	if (comClouds.y < 75) {
		return 0.0f;
	}
	
	Point diff;
	PointSubtract(comBlueprints,comClouds,diff);
	
	float score = 0;
	
	for(unsigned int i = 0; i < blueprints.size();i++){
		CloudModel* cloud = blueprints[i];
		assert(clouds.size()>0);
		CloudModel* closest = BluePrintDetect::GetClosestNeighbor(cloud,clouds,diff);
		for(unsigned int j = 0; j < clouds.size();j++){
			if(clouds[j] == closest){
				clouds.erase(clouds.begin() + j);
				break;
			}
		}

		score += BluePrintDetect::CloudsDeviation(cloud,closest,diff);
	}
	if(blueprints.size()>0)
		return score / blueprints.size();
	else 
		return 0;
}

void BluePrintDetect::ComputeCenterOfMass( vector<CloudModel*> clouds , Point& weightedPosR ){
	
	float totalMass = 0; //its not really "mass" per se, but it represents the weighting factor used in computing the weighted center (aka, the center-of-mass)
	PointInit(0,0,0,weightedPosR);
	
	for(unsigned int i = 0; i < clouds.size();i++){
		
		if(clouds[i]->getRadius() > 0){
			
			CloudModel* cloud = clouds[i];
			PointAdd(	cloud->posX * cloud->getRadius(),
						cloud->posY * cloud->getRadius(),
						cloud->posZ * cloud->getRadius(),
						weightedPosR );
			
			assert(cloud->getRadius()!=0);	
			
			totalMass += cloud->getRadius();
		}
	}
	PointDivide(totalMass,weightedPosR);
}

float BluePrintDetect::CloudsDeviation(CloudModel* cloud,CloudModel* closest , const Point& offset ){
	
	if(cloud == NULL || closest == NULL)
		return 0.0f;
	
	assert(cloud!=NULL);
	assert(closest!=NULL);
	
	//one is good, zero is bad
	float sizeDiff = fabs( cloud->getRadius() - closest->getRadius() );
	sizeDiff = sizeDiff / SizeTolerance;
	if(sizeDiff>1) sizeDiff = 1;
	sizeDiff = 1 - sizeDiff;
	
	float posDiff = GetDistance(cloud,closest,offset);
	posDiff = posDiff / PosTolerance;
	if(posDiff>1) posDiff = 1;
	posDiff = 1 - posDiff;
	
	return (sizeDiff + posDiff)/2;//noramlize
}

CloudModel* BluePrintDetect::GetClosestNeighbor(CloudModel* cloud, vector<CloudModel*> clouds, const Point& offset ){
	assert(clouds.size()>0);
	CloudModel* closest = NULL;
	float min_d = FLT_MAX;
	for(int i = 0; i < clouds.size();i++){
		float d = GetDistance(clouds[i],cloud,offset);
		if(d < min_d ){
			closest = clouds[i];
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