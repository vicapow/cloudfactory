// Timothy Luciani

#ifndef WARMUP_H
#define WARMUP_H

#include "BluePrintDetect.h"
#include "Metaballs.h"
#include "Levels.h"
#include <vector>

struct m_cloud{
	
	m_cloud(){};
	
	std::vector<Metaballs3D*> clouds;
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
	
protected:
    void CreateScene (int level);
	
	std::vector<TriMesh*> mesh;
    NodePtr mScene;
    Culler mCuller;

	int64_t previous_time;
	int64_t current_time;
	
    Float4 mTextColor;
	Picker mPicker;
	Levels *level;
	
	bool ROTATE;
	double  SEUIL;
		
	int PX_M2;
	
	int current_cloud_a, current_cloud_b, current_cloud_c; // cloud being edited
	std::vector<struct m_cloud> blobs; // vector of clouds
};

WM5_REGISTER_INITIALIZE(CloudFactory);
WM5_REGISTER_TERMINATE(CloudFactory);

#endif
