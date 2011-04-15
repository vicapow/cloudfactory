#include "CloudFactory.hpp"
#include "Cannons.hpp"

/* globals used to determine state of game */

static bool STATE_GROW_A, STATE_GROW_B, STATE_GROW_C;
static bool STOP_GROWTH_A, STOP_GROWTH_B, STOP_GROWTH_C;
static char CUR_KEY;

static Wm5::Float3 location_c, location_b, location_a;
static Wm5::Float3 incrementer_c, incrementer_b, incrementer_a;

GLfloat posX, posY, posZ;


void CloudFactory::initializeGL()
{
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	// set camera positions

	load_image("Night.raw");
	
	create_scene();
	
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(onEnterFrame()));
	timer->start(0);
	
	elapsedTimer.start();
	
}

void CloudFactory::resizeGL( int w, int h )
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 1000.0);
	gluLookAt(canvas->SX/2, canvas->SY/2, canvas->SZ*10 , canvas->SX/2, canvas->SY/2, 0, 0, 1, 0);
}

void CloudFactory::onEnterFrame(){

	int frame_time = elapsedTimer.restart();
	list<m_cloud*>::iterator it;
	for( it = clouds.begin(); it != clouds.end(); it++){
		(*it)->update(frame_time/50.0f);
	}
	
	paintGL();
	
	//cout << "on enter frame " << endl;
	
}

void CloudFactory::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
    glMatrixMode(GL_MODELVIEW);
	// specify the lists to be drawn
	GLubyte lists[3]; 
	lists[0] = 0; lists[1] = 1; lists[2] = 2;
	
	// draw lists
	glListBase(can_ind);
	glCallLists(3, GL_UNSIGNED_BYTE, lists);
	
	display_image(640, 480);
	
	
	// Draw the triangles
    canvas->draw();
	
	glFlush();
	swapBuffers();
}

void CloudFactory::keyPressEvent(QKeyEvent *event){

	static float DEFAULT_RADIUS = 20;
	
	switch (event->key()) {
			
		case 'A': // cannon to the far right
		case 'a':
		{
			if(CUR_KEY != 'a'){
				
				//add a cloud from factory A
				clouds.push_back( cur_cloud_a = new m_cloud(canvas->SX/4,0,0,DEFAULT_RADIUS));
				
				//submit the added cloud to the user_guess list of clouds
				user_guess.push_back(cur_cloud_a->model);
				canvas->addMetaball(cur_cloud_a->model);
				
				
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
				
				clouds.push_back( cur_cloud_b = new m_cloud(canvas->SX/2,0,0,DEFAULT_RADIUS));
				user_guess.push_back(cur_cloud_b->model);
				canvas->addMetaball(cur_cloud_b->model);
				
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
				
				clouds.push_back( cur_cloud_c = new m_cloud(canvas->SX - canvas->SX/4 ,0,0,DEFAULT_RADIUS));
				user_guess.push_back(cur_cloud_c->model);
				canvas->addMetaball(cur_cloud_c->model);
				
				STOP_GROWTH_C = false;
				STATE_GROW_C = true;
				CUR_KEY = 'd';
			}
			
			break;
		}
		case 'r': // reset
		case 'R':
		{
			
			user_guess.clear(); // clear guesses
			
			for(unsigned int i = 0; i < clouds.size(); i++){
			}
			
			//delete all the clouds
			list<m_cloud*>::iterator it;
			for( it = clouds.begin(); it != clouds.end(); it++){
				canvas->removeMetaball( (*it)->model );
				delete (*it);
			}
			
			clouds.clear(); // clear clouds list
			
		}break;
			
		default:
			break;
	}
} // end create scene

void CloudFactory::keyReleaseEvent(QKeyEvent *event){
	
	switch (event->key()) { // key was released
			
		case 'a': // cannot to the far left
		case 'A':
			
			cur_cloud_a = NULL;
			STATE_GROW_A = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 's': // middle cannon
		case 'S':
			
			cur_cloud_b = NULL;
			STATE_GROW_B = false; // set growing to false
			CUR_KEY = ' ';
			break;
			
		case 'd': // cannon to the far right
		case 'D':{
			cur_cloud_c = NULL;
			STATE_GROW_C = false; // set growing to false
			CUR_KEY = ' ';
			break;
		}
			
		default:
			break;
	}
		
} // end key release

void CloudFactory::load_image(const char * filename){
	
    int width, height;
    char * data;
    FILE * file;
	
    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return;
	
    // allocate buffer
    width = 256;
    height = 256;
    data = (char*)malloc( width * height * 3 );
	
    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );
	
    // allocate a texture name
    glGenTextures( 1, &texture );
	
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
	
    // free buffer
    free( data );
}

void CloudFactory::display_image(int width, int height)
{
	// bind and draw the foreground texture
	
	glEnable(GL_TEXTURE_2D);   
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f); glVertex2f(0     , 0 +  0);
	glTexCoord2f(1.0f,0.0f); glVertex2f(1, 0 +  0);
	glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
	glTexCoord2f(0.0f,1.0f); glVertex2f(0     , 1);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void CloudFactory::create_scene(){
	
	makeCurrent();
	
	can_ind = glGenLists(3);
	
	/* cannon 1 */
	glNewList(can_ind, GL_COMPILE);
	
	APoint trans = APoint(2.0, -4.0, 0.0);
	APoint scale = APoint(1.0, 1.0, 1.0);
	
	CreateCannon(trans, scale);
	
	glEndList();

	/* cannon 2 */
	
	glNewList(can_ind+1, GL_COMPILE);
	
	trans = APoint(0.0, -4.0, 0.0);
	scale = APoint(1.0, 1.0, 1.0);
	
	CreateCannon(trans, scale);
	
	glEndList();

	
	/* cannon 2 */
	
	glNewList(can_ind +2, GL_COMPILE);
	
	trans = APoint(-2.0, -4.0, 0.0);
	scale = APoint(1.0, 1.0, 1.0);
	
	CreateCannon(trans, scale);
	
	glEndList();
	
	
	canvas = new MetaballCanvas();
	canvas->init();
	
//	canvas->LocalTransform.SetScale(APoint(0.4,0.4,0.4));
//	canvas->LocalTransform.SetTranslate(APoint(0.610f,-0.1,0.1f));
//	canvas->draw();
	
//	mScene->AttachChild(canvas);

}

