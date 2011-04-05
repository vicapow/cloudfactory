/*
 *  Metaballs.h
 *  CloudFactory
 *
 *  Created by Timothy Luciani on 3/18/11.
 *  Copyright 2011 University of Pittsburgh. All rights reserved.
 *
 */

#include "CloudModel.h"
#include "marching_cube.h"
#include "Time_Counter.h"

#define MATRIX_SIZE 1500
#define STEP 16
#define STEP_B 4

extern double *texture_map;

class Potential_Matrix : public Potential_Field
{
	public :
	
	void Reset()
	{		
		int l = ((Size>>STEP_B)+1)*((Size>>STEP_B)+1)*((Size>>STEP_B)+1)*sizeof(double);		
		memset(matrix , 0 , l);
	};
	
	Potential_Matrix(int size): Potential_Field(size)
	{
		int l = (size>>STEP_B)+1;
		matrix = new double[l*l*l];
		if( matrix == NULL)
			std::cout << "error : not enough memory !!! \n" ;
		this->Reset();
		match = false;
	};
	
	
	
	/* get the potential for a grid cell */
	inline double Get_Potential( Point3D p)
	{		
		return matrix[(p.x>>STEP_B) + (p.y>>STEP_B)*((Size>>STEP_B)+1) + (p.z>>STEP_B)*((Size>>STEP_B)+1)*((Size>>STEP_B)+1)];
	};
	
	
	/* add potiential to a particular grid cell */
	inline void Set_Potential_world( int x ,int y, int z , double p)
	{		
		matrix[ (x>>STEP_B) + (y>>STEP_B)*((Size>>STEP_B)+1) + (z>>STEP_B)*((Size>>STEP_B)+1)*((Size>>STEP_B)+1) ] += p;
	};
	
	
	inline void Set_Potential_matrix( int x ,int y, int z , double p)
	{	
		matrix[x + y*((Size>>STEP_B)+1) + z*((Size>>STEP_B)+1)*((Size>>STEP_B)+1)] += p;
	};

	private:
	double *matrix;
	
};

class Metaballs3D : public Node, public CloudModel
{
	public :
	
	Metaballs3D(){}
	Metaballs3D(float px , float py , float pz, int rayon, float speed, std::string dir): CloudModel(px,py,pz,rayon)
	{
		Speed = speed;
		
		direction = dir;
		
		Rayon = rayon;
	}
	
	void AddToMatrix( Potential_Matrix * world )
	{
		unsigned char r =  (unsigned char) Rayon;
		unsigned char rapport=7;
		
		while( !(r & 0x01 ) )
		{
			r=r>>1;
			rapport--;
		}
		
		int x0 = ((int)posX - Rayon) >> STEP_B;
		int offsetx = (x0<<STEP_B) - ((int)posX - Rayon);
		if( offsetx != 0)
		{
			x0++;
			offsetx+=STEP;
		}
		
		int x1 = ( (int)posX +Rayon )>>STEP_B;		
		int y0 = ((int)posY - Rayon) >> STEP_B;
		
		int offsety = (y0<<STEP_B) - ((int)posY - Rayon);
		if( offsety != 0)
		{
			y0++;
			offsety=STEP;
		}
		
		int y1 = ((int)posY +Rayon )>>STEP_B;	
		int z0 = ((int)posZ - Rayon) >> STEP_B;
		
		int offsetz = (z0<<STEP_B) - ((int)posZ - Rayon);
		if( offsetz != 0)
		{
			z0++;
			offsetz+=STEP;
		}
		
		int z1 = ( (int)posZ + Rayon )>>STEP_B;		
		
		double potential;
		
		for(int x=x0 ; x<=x1; x++)
			for(int y=y0 ; y<=y1;y++)
				for(int z=z0 ; z<=z1; z++)
					if(    (x << STEP_B ) >=0 &&  (x << STEP_B ) <= MATRIX_SIZE
					   && (y << STEP_B ) >=0 &&  (y << STEP_B ) <= MATRIX_SIZE
					   && (z << STEP_B ) >=0 &&  (z << STEP_B ) <= MATRIX_SIZE )
					{					
						potential = texture_map[ ( (offsetx+((x-x0)<<STEP_B))<<rapport)
												+(((offsety+((y-y0)<<STEP_B))<<rapport)*257)
												+(((offsetz+((z-z0)<<STEP_B))<<rapport)*257*257)];
						world->Set_Potential_matrix( x , y , z , potential);					
					}
	}
	
	void Move(float time)
	{
		if(direction == "STRAIGHT")
			posY += (Speed * time);	
		
		else if(direction == "RIGHT"){
			posX += (Speed * time/3);
			posY += (Speed * time);	
		}
		else if(direction == "LEFT"){
			posX += (-Speed * time/3);
			posY += (Speed * time);	
		}
	}
	
	void Grow(float amount){
		
		posX += amount;
	}
	
	void SetSpeed(float speed){
		
		Speed = speed;
	}
	
	inline int Get_Px() { return (int)posX; };
	inline int Get_Py() { return (int)posY; };
	inline int Get_Pz() { return (int)posZ; };
	inline void Set_Px(float px) { posX = px;};
	private :
	
	int Rayon;
	float Speed;
	std::string direction;
};
