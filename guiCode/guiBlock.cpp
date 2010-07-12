// This is the implementation file block.cpp

#include <vector> 
#include "guiBlock.h"
#include "guiFace.h"
#include "guiTypedefs.h"




Block::Block (void)
{
   nXpoints = 0;
   nYpoints = 0;
   nControlPoints = 0;
   nBlockedCells = 0;
   faces = new Face[4];
   controlPoints = new int[MAX_CONTROL_POINTS];
   coordinates = NULL;
   isCellBlocked = NULL;

   for(int i=0; i<MAX_CONTROL_POINTS; i++) {
      for(int j=0; j<3; j++) {
         controlPointData[i][j] = 0.0;
      }
   }
}




Block::~Block()
{
   delete[] faces;
   delete[] coordinates;
   delete[] controlPoints;
   delete[] isCellBlocked;
   faces = NULL;		       // Cuneyt: Are these necessary ?
   coordinates = NULL;
   controlPoints = NULL;
   isCellBlocked = NULL;
}




int Block::addRemoveControlPoint(int c)
{
   int i, returnState, prevCp = -1;
   bool isAlreadyControlPoint = FALSE;

   for(i = 0; i < nControlPoints; i++) {
      if (c == controlPoints[i]) {
         isAlreadyControlPoint = TRUE;
         prevCp = i;
         break;
      }
   }

   if (isAlreadyControlPoint) {
      // Point c is already a control point. Remove it from the control points list.
      for(i = prevCp; i < nControlPoints-1; i++) {
         controlPoints[i] = controlPoints[i+1];
      }
      nControlPoints = nControlPoints - 1;
      returnState = 1;
   } else if (nControlPoints >= MAX_CONTROL_POINTS) {
      returnState = 2;  // Means the control point list is full.
   } else {
      // Control points list is not full. New control points can be added.
      controlPoints[nControlPoints] = c;
      nControlPoints = nControlPoints + 1;
      returnState = 3;
   }

   return returnState;
}  // End of function addRemoveControlPoint()




void Block::addBlockedCell(int bc)
{
   if (isCellBlocked[bc] == 0) {
      isCellBlocked[bc] = 1;     // If the cell is not already blocked, mark it as blocked
      nBlockedCells = nBlockedCells + 1;
   }
}





void Block::removeBlockedCell(int bc)
{
   if (isCellBlocked[bc] == 1) {
      isCellBlocked[bc] = 0;  // If the cell is already blocked, mark it as not blocked
      nBlockedCells = nBlockedCells - 1;
   }
}





int Block::findCellAtXY(float x, float y, int &iCell, int &jCell, float** &cornerCoor)
{
   int i, j, cell, c1, c2, c3, c4;
   int nX, nY;
	
   nX = nXpoints;
   nY = nYpoints;

   cell = -1;
   for (j = 0; j < nY-1; j++ ) {
      for (i = 0; i < nX-1; i++ ) {
         c1 = j * nX + i;
         c2 = c1 + 1;
         c3 = c2 + nX;
         c4 = c1 + nX;
         cornerCoor[0][0] = coordinates[2*c1];
         cornerCoor[0][1] = coordinates[2*c1+1];
         cornerCoor[1][0] = coordinates[2*c2];
         cornerCoor[1][1] = coordinates[2*c2+1];
         cornerCoor[2][0] = coordinates[2*c3];
         cornerCoor[2][1] = coordinates[2*c3+1];
         cornerCoor[3][0] = coordinates[2*c4];
         cornerCoor[3][1] = coordinates[2*c4+1];

         if (x >= cornerCoor[0][0] && x <= cornerCoor[1][0] && y >= cornerCoor[0][1] && y<= cornerCoor[2][1]) { // cuneyt: Burada hucreler Kartezyen kabul edildi. Degil ise daha karisik hesaplar gerekli
            cell = j * (nX-1) + i;  // Coordinates (x,y) is in this cell
            iCell = i;
            jCell = j;
            break;
         }
      }
      if (cell != -1) break;
   }
   
   return cell;
} // End of function findCellAtXY()




int Block::isXYinNeighbors(float x, float y, int &iCell, int &jCell, float** &cornerCoor, int* neighborCells)
{
   int i, j, cell, c1, c2, c3, c4, n;
   int nX, nY;
	
   nX = nXpoints;
   nY = nYpoints;

   cell = -1;

   for(n=0; n<9; n++) {
      if (neighborCells[n] == -1) continue;

      i = neighborCells[n] % (nX-1);   // Remainder function
      j = neighborCells[n] / (nX-1);   // Integer division
      
      c1 = j * nX + i;
      c2 = c1 + 1;
      c3 = c2 + nX;
      c4 = c1 + nX;
      cornerCoor[0][0] = coordinates[2*c1];
      cornerCoor[0][1] = coordinates[2*c1+1];
      cornerCoor[1][0] = coordinates[2*c2];
      cornerCoor[1][1] = coordinates[2*c2+1];
      cornerCoor[2][0] = coordinates[2*c3];
      cornerCoor[2][1] = coordinates[2*c3+1];
      cornerCoor[3][0] = coordinates[2*c4];
      cornerCoor[3][1] = coordinates[2*c4+1];
      
      if (x >= cornerCoor[0][0] && x <= cornerCoor[1][0] && y >= cornerCoor[0][1] && y<= cornerCoor[2][1]) { // cuneyt: Burada hucreler Kartezyen kabul edildi. Degil ise daha karisik hesaplar gerekli
         cell = neighborCells[n];  // Coordinates (x,y) is in this cell
         iCell = i;
         jCell = j;
         break;
      }
      if (cell != -1) break;
   }
   
   return cell;
} // End of function isXYinNeighbors()
