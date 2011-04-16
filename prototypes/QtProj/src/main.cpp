
#include "GraphicsView.h"
#include <iostream>

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
	
//	QGraphicsScene scene;
//    QGraphicsView view(&scene);
//	QPixmap img("Night.jpeg");
//	
//    QGraphicsPixmapItem item(img);
//    scene.addItem(&item);
//    view.show();
	
	
    return app.exec();
}
