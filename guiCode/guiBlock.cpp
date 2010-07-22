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
   faces = NULL;		       // Cuneyt: Are these NULLs necessary ?
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




void Block::addBlockedCell(int cell)
{
   if (isCellBlocked[cell] == 0) {
      isCellBlocked[cell] = 1;     // If the cell is not already blocked, mark it as blocked
      nBlockedCells = nBlockedCells + 1;
   }
}





void Block::removeBlockedCell(int cell)
{
   if (isCellBlocked[cell] == 1) {
      isCellBlocked[cell] = 0;  // If the cell is already blocked, mark it as not blocked
      nBlockedCells = nBlockedCells - 1;
   }
}





void Block::autoFillBlockedCell(int cell)
{
   // Find all the cells that are inside a region surrounded by blocked cells and mark them
   // as blocked cells.

   // Return immediately if the initially clicked cell is already a blocked cell
   if (isCellBlocked[cell] == 1) {
      return;
   }

   isCellBlocked[cell] = 1;
   nBlockedCells = nBlockedCells + 1;

   autoFillRecursive(cell);
}




void Block::autoFillRecursive(int cell)
{
   // Recursive function used to mark all the cells that needs to be blcoked during an auto
   // fill process.
   
   int nX = getnXpoints();
   int nY = getnYpoints();

   // Find neighbors of this cell.
   // Cuneyt: This neighbor finding process is also performed in streamfunction plotting, but
   // in a slightly different way. It will be good if this can be put inside a function.
   int neighbor[4];
   neighbor[0] = cell + 1;         // Right neighbor
   neighbor[1] = cell - 1;         // Left neighbor	
   neighbor[2] = cell + nX - 1;    // Top neighbor
   neighbor[3] = cell - (nX - 1);  // Bottom neighbor

   // Correct neighbors that should actually fall outside the problem domain
   if (cell < nX-1) {               // cell is already located at the bottom boundary
      neighbor[3] = -1;
   }
   if (cell > (nX-1)*(nY-1)-nX) {   // cell is already located at the top boundary
      neighbor[2] = -1;
   }
   if (cell % (nX-1) == 0) {        // cell is already located at the left boundary
      neighbor[1] = -1;
   }
   if ((cell+1) % (nX-1) == 0) {    // cell is already located at the right boundary
      neighbor[0] = -1;
   }

   for (int i=0; i<4; i++) {
      if (neighbor[i] != -1 && isCellBlocked[neighbor[i]] == 0) {
         isCellBlocked[neighbor[i]] = 1;
         nBlockedCells = nBlockedCells + 1;
         autoFillRecursive(neighbor[i]);
      }
   }
}  // End of function autoFillRecursive()




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

         // Cuneyt: Here all the cells are assumed to be Cartesian. More complicated calculations are necessary if this is not the case.
         if (x >= cornerCoor[0][0] && x <= cornerCoor[1][0] && y >= cornerCoor[0][1] && y<= cornerCoor[2][1]) {
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
      
      // Cuneyt: Here all the cells are assumed to be Cartesian. More complicated calculations are necessary if this is not the case.
      if (x >= cornerCoor[0][0] && x <= cornerCoor[1][0] && y >= cornerCoor[0][1] && y<= cornerCoor[2][1]) {
         cell = neighborCells[n];  // Coordinates (x,y) is in this cell
         iCell = i;
         jCell = j;
         break;
      }
      if (cell != -1) break;
   }
   
   return cell;
} // End of function isXYinNeighbors()
