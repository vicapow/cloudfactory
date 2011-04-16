
#include "GraphicsView.h"
#include <iostream>

#include <QLabel>

using namespace std;

int main(int argc, char **argv) {
    
    QApplication app(argc,argv);
//	CloudFactory *game = new CloudFactory();
//    app.setActiveWindow(game);
//	game->show();


	
	GraphicsView view;

	view.resize(970,650);
	view.move( view.mapToGlobal( QPoint( 0, 0 ) ) );
	view.show();
	
	
	//example of drawing an image
	
//	QPixmap img("Night.jpeg");
//    QGraphicsPixmapItem item(img);
//    view.mainScene->addItem(&item);
	
	QLabel* label = new QLabel("Hello WOrld");
	label->move( view.width()/2 , view.height() /2 );
	view.mainScene->addWidget(label);
	
    return app.exec();
}
