/**
 * Author: Erik Smistad
 * www.thebigblob.com
**/

#include <GLUT/glut.h>
#include "marchingcubes.hpp"
#include "mygldrawer.h"
#include <iostream>

using namespace std;



int main(int argc, char **argv) {
    
    QApplication app(argc,argv);
    MyGLDrawer* drawer = new MyGLDrawer();
    app.setActiveWindow(drawer);
    drawer->show();
    return app.exec();
}
