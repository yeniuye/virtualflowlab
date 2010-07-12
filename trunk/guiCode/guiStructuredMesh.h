// This is the header file structuredMesh.h
// This is the interface for the class StructuredMesh.

#ifndef STRUCTUREDMESH_H
#define STRUCTUREDMESH_H

#include <string>
#include <vector>
#include <QColor>
#include "guiTypedefs.h"
#include "guiPrimitive.h"
#include "guiBlock.h"
#include "../fparserCode/fparser.h"

using namespace std;

class StructuredMesh {
public:
	StructuredMesh();
	~StructuredMesh();

	void setnPrimitives(int i)          {nPrimitives = i;}
	void setnBlocks(int i)              {nBlocks = i;}
	void setMinXused(double a)          {minXused = a;}
	void setMaxXused(double a)          {maxXused = a;}
	void setMinYused(double a)          {minYused = a;}
	void setMaxYused(double a)          {maxYused = a;}
	void setPrimitiveUnderMouse(int i)  {primitiveUnderMouse = i;}
	void setIsMeshGenerated(bool b)     {isMeshGenerated = b;}

	int getnPrimitives()          {return nPrimitives;}
	int getnBlocks()              {return nBlocks;}
	double getMinXused()          {return minXused;}
	double getMaxXused()          {return maxXused;}
	double getMinYused()          {return minYused;}
	double getMaxYused()          {return maxYused;}
	int getPrimitiveUnderMouse()  {return primitiveUnderMouse;}
	bool getIsMeshGenerated()     {return isMeshGenerated;}

	int getFirstEmptyPrimitivePos();
	void updateMinMaxUsed();

	int isLineValid(float* &coor);
	int isArcValid(float* &coor, bool center, bool cw);
	int isFunctionValid(string function, float* &coor);

	void addLine(int i, float* &coor);
	void addArc(int i, float* &coor, bool center, bool cw);
	void addFunction(int i, string f, float* &coor);

	bool isGeometryClosed();
	void calculateNumberOfBlocks();
	void allocateBlocks()    {blocks = new Block[nBlocks];}
	void calculateNumberOfPrimitivesOnFaces();
	void allocateFaceData();
	void setFacePrimitives();
	void calculateNumberOfBoundaryPointsOnFaces();
	bool isFacePrimitiveOrderingCorrect();
	bool doSW_NEpointsMatch();
	void generateLinearTFIMesh();
	void generateLaplaceMesh();

	Primitive *primitives;          // Dynamic array that will hold all the geometric primitives.
	Block *blocks;                  // Dynamic array that will hold all the blocks.
private:
	int nPrimitives;                // Number of geometric primitives
	int nBlocks;                    // Number of blocks used in the mesh
	int primitiveUnderMouse;        // The geometric primitive under the mouse. It is equal to -1 if there is no primitive under the mouse.
	double minXused, maxXused,      // These are the min and max coordinates of the primitives shown on the screen.
		   minYused, maxYused;
	bool isMeshGenerated;           // Default is FALSE
};

#endif
