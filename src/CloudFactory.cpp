// Timothy Luciani

#include "CloudFactory.h"

static bool STATE_GROW_A, STATE_GROW_B, STATE_GROW_C;
static bool STATE_MOVE;
static char CUR_KEY;

static Potential_Matrix *The_Matrix;
double *texture_map;

WM5_WINDOW_APPLICATION(CloudFactory);

//----------------------------------------------------------------------------
CloudFactory::CloudFactory ()
:
WindowApplication3("Cloud Factory", 0, 0, 970, 650,
				   Float4(1.0f, 1.0f, 1.0f, 1.0f)),
					mTextColor(1.0f, 0.0f, 0.0f, 1.0f)
{}
//----------------------------------------------------------------------------
bool CloudFactory::OnInitialize ()
{
    if (!WindowApplication3::OnInitialize())
    {
        return false;
    }
	
    // Set up the camera.
    mCamera->SetFrustum(60.0f, 970/650, 0.1f, 100000.0f);
    APoint camPosition;
    AVector camDVector;

    camPosition = APoint(0.5f, 0.25f, 1.0f);
	camDVector = AVector(0.0f, 0.0f, -1.0f);
    
	Init_Texture();
	The_Matrix = new Potential_Matrix( MATRIX_SIZE );
	
    AVector camUVector(0.0f, 1.0f, 0.0f);
    AVector camRVector = camDVector.Cross(camUVector);
    mCamera->SetFrame(camPosition, camDVector, camUVector, camRVector);
	
	level = new Levels("../../src/Levels.txt");
	
    CreateScene(0);
	
    // Initial update of objects.
    mScene->Update();
	
    // Initial culling of scene.
    mCuller.SetCamera(mCamera);
    mCuller.ComputeVisibleSet(mScene);
	
    InitializeCameraMotion(0.01f, 0.001f);
    InitializeObjectMotion(mScene);
		
	Environment::InsertDirectory("../../resources/");
	current_cloud_a = current_cloud_b = current_cloud_c = -1; // initialize current cloud
	STATE_GROW_A = STATE_GROW_B = STATE_GROW_C =false; // set growing to false
	STATE_MOVE = true;
	CUR_KEY = ' ';
	ROTATE = 0;
	SEUIL = 1.0;
	PX_M2 = 335;
		
    return true;
}
//----------------------------------------------------------------------------
void CloudFactory::OnTerminate ()
{
    mScene = 0;
	
    WindowApplication3::OnTerminate();
}
//----------------------------------------------------------------------------
void CloudFactory::OnIdle ()
{
    MeasureTime();
	
	/* measure time between time time step and last */
	
	current_time = GetTimeInMicroseconds()/1000;
	int frame_time = current_time - previous_time;
	previous_time = current_time;

	float frame_step_time = frame_time/33.33;
	
	printf("frame step %f\n", frame_step_time);
	
//	std::list<CloudModel*> cl(clouds.begin(), clouds.end());
	
//	list<CloudModel*> blue = level->get_level(0)->get_model();
//	list<CloudModel*>::iterator it;
	
//	float score = BluePrintDetect::CalculateError(blue, cl);
//	printf("score: %f\n", score);
	
	if(clouds.size() > 0 && STATE_MOVE){ // move clouds up
		
		Time_Counter counter;
		
		The_Matrix->Reset();
		counter.restart();
		
		for(int i=0; i < (int)clouds.size(); i++)
		{
			clouds[i]->Move(frame_step_time);
			
			if(clouds[i]->Get_Py() > 2000){
				mScene->DetachChild(clouds[i]);
				clouds.erase(clouds.begin() + i);
			}
				
			
			clouds[i]->AddToMatrix(The_Matrix);
		}
		
		counter.stop();
		
		Marching_Cube_Begin(The_Matrix,STEP);
		for(int i=0; i < (int)clouds.size();i++){
			clouds[i]->SetChild(0, Draw_Iso_Surface_Around_Point( The_Matrix , SEUIL , STEP ,
																 (clouds[i]->Get_Px()/STEP)*STEP,
																 (clouds[i]->Get_Py()/STEP)*STEP,
																 (clouds[i]->Get_Pz()/STEP)*STEP) );		
			
			clouds[i]->Update();
			
		}
		
		mCuller.ComputeVisibleSet(mScene);
		
		Marching_Cube_End();	
		
	} // end if clouds

    if (mRenderer->PreDraw())
    {
        mRenderer->ClearBuffers();
        mRenderer->Draw(mCuller.GetVisibleSet());
        DrawFrameRate(8, GetHeight()-8, mTextColor);
        
		mRenderer->PostDraw();
        mRenderer->DisplayColorBuffer();
    }
	
	if (MoveCamera())
    {
		printf("move camera\n");
		mScene->Update();
        mCuller.ComputeVisibleSet(mScene);
    }
	
    if (MoveObject())
    {
		printf("move scene\n");
        mScene->Update();
        mCuller.ComputeVisibleSet(mScene);
    }
		
	mCuller.ComputeVisibleSet(mScene);

    UpdateFrameCount();
}
//----------------------------------------------------------------------------
void CloudFactory::Init_Texture()
{
	texture_map = new double[257*257*257];
	if( texture_map == NULL)
	{
		cout << "not enough memory for the 3D texture !! \n";
		exit(0);
	}
	
	memset( texture_map , 0 , 257*257*257*sizeof(double));
	
	double potential;
	for(int x=-128 ; x <= 128 ; x++)
		for(int y=-128 ; y <= 128 ; y++)
			for(int z=-128 ; z <= 128 ; z++)
				if( x*x+y*y+z*z < 128*128)
				{
					if( x==0 && y==0 && z==0)
						potential = 16384.0;
					else
						potential = ((16384.0 / ( x*x + y*y + z*z))-1.0);
					
					texture_map[(x+128)+(y+128)*257+(z+128)*257*257] = potential;			
				}
				else
					texture_map[(x+128)+(y+128)*257+(z+128)*257*257] = 0.0;			
}
//----------------------------------------------------------------------------
bool CloudFactory::OnKeyDown (unsigned char key, int x, int y)
{	
	
	switch (key) {
		
		case 'A': // cannon to the far left
		case 'a':
		{
			if(CUR_KEY != 'a'){
			
				Cannons *object = level->getCannon(0, 0);
			
				Transform loc = object->getTrans();
						
				APoint t = loc.GetTranslate();

				loc.SetTranslate(t+APoint(0.0f, 0.45f, 0.0f));			
				loc.SetScale(APoint(1.0f, 1.0f, 1.0f));
			
				printf("create cloud!\n");
				
				clouds.push_back(new Metaballs3D(85, 0, 0, 128)); // create new cloud
				current_cloud_a = clouds.size()-1; // get current index for growth
			
				mScene->AttachChild(clouds[clouds.size()-1]);
				
				STATE_GROW_A = true; // set to grow as long as the key is held
				CUR_KEY = 'a';
			}
			
			break;
		}
		case 'S': // middle cannon
		case 's':
		{
			if(CUR_KEY != 's' ){
				
				Cannons *object = level->getCannon(0, 1);
				
				Transform loc = object->getTrans();
				
				APoint t = loc.GetTranslate();
				
				loc.SetTranslate(t+APoint(0.0f, 0.45f, 0.0f));			
				loc.SetScale(APoint(1.0f, 1.0f, 1.0f));
				
				clouds.push_back(new Metaballs3D(350, 0, 0, 128)); // create new cloud
				current_cloud_b = clouds.size()-1; // get current index for growth
				
				mScene->AttachChild(clouds[clouds.size()-1]);
				
				STATE_GROW_B = true;
				CUR_KEY = 's';

			}
			break;
		}
		
		case 'D': // cannon to the far right
		case 'd':
		{
			if(CUR_KEY != 'd'){
				
				Cannons *object = level->getCannon(0, 2);
				
				Transform loc = object->getTrans();
				
				APoint t = loc.GetTranslate();
				
				loc.SetTranslate(t+APoint(0.0f, 0.45f, 0.0f));			
				loc.SetScale(APoint(1.0f, 1.0f, 1.0f));
				
				clouds.push_back(new Metaballs3D(575, 0, 0, 128)); // create new cloud
				current_cloud_c = clouds.size()-1; // get current index for growth
				
				mScene->AttachChild(clouds[clouds.size()-1]);
			
				STATE_GROW_C = true;
				CUR_KEY = 'd';
			}
			
			break;
		}
			
		default:
			break;
	}
	
    return true;
}

bool CloudFactory::OnKeyUp (unsigned char key, int x, int y)
{
	
	switch (key) { // key was released
		
		case 'a': // cannot to the far left
		case 'A':
			current_cloud_a = -1;
			STATE_GROW_A = STATE_GROW_B = STATE_GROW_C = false; // set growing to false
			CUR_KEY = ' ';
			break;
		case 's': // middle cannon
		case 'S':
			current_cloud_b = -1;
			STATE_GROW_A = STATE_GROW_B = STATE_GROW_C = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 'd': // cannon to the far right
		case 'D':{
			current_cloud_c = -1;
			STATE_GROW_A = STATE_GROW_B = STATE_GROW_C = false; // set growing to false
			CUR_KEY = ' ';
			break;
		}
		
		default:
			break;
	}
	
	    return true;
}
//----------------------------------------------------------------------------
bool CloudFactory::OnMouseClick (int button, int state, int x,
							 int y, unsigned int)
{
	
    if (state != MOUSE_DOWN && state != MOUSE_LEFT_BUTTON)
    {
		STATE_GROW_A = false; // stop growing
		current_cloud_a = -1; // end current cloud
		
        return false;
    }
	
	if (state == MOUSE_DOWN) {
		
		STATE_GROW_A = true;

		// Do a picking operation.
		APoint pos;
		AVector dir;
		if (mRenderer->GetPickRay(x, GetHeight() - 1 - y, pos, dir))
		{
			mPicker.Execute(mScene, pos, dir, 0.0f, Mathf::MAX_REAL);
			
			if (mPicker.Records.size() > 0)
			{
				// Display the selected object's name.
				const PickRecord& record = mPicker.GetClosestNonnegative();
				const Spatial* object = record.Intersected;
							
				if(object->GetName() == "Cannon"){ // if stack clicked
									
					printf("cannon!\n");
					Transform loc = object->LocalTransform;
					APoint t = loc.GetTranslate();
					
					loc.SetTranslate(t+APoint(0.0f, 0.45f, 0.0f));			
					loc.SetScale(APoint(1.0f, 1.0f, 1.0f));
					
					clouds.push_back(new Metaballs3D(350, 0, 0, 128)); // create new cloud
					current_cloud_a = clouds.size()-1; // get current index for growth
				
					mScene->AttachChild(clouds[clouds.size()-1]);
	//				mScene->Update();
					mCuller.ComputeVisibleSet(mScene);

				} // end if
				else if(object->GetName() == "metaball"){
						printf("found metaball\n");
				}
			} // end if
		} // end if pick  
					
	} // case mouse down

return true;
} 

void CloudFactory::CreateScene (int index)
{
	mScene = level->load_level(mScene, index);
	
}
