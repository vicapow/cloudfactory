// Timothy Luciani

#ifndef WARMUP_H
#define WARMUP_H

#include "BluePrintDetect.h"
#include "Metaballs.h"
#include "Levels.h"
#include "CloudModel.h"
#include <vector>
#include <list>
#include "marchingcubes.hpp"
#include "MetaballCanvas.h"

using namespace std;

class m_cloud {
	
public:
	//constructor
	m_cloud(){
		cout << "m clouds created!" << endl;
		
		//there should only be oe metaball for each cloud but for now, this value isn't being used.
		match = false;
	};
	
	std::vector<Metaballs3D*> clouds;
	
	bool match;
	
	//desctuctor
	~m_cloud(){
		cout << "m clouds destroied" << endl;
	}
	
};

class CloudFactory : public WindowApplication3
{
    WM5_DECLARE_INITIALIZE;
    WM5_DECLARE_TERMINATE;
	
public:
    CloudFactory ();
	
    virtual bool OnInitialize ();
    virtual void OnTerminate ();
    virtual void OnIdle ();
    virtual bool OnKeyDown (unsigned char key, int x, int y);
	virtual bool OnKeyUp (unsigned char key, int x, int y);
	virtual bool OnMouseClick (int button, int state, int x,
							   int y, unsigned int);
	void Init_Texture();
	
	Node* metaballVertices;
	
	MetaballCanvas* canvas;
protected:
    void CreateScene (int level);
	TriMesh* CreateTorus();
	
	std::vector<TriMesh*> mesh;
    NodePtr mScene;
    Culler mCuller;

	int64_t previous_time;
	int64_t current_time;
	
    Float4 mTextColor;
	Picker mPicker;
	Levels *level;

	double  SEUIL;
	
	int current_cloud_a, current_cloud_b, current_cloud_c; // cloud being edited
	int current_model_a, current_model_b, current_model_c; // current cloud model
	std::vector<struct m_cloud*> blobs; // vector of clouds
	vector<CloudModel*> user_guess;
};

WM5_REGISTER_INITIALIZE(CloudFactory);
WM5_REGISTER_TERMINATE(CloudFactory);

#endif
