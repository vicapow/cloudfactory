#include "CloudFactory.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    
    QApplication app(argc,argv);
	CloudFactory *game = new CloudFactory();
    app.setActiveWindow(game);
	game->show();
    return app.exec();
}
