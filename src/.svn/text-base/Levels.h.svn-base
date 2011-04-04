/*
 *  Level.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 3/14/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "Wm5WindowApplication3.h"
#include "Background.h"
#include "Cannons.h"
#include "BluePrintDetect.h"
#include <iostream>
#include <vector>
#include "CloudModel.h"

using namespace Wm5;

class Level {
	
private:
	
	std::vector<Cannons*> cannons; // vector of cannons
	std::string m_background; // background image
	std::string blueprint; // blueprint
	std::list<CloudModel*> blue;

public:
	
	Level(){} // constructor
	~Level(){} // deconstructor
	
	void add_cannon(Cannons *c) { cannons.push_back(c); }
	void add_blue(CloudModel *c) { blue.push_back(c); }
	
	Cannons *get_cannon(int index) { return cannons[index]; }
	int num_cannons() { return cannons.size(); }
	std::list<CloudModel*> get_model() { return blue; } 
	
	int get_size(){ return blue.size(); }
	
	void set_name(std::string background) {m_background = background;}
	void set_blue(std::string background) {blueprint = background;}

	std::string get_name() { return m_background; }
	std::string get_blue() { return blueprint; }

}; // end level class

class Levels {
	
private:
	
	void parse_levels(std::string file);
	
public:
	
	Levels(std::string levels); // default constructor
	~Levels(); // deconstructor
	
	NodePtr load_level(NodePtr mScene, int index);
	Cannons *getCannon(int level, int cannon);
	Level *get_level(int index);
	
}; // end level class
