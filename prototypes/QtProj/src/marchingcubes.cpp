/**
 * Author: Erik Smistad
 * www.thebigblob.com
**/

#include "marchingcubes.hpp"
//vector<vertex> vertexList;
list<vertex> vertexList;

uchar *** parseRawFile(char * filename, int sizeX, int sizeY, int sizeZ) {
    uchar *** voxels = new uchar**[sizeX];
    FILE * file = fopen(filename, "rb");
    if(file == NULL) {
        printf("File not found: %s", filename);
        exit(-1);
    }

    for(int x = 0; x < sizeX; x++) {
        voxels[x] = new uchar*[sizeY];
        for(int y = 0; y < sizeY; y++) {
            voxels[x][y] = new uchar[sizeZ];
            fread(voxels[x][y], sizeof(uchar), sizeZ, file);
        }
    }
    return voxels;
}

uchar *** newVoxelMatrix(int sizeX, int sizeY, int sizeZ ){
    uchar *** voxels = new uchar**[sizeX];

    for(int x = 0; x < sizeX; x++) {
        voxels[x] = new uchar*[sizeY];
        for(int y = 0; y < sizeY; y++) {
            voxels[x][y] = new uchar[sizeZ];
			for(int z = 0; z < sizeZ; z++){
				voxels[x][y][z] = 0;
			}
        }
    }
    return voxels;
}

void initVoxels(float *** voxels, int sizeX, int sizeY, int sizeZ ){
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
			for(int z = 0; z < sizeZ; z++){
				voxels[x][y][z] = 0;
			}
        }
    }
}

float *** newFloatMatrix(int sizeX, int sizeY, int sizeZ ){
    float *** voxels = new float**[sizeX];

    for(int x = 0; x < sizeX; x++) {
        voxels[x] = new float*[sizeY];
        for(int y = 0; y < sizeY; y++) {
            voxels[x][y] = new float[sizeZ];
			for(int z = 0; z < sizeZ; z++){
				voxels[x][y][z] = 0;
			}
        }
    }
    return voxels;
}

void convertFloatToUCharVoxel(float *** metaballs, uchar *** voxels, int sizeX, int sizeY, int sizeZ ){
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
			for(int z = 0; z < sizeZ; z++){
				voxels[x][y][z] = floor(metaballs[x][y][z] + 0.5 );
			}
        }
    }
}

void drawMetaball(float ***voxels,int sizeX,int sizeY,int sizeZ, int px, int py, int pz, int R ){
	
	int maxR = R*10;//cache R*2 because we'll be using it frequently
	
	//upper and lower bounds for optimization
	
	// -- X 
		int lx = px - maxR;
		if(lx<0) lx = 0;
	
		int ux = px + maxR;
		if(ux>sizeX) ux = sizeX;
	
	
	// -- Y 
		int ly = py - maxR;
		if(ly<0) ly = 0;
	
		int uy = py + maxR;
		if(uy>sizeY) uy = sizeY;
	
	// -- Z 
//		int lz = pz - maxR;
//		if(lz<0) lz = 0;
//	
//		int uz = pz + maxR;
//		if(uz>sizeZ) uz = sizeZ;
	
    for(int x = lx; x < ux; x++) {
        for(int y = ly; y < uy; y++) {
			for(int z = 0; z < sizeZ; z++){
				voxels[x][y][z] += ( (R) / (float)( (x-px)*(x-px) + (y-py)*(y-py) + (z-pz)*(z-pz)*4 ) );
			}
        }
    }
}

void interpolate( double isolevel, vertex p1, vertex p2, float valp1, float valp2, vertex& p ) {
    
	if(fabs(isolevel - valp1) < 0.00001){
		p = p1;
        return;
	}
    if(fabs(isolevel - valp2) < 0.00001){
		p = p2;
		return;
	}
    if(fabs(valp1 - valp2) < 0.00001){
		p = p1;
        return;
	}

    double diff = (double)(isolevel - valp1) / (valp2 - valp1);
    p.x = p1.x + diff * (p2.x - p1.x);
    p.y = p1.y + diff * (p2.y - p1.y);
    p.z = p1.z + diff * (p2.z - p1.z);

    p.normal_x = p1.normal_x + diff * (p2.normal_x - p1.normal_x);
    p.normal_y = p1.normal_y + diff * (p2.normal_y - p1.normal_y);
    p.normal_z = p1.normal_z + diff * (p2.normal_z - p1.normal_z);

}

void processCube(cube cube, double isolevel) {
    int cubeindex = 0;
    if(cube.val[0] > isolevel) cubeindex |= 1;
    if(cube.val[1] > isolevel) cubeindex |= 2;
    if(cube.val[2] > isolevel) cubeindex |= 4;
    if(cube.val[3] > isolevel) cubeindex |= 8;
    if(cube.val[4] > isolevel) cubeindex |= 16;
    if(cube.val[5] > isolevel) cubeindex |= 32;
    if(cube.val[6] > isolevel) cubeindex |= 64;
    if(cube.val[7] > isolevel) cubeindex |= 128;

    // Cube is entirely in/out of the surface
    if(edgeTable[cubeindex] == 0 || edgeTable[cubeindex] == 255)
        return;

    vertex vertlist[12];
    // Find the vertices where the surface intersects the cube
    if(edgeTable[cubeindex] & 1)
        interpolate(isolevel,cube.p[0],cube.p[1],cube.val[0],cube.val[1], vertlist[0] );
    if(edgeTable[cubeindex] & 2)
        interpolate(isolevel,cube.p[1],cube.p[2],cube.val[1],cube.val[2], vertlist[1]);
    if(edgeTable[cubeindex] & 4)
        interpolate(isolevel,cube.p[2],cube.p[3],cube.val[2],cube.val[3], vertlist[2] );
    if(edgeTable[cubeindex] & 8)
        interpolate(isolevel,cube.p[3],cube.p[0],cube.val[3],cube.val[0], vertlist[3] );
    if(edgeTable[cubeindex] & 16)
        interpolate(isolevel,cube.p[4],cube.p[5],cube.val[4],cube.val[5], vertlist[4] );
    if(edgeTable[cubeindex] & 32)
        interpolate(isolevel,cube.p[5],cube.p[6],cube.val[5],cube.val[6], vertlist[5] );
    if(edgeTable[cubeindex] & 64)
        interpolate(isolevel,cube.p[6],cube.p[7],cube.val[6],cube.val[7], vertlist[6] );
    if(edgeTable[cubeindex] & 128)
        interpolate(isolevel,cube.p[7],cube.p[4],cube.val[7],cube.val[4], vertlist[7] );
    if(edgeTable[cubeindex] & 256)
        interpolate(isolevel,cube.p[0],cube.p[4],cube.val[0],cube.val[4], vertlist[8] );
    if(edgeTable[cubeindex] & 512)
        interpolate(isolevel,cube.p[1],cube.p[5],cube.val[1],cube.val[5], vertlist[9] );
    if(edgeTable[cubeindex] & 1024)
		interpolate(isolevel,cube.p[2],cube.p[6],cube.val[2],cube.val[6], vertlist[10] );
    if(edgeTable[cubeindex] & 2048)
        interpolate(isolevel,cube.p[3],cube.p[7],cube.val[3],cube.val[7], vertlist[11] );

    for(int i = 0; triTable[cubeindex][i] != -1; i++) {
        vertexList.push_back(vertlist[triTable[cubeindex][i]]);
    }
}

void clearVertexList(){
	vertexList.erase(vertexList.begin(),vertexList.end());
}

list<vertex>& runMarchingCubes(float ***voxels, int sizeX, int sizeY, int sizeZ, 
        int stepX, int stepY, int stepZ, double isovalue) {
    // Run the processCube function on every cube in the grid
	for(int x = stepX; x < sizeX-2*stepX; x += stepX) {
		for(int y = stepY; y < sizeY-2*stepY; y += stepY) {
			for(int z = stepZ; z < sizeZ-2*stepZ; z += stepZ) {
				cube c = {{
					{x,y,z, 
                        (double)(voxels[x+stepX][y][z]-voxels[x-stepX][y][z]) / -stepX,
                        (double)(voxels[x][y+stepY][z]-voxels[x][y-stepY][z]) / -stepY,
                        (double)(voxels[x][y][z+stepZ]-voxels[x][y][z-stepZ]) / -stepZ
                    },
					{x+stepX,y,z, 
                        (double)(voxels[x+2*stepX][y][z]-voxels[x][y][z]) / -stepX,
                        (double)(voxels[x+stepX][y+stepY][z]-voxels[x+stepX][y-stepY][z]) / -stepY,
                        (double)(voxels[x+stepX][y][z+stepZ]-voxels[x+stepX][y][z-stepZ]) / -stepZ
                    },
					{x+stepX,y,z+stepZ, 
                        (double)(voxels[x+2*stepX][y][z+stepZ]-voxels[x][y][z+stepZ]) / -stepX,
                        (double)(voxels[x+stepX][y+stepY][z+stepZ]-voxels[x+stepX][y-stepY][z+stepZ]) / -stepY,
                        (double)(voxels[x+stepX][y][z+2*stepZ]-voxels[x+stepX][y][z]) / -stepZ
                    },
					{x,y,z+stepZ, 
                        (double)(voxels[x+stepX][y][z+stepZ]-voxels[x-stepX][y][z+stepZ]) / -stepX,
                        (double)(voxels[x][y+stepY][z+stepZ]-voxels[x][y-stepY][z+stepZ]) / -stepY,
                        (double)(voxels[x][y][z+2*stepZ]-voxels[x][y][z]) / -stepZ
                    },
					{x,y+stepY,z, 
                        (double)(voxels[x+stepX][y+stepY][z]-voxels[x-stepX][y+stepY][z]) / -stepX,
                        (double)(voxels[x][y+2*stepY][z]-voxels[x][y][z]) / -stepY,
                        (double)(voxels[x][y+stepY][z+stepZ]-voxels[x][y+stepY][z-stepZ]) / -stepZ
                    },
					{x+stepX,y+stepY,z, 
                        (double)(voxels[x+2*stepX][y+stepY][z]-voxels[x+stepX][y+stepY][z]) / -stepX,
                        (double)(voxels[x+stepX][y+2*stepY][z]-voxels[x+stepX][y][z]) / -stepY,
                        (double)(voxels[x+stepX][y+stepY][z+stepZ]-voxels[x+stepX][y+stepY][z-stepZ]) / -stepZ
                    },
					{x+stepX,y+stepY,z+stepZ, 
                        (double)(voxels[x+2*stepX][y+stepY][z+stepZ]-voxels[x][y+stepY][z+stepZ]) / -stepX,
                        (double)(voxels[x+stepX][y+2*stepY][z+stepZ]-voxels[x+stepX][y][z+stepZ]) / -stepY,
                        (double)(voxels[x+stepX][y+stepY][z+2*stepZ]-voxels[x+stepX][y+stepY][z]) / -stepZ
                    },
					{x,y+stepY,z+stepZ, 
                        (double)(voxels[x+stepX][y+stepY][z+stepZ]-voxels[x-stepX][y+stepY][z+stepZ]) / -stepX,
                        (double)(voxels[x][y+2*stepY][z+stepZ]-voxels[x][y][z+stepZ]) / -stepY,
                        (double)(voxels[x][y+stepY][z+2*stepZ]-voxels[x][y+stepY][z]) / -stepZ
                    }
				},{
					voxels[x][y][z],
					voxels[x+stepX][y][z],
					voxels[x+stepX][y][z+stepZ],
					voxels[x][y][z+stepZ],
					voxels[x][y+stepY][z],
					voxels[x+stepX][y+stepY][z],
					voxels[x+stepX][y+stepY][z+stepZ],
					voxels[x][y+stepY][z+stepZ]
				}};
				processCube(c, isovalue);
			}
		}
	}

	return vertexList;
}
