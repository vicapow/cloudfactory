
#include <QLabel>

#include <iostream>

#include "GraphicsView.h"
#include "HomeScreen.h"
#include "Levels.h"

using namespace std;

int main(int argc, char **argv) {
    
    QApplication app(argc,argv);

	Levels level(":/resources/levels.xml");
	
//	GraphicsView view;
//
//	view.resize(970,650);
//	view.move( view.mapToGlobal( QPoint( 0, 0 ) ) );
//	view.show();
	
    return app.exec();
}
