


#ifndef CLOUD_MODEL_H
#define CLOUD_MODEL_H


class CloudModel {

public:
	
	CloudModel(float x, float y, float z, float r ): posX(x) , posY(y), posZ(z), radius(r) { 
		assert(radius!=0);
	}
	CloudModel(): posX(0) , posY(0), posZ(0), radius(0) { }
	CloudModel(const float *pos){
		posX =   *pos++;
		posY =   *pos++;
		posZ =   *pos++;
		radius = *pos++;
	}
	
	float posX;
	float posY;
	float posZ;
	
	void setRadius(float r){
		assert(r!=0);
		radius = r;
	}
	
	float getRadius(){
		return radius;
	}
	
private: 
	
	float radius;

};

#endif /*CLOUD_MODEL_H*/