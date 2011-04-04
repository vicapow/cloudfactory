/*
 *  Level.cpp
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 3/14/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "Levels.h"

std::vector<Level*> levels;

Levels::Levels(std::string file){

	parse_levels(file); // parse levels	
}

Levels::~Levels(){
	levels.clear();
}

void Levels::parse_levels(std::string filename){

	FILE *fp;
	char *peek; /* used to peek at the first character */
	char *c_string; /* string to be read in */
	long int filesize = 0; /* size of the file being read */
	
	fp = fopen(filename.c_str(), "r"); /* open file */
	
	if(fp == NULL){ /* see if the file was opened */
		
		printf("Problem opening file\n");
		return;
		
	} // end if
	
	fseek(fp, 0, SEEK_END); /* go to the end */
	filesize = ftell(fp); /* tell the size */
	fseek(fp, 0, SEEK_SET); /* go back to the beginnnig */
	
	c_string = new char[filesize]; /* allocate memory for file */
	
	fread(c_string, 1, filesize, fp); /* read entire file */
	fclose(fp); /* close the file */

	Level *L; 
	
	peek = strtok (c_string, " \t\n#:");

	while(peek != NULL){
				
		if(strncmp(peek, "Level", 5) == 0){
			
			L = new Level();
			peek = strtok (NULL, " \t\n#:,");
		}
		else if(strncmp(peek, "Cannon", 6) == 0){
		  
			while (strncmp(peek, "Cannon", 6) == 0) {
								
				APoint trans;
				trans[0] = atof(peek = strtok (NULL, " ,"));
				trans[1] = atof(peek = strtok (NULL, " ,"));
				trans[2] = atof(peek = strtok (NULL, " ,"));
				
				APoint scale;
				scale[0] = atof(peek = strtok (NULL, " ,"));
				scale[1] = atof(peek = strtok (NULL, " ,"));
				scale[2] = atof(peek = strtok (NULL, " ,"));
				
				char *axis = peek = strtok (NULL, " ,");
				
				HMatrix rot;
				rot.MakeIdentity();
				
				if(*axis == 'X'){
					
					rot = rot * HMatrix(AVector::UNIT_X, Mathf::DEG_TO_RAD * atof(peek = strtok (NULL, " \n")) );
					
				} // end if
				if(*axis == 'Y'){
					
					rot = rot * HMatrix(AVector::UNIT_Y, Mathf::DEG_TO_RAD * atof(peek = strtok (NULL, " \n")) );
					
				} // end if
				if(*axis == 'Z'){
					
					printf("rot Z\n");
					
					rot = HMatrix(AVector::UNIT_Z, Mathf::DEG_TO_RAD * atof(peek = strtok (NULL, " \n")) );
					
				} // end if
				
				L->add_cannon(new Cannons(trans, rot, scale));
			
				peek = strtok (NULL, " \t\n#:,");
				
			} // end while
		  
		} // end else if
		
		else if(strncmp(peek, "Background", 10) == 0){
		
			L->set_name(peek = strtok (NULL, " \n") );
			peek = strtok (NULL, " \t\n#:");
		}
		else if(strncmp(peek, "Blueprint", 9) == 0){
			
			L->set_blue(peek = strtok (NULL, " \n\r") );
			peek = strtok (NULL, " \t\n#:");
		}
		else if(strncmp(peek, "Detect", 6) == 0){
			
			peek = strtok (NULL, " \t\n#:,");

			while (strncmp(peek, "BP", 2) == 0) {
				
				float radius = atof(peek = strtok (NULL, " ,"));
				
				APoint trans;
				trans[0] = atof(peek = strtok (NULL, " ,"));
				trans[1] = atof(peek = strtok (NULL, " ,"));
				trans[2] = atof(peek = strtok (NULL, " ,\n\r"));
				
				CloudModel* model = new CloudModel();
				model->radius = radius;
				model->posX = trans.X();
				model->posY = trans.Y();
				model->posZ = trans.Z();
												
				L->add_blue(model);
								
				peek = strtok (NULL, " \r\n\t#:,");
				
			} // end while
		}
		else if(strncmp(peek, "END", 3) == 0){
			
			levels.push_back(L);
			return;
		}
		else
			peek = strtok (NULL, " \t\n#:");
		
	}// end while
	
} // end parse levels

NodePtr Levels::load_level(NodePtr mScene, int index){
	
	mScene = new0 Node();
	
	Level *L = levels[index]; 
			
	for(int ii = 0; ii < L->num_cannons(); ii++) {
		mScene->AttachChild(L->get_cannon(ii));
	}
	
	TriMesh *bkgd = CreateSquare(L->get_name()); /* background square */
	TriMesh *blue = CreateBlueprint(L->get_blue());
	
	bkgd->LocalTransform.SetTranslate(APoint(0.4f, 0.26f, -0.72f));
	bkgd->LocalTransform.SetScale(APoint(0.75, 0.75, 0.75));
	
	mScene->AttachChild(bkgd);
	mScene->AttachChild(blue);

	
	return mScene;
}

Cannons *Levels::getCannon(int level, int cannon){

	return levels[level]->get_cannon(cannon);
}

Level *Levels::get_level(int index){ return levels[index]; }