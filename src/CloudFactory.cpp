// Timothy Luciani

#include "CloudFactory.h"

static bool STATE_GROW_A, STATE_GROW_B, STATE_GROW_C;
static bool STOP_GROWTH_A, STOP_GROWTH_B, STOP_GROWTH_C;
static bool STATE_MOVE;
static char CUR_KEY;

static Wm5::Float3 location_c, location_b, location_a;
static Wm5::Float3 incrementer_c, incrementer_b, incrementer_a;

static Potential_Matrix *The_Matrix;
double *texture_map;

/* an example blueprint */
const int BP_LEN = 3;
const float BP[BP_LEN][4] = { { 670, 500, 0 , 75 } , { 825, 500, 0 , 75 } , { 975,500,0 , 75 } };
vector<CloudModel*> example_blueprint;


int size = 64;

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
	
	//Environment::InsertDirectory("../../resources/");
	
	canvas = new MetaballCanvas();
	
	for(int i = 0; i < BP_LEN; i++){
		example_blueprint.push_back(new CloudModel(BP[i]));
		cout << " adding blueprint: x" << BP[i][0] << " y: " << BP[i][1] << " z: " << BP[i][2] << " r: " << BP[i][3] << endl;
	}
	
    // Set up the camera.
    mCamera->SetFrustum(60.0f, 970/650, 0.1f, 1000.0f);
    APoint camPosition;
    AVector camDVector;

    camPosition = APoint(0.42f, 0.2f, 0.45f);
	camDVector = AVector(0.0f, 0.0f, -1.0f);
    
	Init_Texture();
	The_Matrix = new Potential_Matrix( MATRIX_SIZE );
	
    AVector camUVector(0.0f, 1.0f, 0.0f);
    AVector camRVector = camDVector.Cross(camUVector);
    mCamera->SetFrame(camPosition, camDVector, camUVector, camRVector);
	
	level = new Levels("../../src/Levels.txt");
	
    CreateScene();
	
    // Initial update of objects.
    mScene->Update();
	
    // Initial culling of scene.
    mCuller.SetCamera(mCamera);
    mCuller.ComputeVisibleSet(mScene);
	
    InitializeCameraMotion(0.01f, 0.001f);
    InitializeObjectMotion(mScene);
		
	current_cloud_a = current_cloud_b = current_cloud_c = -1; // initialize current cloud
	STATE_GROW_A = STATE_GROW_B = STATE_GROW_C =false; // set growing to false
	STOP_GROWTH_A = STOP_GROWTH_B = STOP_GROWTH_C = false;
	STATE_MOVE = true;
	CUR_KEY = ' ';
	SEUIL = 1;
		
	incrementer_a = Wm5::Float3(1.0, 1.0, 1.0);
	incrementer_b = Wm5::Float3(1.0, 1.0, 1.0);
	incrementer_c = Wm5::Float3(1.0, 1.0, 1.0);

    return true;
}

//----------------------------------------------------------------------------
void CloudFactory::OnTerminate ()
{
    mScene = 0;
	blobs.clear();
    WindowApplication3::OnTerminate();
}


//----------------------------------------------------------------------------
void CloudFactory::OnIdle () // aka, On Enter Frame
{
    MeasureTime();
		
	/* measure time between time time step and last */
	
	canvas->draw();
	
	current_time = GetTimeInMicroseconds()/1000;
	int frame_time = current_time - previous_time;
	previous_time = current_time;

	float frame_step_time = frame_time/33.33;
	
	cout << "frame_step_time: " << frame_step_time << endl;
	
	
	for(int i = 0; i < blobs.size(); i++){
		blobs[i]->update(frame_step_time);
	}
	
	Time_Counter counter;
	counter.restart();

	mCuller.ComputeVisibleSet(mScene);

	
#pragma mark Cloud Blueprint Detection
	
    
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
	static float DEFAULT_RADIUS = 20;
	
	switch (key) {
		
		case 'A': // cannon to the far right
		case 'a':
		{
			if(CUR_KEY != 'a'){
				
				//add a cloud from factory A
				blobs.push_back(new m_cloud(canvas->SX/4,0,0,DEFAULT_RADIUS));
				current_cloud_a = blobs.size() - 1;
				
				
				//submit the added cloud to the user_guess list of clouds
				user_guess.push_back(blobs[current_cloud_a]->model);
				canvas->addMetaball(blobs[current_cloud_a]->model);

				
				STOP_GROWTH_A = false;
				STATE_GROW_A = true;
				CUR_KEY = 'a';
			}
			
			break;
		}
		case 'S': // cannon to the far right
		case 's':
		{
			if(CUR_KEY != 's'){
				
				blobs.push_back(new m_cloud(canvas->SX/2,0,0,DEFAULT_RADIUS));
				
				current_cloud_b = blobs.size() - 1; // get current index for growth
				
				user_guess.push_back(blobs[current_cloud_b]->model);
				canvas->addMetaball(blobs[current_cloud_b]->model);
				
				STOP_GROWTH_B = false;
				STATE_GROW_B = true;
				CUR_KEY = 's';
			}
			
			break;
		}
		
		case 'D': // cannon to the far right
		case 'd':
		{
			if(CUR_KEY != 'd'){
				
				blobs.push_back( new m_cloud( canvas->SX - canvas->SX/4, 0, 0, DEFAULT_RADIUS ) );
								
				current_cloud_c = blobs.size() - 1; // get current index for growth
				
				user_guess.push_back(blobs[current_cloud_c]->model);
				canvas->addMetaball(blobs[current_cloud_c]->model);
			
				STOP_GROWTH_C = false;
				STATE_GROW_C = true;
				CUR_KEY = 'd';
			}
			
			break;
		}
		case 'r': // reset
		case 'R':
		{
			
			for(unsigned int i = 0; i < blobs.size(); i++){
				canvas->removeMetaball(blobs[i]->model);
			}
			
			blobs.clear(); // clear blobs
			user_guess.clear(); // clear guesses
			
			mCuller.ComputeVisibleSet(mScene); // update scene
		}break;
			
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
			STATE_GROW_A = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 's': // middle cannon
		case 'S':
			
			current_cloud_b = -1;
			STATE_GROW_B = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 'd': // cannon to the far right
		case 'D':{
			current_cloud_c = -1;
			STATE_GROW_C = false; // set growing to false
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
	
    //if (state != MOUSE_DOWN && state != MOUSE_LEFT_BUTTON)
//    {
//		STATE_GROW_A = false; // stop growing
//		current_cloud_a = -1; // end current cloud
//		
//        return false;
//    }
//	
//	if (state == MOUSE_DOWN) {
//		
//		STATE_GROW_A = true;
//
//		// Do a picking operation.
//		APoint pos;
//		AVector dir;
//		if (mRenderer->GetPickRay(x, GetHeight() - 1 - y, pos, dir))
//		{
//			mPicker.Execute(mScene, pos, dir, 0.0f, Mathf::MAX_REAL);
//			
//			if (mPicker.Records.size() > 0)
//			{
//				// Display the selected object's name.
//				const PickRecord& record = mPicker.GetClosestNonnegative();
//				const Spatial* object = record.Intersected;
//							
//				if(object->GetName() == "Cannon"){ // if stack clicked
//									
//					printf("cannon!\n");
//					Transform loc = object->LocalTransform;
//					APoint t = loc.GetTranslate();
//					
//					loc.SetTranslate(t+APoint(0.0f, 0.45f, 0.0f));			
//					loc.SetScale(APoint(1.0f, 1.0f, 1.0f));
//					
////					clouds.push_back(new Metaballs3D(350, 0, 0, 128, 4)); // create new cloud
////					current_cloud_a = clouds.size()-1; // get current index for growth
//				
////					mScene->AttachChild(clouds[clouds.size()-1]);
//	//				mScene->Update();
//					mCuller.ComputeVisibleSet(mScene);
//
//				} // end if
//				else if(object->GetName() == "metaball"){
//						printf("found metaball\n");
//				}
//			} // end if
//		} // end if pick  
//					
//	} // case mouse down

return true;
}


void CloudFactory::CreateScene ()
{
	
	//cout << "ball1: " << ball1 << endl;
	
	mScene = level->load_level(mScene, 0);
	
	canvas->init();
	canvas->LocalTransform.SetScale(APoint(0.4,0.4,0.4));
	canvas->LocalTransform.SetTranslate(APoint(0.610f,-0.1,0.1f));
	canvas->draw();
	
	mScene->AttachChild(canvas);
	
}
