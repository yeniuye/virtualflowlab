#ifndef FACE_H
#define FACE_H

#include <vector>
#include "guiTypedefs.h"

using namespace std;

class Face
{
public:
	Face();
	~Face();

	void setnPrimitives(int i)              {nPrimitives = i;}
	void setnBoundaryPoints(int i)          {nBoundaryPoints = i;}
	void appendPrimitive(int i, int p)      {primitives[i] = p;}
	void setOrderedList(int i, int ii)      {orderedList[i] = ii;}
	void setNeedReversing(int i, bool b)    {needReversing[i] = b;}

	int* getPrimitives()       {return primitives;}
	int* getOrderedList()      {return orderedList;}
	bool* getNeedReversing()   {return needReversing;}
	int getnPrimitives()       {return nPrimitives;}
	int getnBoundaryPoints()   {return nBoundaryPoints;}

	void allocateData();

	// calculateBoundaryPoint()  cuneyt: Boyle bisey lazim.
private:
	int nPrimitives;        // Number of geometric primitives of a face
	int nBoundaryPoints;    // Number of boundary points of a face.
	int *primitives;        // A dynamic array of geometric primitives used on a face.
	int *orderedList;       // Ordered list of the primitives on a face so that they can be added end-to-end to form a continuous face.
	bool *needReversing;    // Some primitives of the face may need reversing so that they can be added end-to-end to form a continuous face.
};

#endif
