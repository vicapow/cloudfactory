


#include <iostream>
#include <list>
#include "BluePrintDetect.h"

using namespace std;

int main(int argc , char* argv[] ){
	//create blueprint
	list<CloudModel*> blueprints;
	list<CloudModel*> clouds;
	
	//add the blueprints
	CloudModel* model = new CloudModel();
	model->radius = 10;
	model->posY = 0;
	blueprints.push_back(model);

	
	//add the clouds
	model = new CloudModel();
	model->radius = 11;
	model->posY = 100;
	clouds.push_back(model);

	
	
	float error = BluePrintDetect::CalculateError(blueprints,clouds);
	
	cout << " ERROR: " << error << endl;
	
}