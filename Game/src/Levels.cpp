/*
 *  Levels.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 4/16/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "Levels.h"

Levels::Levels(QString filename){
	
	QDomDocument doc;
	
	QFile file(filename);
	
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text )){
		cerr << "error reading level file" << endl;
		return;
	}
	QString errMsg;
	int errLine;
	int errColumn;
	QString xmlString = QString(file.readAll());
	if(!doc.setContent(xmlString,false,&errMsg,&errLine,&errColumn)){
		file.close();
		cerr << xmlString.toStdString() << endl;
		cerr << "error parsing level file " << endl;
		cerr << "errLine: " << errMsg.toStdString() << endl << "line: " << errLine << "col: " << errColumn << endl;
		return;
	}
	file.close();
	
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();//levels
	while(!n.isNull()){
		QDomElement levelElem = n.toElement();//level
		if(!levelElem.isNull()){
			cout << qPrintable(levelElem.tagName()) << endl;
			Level level;
			level.num_cannons = levelElem.attribute("cannons", "0").toInt();
			QDomNode modelNode = levelElem.firstChild();
			while(!modelNode.isNull()){
				QDomElement modelElem = modelNode.toElement();
				if( modelElem.tagName() == "model" ){
					CloudModel* model = new CloudModel( modelElem.attribute("x","0").toInt() , modelElem.attribute("y","0").toInt() , modelElem.attribute("z","0").toInt() , modelElem.attribute("r","0").toInt() );
					level.blueprint.push_back(model);
					cout << " adding model to blueprint: y: " << level.blueprint[level.blueprint.size()-1]->posY << endl;
				}
				modelNode = modelNode.nextSibling();
			}
			levels.push_back(level);
		}
		n = n.nextSibling();
	}
	
	print();
}

void Levels::print(){
	for(int i = 0; i < levels.size(); i++){
		cout << " level: num_cannons : " << levels[i].num_cannons << endl;
		for(int j = 0; j < levels[i].blueprint.size();j++){
			CloudModel* model = levels[i].blueprint[j];
			cout << "\t model: x: " << model->posX << " y: " << model->posY << " z: " << model->posZ << " r: " << model->getRadius() << endl;
		}
	}
}