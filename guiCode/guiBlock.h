#ifndef BLOCK_H
#define BLOCK_H

#include <QColor>
#include "guiTypedefs.h"
#include "guiFace.h"

using namespace std;




class Block
{
public:
   Block();
   ~Block();

   void setnXpoints(int i)        {nXpoints = i;}
   void setnYpoints(int i)        {nYpoints = i;}
   void setnControlPoints(int i)  {nControlPoints = i;}
   void setnBlockedCells(int i)   {nBlockedCells = i;}
   int getnXpoints()              {return nXpoints;}
   int getnYpoints()              {return nYpoints;}
   int getnControlPoints()        {return nControlPoints;}
   int getnBlockedCells()         {return nBlockedCells;}
   int addRemoveControlPoint(int c);
   void addBlockedCell(int bc);
   void removeBlockedCell(int bc);
   int findCellAtXY(float x, float y, int &iCell, int &jCell, float** &cornerCoor);
   int isXYinNeighbors(float x, float y, int &iCell, int &jCell, float** &cornerCoor, int* neighborCells);

   Face *faces;          // Each domain has 4 faces (the order is South, East, North, West)
   float *coordinates;   // x and y coordinates of all points of this block. Stored as a one dimensional array (x0, y0, x1, y1, x2, y2, ...)
                         // Points are ordered first in x-direction and than in y-direction.
                         // Allocated while generating the mesh.
                         // These coordinates are needed in mesh generation and making them private makes the process difficult.
   int *isCellBlocked;   // An array of blocked cells, stored as an integer made up of the i and j index of blocked cells.
   int *controlPoints;   // An array of control points, stored as an integer made up of the i and j index of control points.

   double controlPointData[MAX_CONTROL_POINTS][3];	// Used for sending the control point data from the solver to the GUI.  // cuneyt: Multi-blok destegi yok.

private:
   int nXpoints, nYpoints;	// Number of points in the x and y directions, respectively.
   int nBlockedCells;      // Number of blocked cells
   int nControlPoints;     // Number of control points (can not be larger than MAX_CONTROL_POINTS)
};

#endif
