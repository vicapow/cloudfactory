

#ifndef CLOUDFACTORY_H
#define CLOUDFACTORY_H


//--STD--//

#include <GLUT/glut.h>
#include <iostream>

//--EXTERNAL-LIBS--//

#include "Wm5WindowApplication3.h"

#include <QtOpenGL>
#include <QTimer>
#include <QElapsedTimer>

//--LOCAL--//

#include "CloudModel.hpp"
#include "MetaballCanvas.hpp"




using namespace Wm5;
using namespace std;




class m_cloud {
	public:
	//constructor
	m_cloud( float posX, float posY, float posZ, float radius ){
		cout << "m clouds created!" << endl;
		
		//there should only be oe metaball for each cloud but for now, this value isn't being used.
		match = false;
		model = new CloudModel(posX,posY,posZ,radius);
		model->velY = 2;
	};

	void update(float frame_time){
		model->posY+= frame_time * model->velY;
		cout << "frame time: " << frame_time << endl;
	}

	CloudModel* model;
	bool match;

	//desctuctor
	~m_cloud(){
		cout << "m clouds destroied" << endl;
		delete model;
	}

};

class CloudFactory : public QGLWidget
{
    Q_OBJECT        // must include this if you use Qt signals/slots
	
public:
    
	CloudFactory( QWidget *parent = NULL )
	: QGLWidget(parent) {}
			
protected:
	
	/* QT */
    void initializeGL();
    void resizeGL( int w, int h );
	void paintGL();
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	
	/* game functions */
	
	void create_scene();
	void load_image(const char * filename);
	void display_image(int width, int height);
	
	/* data structures */
	MetaballCanvas* canvas;
	std::vector<CloudModel*> user_guess;
	list<struct m_cloud*> clouds;
	
	m_cloud* cur_cloud_a;
	m_cloud* cur_cloud_b;
	m_cloud* cur_cloud_c;
	
	GLuint texture;
	GLuint can_ind;
	QTimer* timer;//the timer that fires draw events
	QElapsedTimer elapsedTimer;
	
protected slots:
	void onEnterFrame();
	
};

#endif 
