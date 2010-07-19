#include <math.h>
#include "guiStructuredMesh.h"
#include "guiPrimitive.h"
#include "guiTypedefs.h"
#include "guiProblem.h"

extern Problem *problem;




StructuredMesh::StructuredMesh (void)
{
   primitives = new Primitive[MAX_PRIMITIVES];
   nPrimitives = 0;
   nBlocks = 0;

   primitiveUnderMouse = -1;

   minXused = minYused = -10.0;  // A large number
   maxXused = maxYused =  10.0;  // A small number

   isMeshGenerated = FALSE;

   blocks = NULL;
}




StructuredMesh::~StructuredMesh (void)
{
   delete[] primitives;
   delete[] blocks;
}




void StructuredMesh::addLine(int i, float* &coor)
{
   primitives[i].setType(LINE);

   primitives[i].setNumberOfEnteredPoints(2);
   primitives[i].allocateEnteredPointCoor();
   primitives[i].setEnteredPointCoor(coor);

   primitives[i].setNumberOfDefPoints(2);
   primitives[i].allocateDefPointCoor();
   primitives[i].setLineDefPointCoor();

   primitives[i].setIsDeleted(FALSE);

   updateMinMaxUsed();    // Update the min and max coordinates that will be shown on the screen.
}




void StructuredMesh::addArc(int i, float* &coor, bool center, bool cw)
{
   primitives[i].setType(ARC);
   primitives[i].setUsesCenter(center);
   primitives[i].setIsCW(cw);

   primitives[i].setNumberOfEnteredPoints(3);
   primitives[i].allocateEnteredPointCoor();
   primitives[i].setEnteredPointCoor(coor);

   primitives[i].setNumberOfDefPoints(CIRCULAR_ARC_DEF_POINTS);
   primitives[i].allocateDefPointCoor();
   primitives[i].calculateArcDefPointCoor();

   primitives[i].setIsDeleted(FALSE);

   updateMinMaxUsed();    // Update the min and max coordinates that will be shown on the screen.
}




void StructuredMesh::addFunction(int i, string f, float* &coor)
{
   primitives[i].setType(FUNCTION);
   primitives[i].setFunction(f);

   primitives[i].setNumberOfEnteredPoints(2);
   primitives[i].allocateEnteredPointCoor();
   primitives[i].setEnteredPointCoor(coor);

   primitives[i].setNumberOfDefPoints(FUNCTION_DEF_POINTS);
   primitives[i].allocateDefPointCoor();
   primitives[i].calculateFunctionDefPointCoor();

   primitives[i].setIsDeleted(FALSE);

   updateMinMaxUsed();    // Update the min and max coordinates that will be shown on the screen.
}




int StructuredMesh::getFirstEmptyPrimitivePos()
{
   for (int i = 0; i < MAX_PRIMITIVES; i++) {
      if (primitives[i].getIsDeleted()) return i;
   }
   return MAX_PRIMITIVES;    // Means we need to increase the length of the primitives array.
}





void StructuredMesh::updateMinMaxUsed()
{
   float *dummyCoor;
   minXused = 100000.0;   // A large number
   minYused = 100000.0;
   maxXused = -100000.0;  // A small number
   maxYused = -100000.0;

   // If there is no primitive drawn, than the following will be skipped and the above values will be used.

   for (int i = 0; i < MAX_PRIMITIVES; i++) {
      if (primitives[i].getIsDeleted()) continue;
      dummyCoor = primitives[i].getDefPointCoor();    // Cuneyt: Is there a problem here ?
      for (int j = 0; j < primitives[i].getNumberOfDefPoints(); j++) {
         if (dummyCoor[2*j] > maxXused)  maxXused = dummyCoor[2*j];
         if (dummyCoor[2*j] < minXused)  minXused = dummyCoor[2*j];
         if (dummyCoor[2*j+1] > maxYused)  maxYused = dummyCoor[2*j+1];
         if (dummyCoor[2*j+1] < minYused)  minYused = dummyCoor[2*j+1];
      }
   }
}




int StructuredMesh::isLineValid(float* &coor)
{
   float *dummyCoor;
   float x1, y1, x2, y2;
   float xx1, yy1, xx2, yy2;

   x1 = coor[0];   // These are shortcuts
   y1 = coor[1];
   x2 = coor[2];
   y2 = coor[3];
   
   if ((x1 == x2) && (y1 == y2))
      return -1;   // The LINE is not valid, because it has zero length.

   for (int i = 0; i < MAX_PRIMITIVES; i++) {   // Check if a line the same as this one already exists or not.
      if (primitives[i].getIsDeleted()) continue;
      if (primitives[i].getType() != LINE) continue;

      dummyCoor = primitives[i].getEnteredPointCoor();  // Cuneyt: Is there a problem here ?

      xx1 = dummyCoor[0];
      xx2 = dummyCoor[2];
      yy1 = dummyCoor[1];
      yy2 = dummyCoor[3];

      if ((x1 == xx1 && y1 == yy1 && x2 == xx2 && y2 == yy2) ||
      (x1 == xx2 && y1 == yy2 && x2 == xx1 && y2 == yy1))
         return -2;  // The LINE is not valid, because the same LINE already exists.
   }
   return 1;  // The LINE is valid

}  // end of function isLineValid()




int StructuredMesh::isArcValid(float* &coor, bool center, bool cw)
{
   float *dummyCoor;
   float x1, x2, x3, y1, y2, y3;
   float xx1, xx2, xx3, yy1, yy2, yy3;

   x1 = coor[0];  // These are shortcuts;
   y1 = coor[1];
   x2 = coor[2];
   y2 = coor[3];
   x3 = coor[4];
   y3 = coor[5];

   if (center) {
      float r1, r2;
      r1 = (x1-x3)*(x1-x3) + (y1-y3)*(y1-y3);  // Square of the distance between the 1st end point and the center.
      r2 = (x2-x3)*(x2-x3) + (y2-y3)*(y2-y3);  // Square of the distance between the 2nd end point and the center.
      if (fabs(r1-r2)/(r1+r2) > 0.01)  // Not a proper arc.
         return -1;
   } else {
      // cuneyt: Bu kismin dolmasi gerek.
   }
   
   for (int i = 0; i < MAX_PRIMITIVES; i++) {   // Check if an arc the same as this one already exists or not.
      if (primitives[i].getIsDeleted()) continue;
      if (primitives[i].getType() != ARC) continue;

      dummyCoor = primitives[i].getEnteredPointCoor();  // Cuneyt: Is there a problem here ?

      xx1 = dummyCoor[0];   // These are shortcuts
      yy1 = dummyCoor[1];
      xx2 = dummyCoor[2];
      yy2 = dummyCoor[3];
      xx3 = dummyCoor[4];
      yy3 = dummyCoor[5];

      if (center && primitives[i].getUsesCenter()) {   // Both the arc we are currently drawing and arc "i" are defined with 2 points and center.
         if (((x1 == xx1 && y1 == yy1 && x2 == xx2 && y2 == yy2 && x3 == xx3 && y3 == yy3) ||
         (x1 == xx2 && y1 == yy2 && x2 == xx1 && y2 == yy1 && x3 == xx3 && y3 == yy3)) &&
         (cw == primitives[i].getIsCW()))
            return -2;   // The ARC is not valid, because the same ARC already exists.
      } else if (!center && !primitives[i].getUsesCenter() ) { // Both the arc we are currently drawing and arc "i" are defined with 3 points.

         // Cuneyt: Fill this part

      }
   }

   return 1;  // The ARC is valid.

} // End of function  isArcValid()




int StructuredMesh::isFunctionValid(string function, float* &coor)
{
   float *dummyCoor;
   float x1, y1, x2, y2;
   float xx1, yy1, xx2, yy2;

   FunctionParser fparser;   // Function parser object
   double funcVals[1];

   x1 = coor[0];   // These are shortcuts
   y1 = coor[1];
   x2 = coor[2];
   y2 = coor[3];
   
   if ((x1 == x2) && (y1 == y2))
      return -1;   // The FUNCTION is not valid, because it has zero length.

   // Check if entered end points lie on the function or not?
   funcVals[0] = coor[0];
   fparser.Parse(function, "x");
   if (fabs(fparser.Eval(funcVals) - coor[1]) > 1e-5) return -2;

   funcVals[0] = coor[2];
   fparser.Parse(function, "x");
   if (fabs(fparser.Eval(funcVals) - coor[3]) > 1e-5) return -3;

   // Check if a FUNCTION the same as this one already exists or not.
   for (int i = 0; i < MAX_PRIMITIVES; i++) {
      if (primitives[i].getIsDeleted()) continue;
      if (primitives[i].getType() != FUNCTION) continue;

      dummyCoor = primitives[i].getEnteredPointCoor();

      xx1 = dummyCoor[0];
      xx2 = dummyCoor[2];
      yy1 = dummyCoor[1];
      yy2 = dummyCoor[3];

      if ((x1 == xx1 && y1 == yy1 && x2 == xx2 && y2 == yy2) ||
      (x1 == xx2 && y1 == yy2 && x2 == xx1 && y2 == yy1))
         return -4;  // The FUNCTION is not valid, because the same FUNCTION already exists.
   }
   return 1;  // The FUNCTION is valid

}  // End of function isFunctionVali()




bool StructuredMesh::isGeometryClosed()
{
   int i, j, jj, k, usedByHowManyPrimitives;
   int nEndPoints = 0, nEndPointMax;
   float *endPointCoor;  // x and y coordinates of the end points of a primitive.
   float *dummyCoor;  // Definition point coordinates of a primitive.
   bool newPoint;
   
   if (nPrimitives == 0) return FALSE;
   
   nEndPointMax = 2 * nPrimitives;

   endPointCoor = new float[2*nEndPointMax];
   
   // First generate the array of end points
   for (i = 0; i < MAX_PRIMITIVES; i++) {
      if (primitives[i].getIsDeleted()) continue;

      dummyCoor = primitives[i].getDefPointCoor();  // Cuneyt: Is there a problem here ?
      
      for(j = 0; j < 2; j++) { // Loop for the two ends of the primitive
         newPoint = TRUE;
         if (j == 0)  jj = 0;  // First point of the primitive
         if (j == 1)  jj = primitives[i].getNumberOfDefPoints() - 1;  // Last point of the primitive.
         for (k = 0; k < nEndPoints; k++) {
            if ( (dummyCoor[2*jj] == endPointCoor[2*k]) && (dummyCoor[2*jj+1] == endPointCoor[2*k+1]) ) {
               newPoint = FALSE;
               break;
            }
         }
         if (newPoint) {
            endPointCoor[2*nEndPoints] = dummyCoor[2*jj];
            endPointCoor[2*nEndPoints+1] = dummyCoor[2*jj+1];
            nEndPoints += 1;
         }
      }
   }
   
   // Than go through all these end points and check whether they are used only once or more than once.
   for (k = 0; k < nEndPoints; k++) {
      usedByHowManyPrimitives = 0;
      for (i = 0; i < MAX_PRIMITIVES; i++) {
         if (primitives[i].getIsDeleted()) continue;

         dummyCoor = primitives[i].getDefPointCoor();  // Cuneyt: Is there a problem here ?
         
         for(j = 0; j < 2; j++) {  // Loop for the two ends of the primitive
            newPoint = TRUE;
            if (j == 0)  jj = 0;  // First point of the primitive
            if (j == 1)  jj = primitives[i].getNumberOfDefPoints() - 1;  // Last point of the primitive.
            
            if ( (dummyCoor[2*jj] == endPointCoor[2*k]) && (dummyCoor[2*jj+1] == endPointCoor[2*k+1]) )
               usedByHowManyPrimitives += 1;
         }
      }
      if (usedByHowManyPrimitives < 2) {   // Indication of an open geometry
         if (endPointCoor != NULL)
         {
            delete[] endPointCoor;
            endPointCoor = NULL;
         }
         return FALSE;
      }
   }

   if (endPointCoor != NULL)
   {
      delete[] endPointCoor;
      endPointCoor = NULL;
   }
   
   return TRUE;

}  // End of function isGeometryClosed()




void StructuredMesh::calculateNumberOfBlocks()
{
   // Calculates the number of blocks used by the mesh.

   int i, maxUsedBlockNumber = 0;
   int* attachedBlock;

   for (i = 0; i < MAX_PRIMITIVES; i++) {
      if (primitives[i].getIsDeleted())  continue;

      attachedBlock = primitives[i].getAttachedBlock();

      if (attachedBlock[0] > maxUsedBlockNumber)
         maxUsedBlockNumber = attachedBlock[0];
      
      if (attachedBlock[1] > maxUsedBlockNumber)
         maxUsedBlockNumber = attachedBlock[1];
   }
   nBlocks = maxUsedBlockNumber + 1;
}  // End of function calculateNumberOfBlocks()




void StructuredMesh::calculateNumberOfPrimitivesOnFaces()
{
   int i, p, block;
   char face;
   int* attachedBlock;
   char* attachedFace;

   for (p = 0; p < MAX_PRIMITIVES; p++) {
      if (primitives[p].getIsDeleted())  continue;

      attachedBlock = primitives[p].getAttachedBlock();
      attachedFace = primitives[p].getAttachedFace();
      
      for (i = 0; i < 2; i++) {   // 2 is because a face can belong to 2 faces, at max.
         
         block = attachedBlock[i];
         face = attachedFace[i];
         
         if (face == 'S')
            blocks[block].faces[0].setnPrimitives( blocks[block].faces[0].getnPrimitives() + 1 );
         if (face == 'E')
            blocks[block].faces[1].setnPrimitives( blocks[block].faces[1].getnPrimitives() + 1 );
         if (face == 'N')
            blocks[block].faces[2].setnPrimitives( blocks[block].faces[2].getnPrimitives() + 1 );
         if (face == 'W')
            blocks[block].faces[3].setnPrimitives( blocks[block].faces[3].getnPrimitives() + 1 );
         
         if (! primitives[p].getIsThereSecondBlock() ) break;
      }
   }
}  // End of function  calculateNumberOfPrimitivesOnFaces()




void StructuredMesh::allocateFaceData()   // Cuneyt: This function can be moved under class Block.
{
   for(int i=0; i<nBlocks; i++) {
      blocks[i].faces[0].allocateData();
      blocks[i].faces[1].allocateData();
      blocks[i].faces[2].allocateData();
      blocks[i].faces[3].allocateData();
   }
}




void StructuredMesh::setFacePrimitives()
{
   int i, j, p, block;
   char face;
   int* attachedBlock;
   char* attachedFace;
   int **counter;  // Counts the number of primitives added to a face upto a certain instant.

   counter = new int*[nBlocks];
   for (i=0; i<nBlocks; i++)
      counter[i] = new int[4];   // Each block has 4 faces.

   for (i=0; i<nBlocks; i++)
      for (j=0; j<4; j++)
         counter[i][j] = 0;

   for (p = 0; p < MAX_PRIMITIVES; p++) {
      if (primitives[p].getIsDeleted())  continue;

      attachedBlock = primitives[p].getAttachedBlock();
      attachedFace = primitives[p].getAttachedFace();
      
      for (i = 0; i < 2; i++) {   // 2 is because a face can belong to 2 faces, at max.
         
         block = attachedBlock[i];
         face = attachedFace[i];
         
         if (face == 'S') {
            blocks[block].faces[0].appendPrimitive(counter[block][0], p);
            counter[block][0] = counter[block][0] + 1;
         }
         if (face == 'E') {
            blocks[block].faces[1].appendPrimitive(counter[block][1], p);
            counter[block][1] = counter[block][1] + 1;
         }
         if (face == 'N') {
            blocks[block].faces[2].appendPrimitive(counter[block][2], p);
            counter[block][2] = counter[block][2] + 1;
         }
         if (face == 'W') {
            blocks[block].faces[3].appendPrimitive(counter[block][3], p);
            counter[block][3] = counter[block][3] + 1;
         }
         
         if (! primitives[p].getIsThereSecondBlock() ) break;
      }
   }

}  // End of function setFacePrimitives()




void StructuredMesh::calculateNumberOfBoundaryPointsOnFaces()
{
   int b, i, j, p, counter;
   int *prims;

   for (b = 0; b < nBlocks; b++) {
      for (i = 0; i < 4; i++) {
         prims = blocks[b].faces[i].getPrimitives();
         counter = 0;
         for (j = 0; j < blocks[b].faces[i].getnPrimitives(); j++) {
            p = prims[j];
            counter += primitives[p].getNumberOfPoints();
         }
         counter -= blocks[b].faces[i].getnPrimitives() - 1;  // e.g. for a 3 primitive face, 2 points are counted twice.
         blocks[b].faces[i].setnBoundaryPoints(counter);
      }
   }
}




bool StructuredMesh::isFacePrimitiveOrderingCorrect(void)
{
   // Cuneyt: This function is very complicated. Even I can not follow it anymore.
   //         Not just a control but things like setOrderedList are alos done. but
   //         they need to be transfered to other functions.

  int b, i, ii, j, jj, k, kk, p, r, rr;
  int p1, p2, first, nPrimitivesOfThisBlock;
  bool *isPrimitiveUsedInThisBlock;
  int *primitivesOfThisBlock;
  float x1, y1, x2, y2;
  bool error = FALSE;
  int *prims, *dummyList;
  bool *dummyList2;
  float *dummyCoor;
  int *orderedPrimitives;      // An ordered list of primitives starting from the first primitive of the South face, than East, North and finally West faces
  bool *needReversing;         // Some primitives may need reversing to form a closed loop for a block.
  int nBlocks = getnBlocks();  // shortcut for number of total blocks

  // First check if there are any faces with zero primitives.
  for (i = 0; i < nBlocks; i++) {
    for (j = 0; j < 4; j++) {
     if (blocks[i].faces[j].getnPrimitives() == 0) {
        return FALSE;   // Cuneyt: A message such as "Block i, face j is not defined" can be provided.
      }
    }
  }

  for (b = 0; b < nBlocks; b++) {
    isPrimitiveUsedInThisBlock = new bool[MAX_PRIMITIVES];

    // Initialization
    for (i = 0; i < MAX_PRIMITIVES; i++)
      isPrimitiveUsedInThisBlock[i] = FALSE;

    // Find the primitives used in this block
    for (i = 0; i < 4; i++) {
     for (j = 0; j < blocks[b].faces[i].getnPrimitives(); j++) {
      prims = blocks[b].faces[i].getPrimitives();
      k = prims[j];
        isPrimitiveUsedInThisBlock[k] = TRUE;
      }
    }

    // Find the number of primitives used in this block
    nPrimitivesOfThisBlock = 0;
    for (i = 0; i < MAX_PRIMITIVES; i++)
      if (isPrimitiveUsedInThisBlock[i])   nPrimitivesOfThisBlock += 1;

    primitivesOfThisBlock = new int[nPrimitivesOfThisBlock];

    j = 0;
    for (i = 0; i < MAX_PRIMITIVES; i++)
      if ( isPrimitiveUsedInThisBlock[i] ) {
        primitivesOfThisBlock[j] = i;
        j++;
      }

    isPrimitiveUsedInThisBlock = NULL;
    delete [] isPrimitiveUsedInThisBlock;



    orderedPrimitives = new int[nPrimitivesOfThisBlock];
    needReversing = new bool[nPrimitivesOfThisBlock];

    // Initializations
    for (i = 0; i < nPrimitivesOfThisBlock; i++) {
      orderedPrimitives[i] = -1;
      needReversing[i] = FALSE;
    }

    // Find the first primitive of the south face of this block
    error = TRUE;

   for (j = 0; j < blocks[b].faces[3].getnPrimitives(); j++) {   // Primitives of the West face
      prims = blocks[b].faces[3].getPrimitives();
      p1 = prims[j];

      for(k = 0; k < 2; k++) {  // Loop for the two ends of the primitive
        if (k == 0)  kk = 0;  // First point of the primitive.
      if (k == 1)  kk = primitives[p1].getNumberOfDefPoints() - 1;  // Last point of the primitive.

      dummyCoor = primitives[p1].getDefPointCoor();
      x1 = dummyCoor[2*kk];
        y1 = dummyCoor[2*kk+1];

      for (p = 0; p < blocks[b].faces[0].getnPrimitives(); p++) {    // Primitives of the South face
        prims = blocks[b].faces[0].getPrimitives();
          p2 = prims[p];

          for(r = 0; r < 2; r++) {  // Loop for the two ends of the primitive
            if (r == 0)  rr = 0;  // First point of the primitive.
            if (r == 1)  rr = primitives[p2].getNumberOfDefPoints() - 1;  // Last point of the primitive.

         dummyCoor = primitives[p2].getDefPointCoor();
            x2 = dummyCoor[2*rr];
            y2 = dummyCoor[2*rr+1];

            if (x1 == x2 && y1 == y2) {   // Primitive p is the first primitive of the South face.
              error = FALSE;
              orderedPrimitives[0] = p2;
              if (r == 1)  needReversing[0] = TRUE;
              goto ONE;
            }
          }
        }
      }
    }


    ONE:  if (error) goto LAST;  // error = TRUE means that West and South faces are not intersecting.


    // Now starting from the first primitive, fill the orderedPrimitives[] and needReversing[] arrays.
    for (i = 0; i < nPrimitivesOfThisBlock - 1; i++) {
      p1 = orderedPrimitives[i];
      error = TRUE;

      // x1, y1 is the end point of p1.
     dummyCoor = primitives[p1].getDefPointCoor();
      if (needReversing[i]) {
        x1 = dummyCoor[0];
        y1 = dummyCoor[1];
      } else {
      x1 = dummyCoor[2*(primitives[p1].getNumberOfDefPoints()-1)];
        y1 = dummyCoor[2*(primitives[p1].getNumberOfDefPoints()-1)+1];
      }

      // Now find another primitive with a starting or end point same as x1, y1
      for (jj = 0; jj < nPrimitivesOfThisBlock; jj++) {
        j = primitivesOfThisBlock[jj];
        if (j == p1 )  continue;

        for(k = 0; k < 2; k++) {  // Loop for the two ends of primitive j
          if (k == 0)  kk = 0;  // First point of the primitive.
        if (k == 1)  kk = primitives[j].getNumberOfDefPoints() - 1;  // Last point of the primitive.

        dummyCoor = primitives[j].getDefPointCoor();
          x2 = dummyCoor[2*kk];
          y2 = dummyCoor[2*kk+1];

          if (x1 == x2 && y1 == y2) {   // Primitives p1 and j have common points.
            error = FALSE;
            orderedPrimitives[i+1] = j;
            if (k == 1)  needReversing[i+1] = TRUE;
            goto TWO;
          }
        }
      }
      TWO: if (error) goto LAST;
    }

    // Check if all the primitives of this block appear in the orderedList only once.
    for (i = 0; i < nPrimitivesOfThisBlock; i++) {
      j = primitivesOfThisBlock[i];
      error = TRUE;

      for (k = 0; k < nPrimitivesOfThisBlock; k++) {
        if (j == orderedPrimitives[k]) {
          error = FALSE;
          break;
        }
      }
      if (error) goto LAST;
    }


    // We can now copy the orderedPrimitives and needReversing arrays to the corresponding arrays of the faces.
    for (j = 0; j < 4; j++) {
      first = 0;  // Ordered list number of the first primitive of this face.

      for ( k = 0; k < j; k++)
        first += blocks[b].faces[k].getnPrimitives();

      for (k = 0; k < blocks[b].faces[j].getnPrimitives(); k++) {
        kk = first + k;
      blocks[b].faces[j].setOrderedList(k, orderedPrimitives[kk]);
      blocks[b].faces[j].setNeedReversing(k, needReversing[kk]);
      }
    }

   orderedPrimitives = NULL;
    needReversing = NULL;
    delete [] orderedPrimitives;
    delete [] needReversing;
  }

  // Check if the faces meet at the corners or not?
  error = FALSE;
  for (b = 0; b < nBlocks; b++) {
    for (i = 0; i < 4; i++) {

      // Last primitive of this face
      dummyList  = blocks[b].faces[i].getOrderedList();
     dummyList2 = blocks[b].faces[i].getNeedReversing();
     k = dummyList[ blocks[b].faces[i].getnPrimitives() - 1 ];
     dummyCoor = primitives[k].getDefPointCoor();

     if (dummyList2[ blocks[b].faces[i].getnPrimitives() - 1 ]) {
        x1 = dummyCoor[0];
        y1 = dummyCoor[1];
      } else {
      x1 = dummyCoor[2*(primitives[k].getNumberOfDefPoints()-1)];
        y1 = dummyCoor[2*(primitives[k].getNumberOfDefPoints()-1)+1];
      }

      // First primitive of the next face
      if (i == 3) {
        ii = 0;  // Next face number
      } else {
        ii = i+1;
      }
      dummyList  = blocks[b].faces[ii].getOrderedList();
     dummyList2 = blocks[b].faces[ii].getNeedReversing();
     kk = dummyList[0];   // Next primitive number
     dummyCoor = primitives[kk].getDefPointCoor();

      if (dummyList2[0]) {
      x2 = dummyCoor[2*(primitives[kk].getNumberOfDefPoints()-1)];
        y2 = dummyCoor[2*(primitives[kk].getNumberOfDefPoints()-1)+1];
      } else {
        x2 = dummyCoor[0];
        y2 = dummyCoor[1];
      }

      if (x1 != x2 || y1 != y2) {   // Problem at the corner
        error = TRUE;
        goto LAST;
      }
    }
  }


  LAST: orderedPrimitives = NULL;
  needReversing = NULL;
  primitivesOfThisBlock = NULL;
  delete [] orderedPrimitives;
  delete [] needReversing;
  delete [] primitivesOfThisBlock;

//  QString s;

  if (error)
    return FALSE;
  else
    return TRUE;
}  // End of function isFacePrimitiveOrderingCorrect()




bool StructuredMesh::doSW_NEpointsMatch()
{
   for (int b = 0; b < nBlocks; b++) {
      if (blocks[b].faces[0].getnBoundaryPoints() != blocks[b].faces[2].getnBoundaryPoints() ||
         blocks[b].faces[1].getnBoundaryPoints() != blocks[b].faces[3].getnBoundaryPoints())
       return FALSE;
  }  

  return TRUE;
}




void StructuredMesh::generateLinearTFIMesh()
{
   int b, i, j, k, n, m, p;
   int sMax, tMax;
   float s, t;
   float (*south)[2], (*east)[2], (*north)[2], (*west)[2];  // Face coordinates 
   float u, v, uv, (*dummy)[2];
   int *dummyList;
   bool *dummyList2;
   float *dummyCoor;

   /*
   Linear TFI works like this (page 3-6 of Handbook of Grid Generation)

   U_(s,t)   = (1-s) * X_(0,t) + s * X_(1,t)
   V_(s,t)   = (1-t) * X_(s,0) + t * X_(s,1)
   U_V_(s,t) = (1-s)(1-t) * X_(0,0) + (1-s)t * X_(0,1) + s(1-t) * X_(1,0) + st * X_(1,1)
   X_(s,t)   = U_(s,t) + V_(s,t) - U_V_(s,t)

   Here
   X_(0,t) is the coordinates of the West face
   X_(1,t) is the coordinates of the East face
   X_(s,0) is the coordinates of the South face
   X_(s,1) is the coordinates of the North face

   X_(0,0) is the coordinates of the West-South corner
   X_(1,0) is the coordinates of the East-South corner
   X_(0,1) is the coordinates of the West-North corner
   X_(1,1) is the coordinates of the East-North corner
   */

   for (b = 0; b < nBlocks; b++) {

      // Allocate space for coordinates of the points of block b.
      // Note: points are stored in a 1D row-first array.
      sMax = blocks[b].faces[0].getnBoundaryPoints();  // Number of points in the s (ksi) direction
      tMax = blocks[b].faces[1].getnBoundaryPoints();  // Number of points in the t (eta) direction
      blocks[b].setnXpoints(sMax);
      blocks[b].setnYpoints(tMax);

      blocks[b].coordinates = NULL;
      delete[] blocks[b].coordinates;						// Cuneyt: Check this deallocaion
      blocks[b].coordinates = new float[2*sMax*tMax];

      // Allocate blocked cell array.
      // Cuneyt: This is not the correct place for this.
      blocks[b].isCellBlocked = new int[(sMax-1)*(tMax-1)];
      for(i=0; i<(blocks[b].getnXpoints()-1)*(blocks[b].getnYpoints()-1); i++)
         blocks[b].isCellBlocked[i] = 0;	// Mark all the cells as not blocked.

      // Allocate and setup face coordinates
      south = new float[sMax][2];   // Cuneyt: change the size of these
      north = new float[sMax][2];
      east  = new float[tMax][2];
      west  = new float[tMax][2];

       for (i = 0; i < 4; i++) {
         k = 0;
         for (j = 0; j < blocks[b].faces[i].getnPrimitives(); j++) {
            dummyList  = blocks[b].faces[i].getOrderedList();
            dummyList2 = blocks[b].faces[i].getNeedReversing();
     
            p = dummyList[j];
            dummyCoor = problem->mesh->primitives[p].getPointCoor();

            if (dummyList2[j]) {
               for (m = primitives[p].getNumberOfPoints() - 1; m >= 0; m--) {
                  if (j > 0 && m == primitives[p].getNumberOfPoints() - 1)  continue;   // Skip the first point of the second, third, etc primitive of a face.
                  for (n = 0; n < 2; n++) {
                     if (i == 0)
                        south[k][n] = dummyCoor[2*m+n];
                     else if (i == 1)
                        east[k][n]  = dummyCoor[2*m+n];
                     else if (i == 2)
                        north[k][n] = dummyCoor[2*m+n];
                     else if (i == 3)
                        west[k][n]  = dummyCoor[2*m+n];
                  }
                  k++;
               }
            } else {
               for (m = 0; m < primitives[p].getNumberOfPoints(); m++) {
                  if (j > 0 && m == 0)  continue;   // Skip the first point of the second, third, etc primitive of a face.
                     for (n = 0; n < 2; n++) {
                  if (i == 0)
                     south[k][n] = dummyCoor[2*m+n];
                  else if (i == 1)
                     east[k][n]  = dummyCoor[2*m+n];
                  else if (i == 2)
                     north[k][n] = dummyCoor[2*m+n];
                  else if (i == 3)
                     west[k][n]  = dummyCoor[2*m+n];
                     }
                     k++;
               }
            }
         }
      }

      // Reverse the north array.
      dummy = new float[sMax][2];

      for (i = 0; i < sMax; i++) {
         dummy[i][0] = north[i][0];
         dummy[i][1] = north[i][1];
      }

      for (i = 0; i < sMax; i++) {
         north[sMax - i - 1][0] = dummy[i][0];
         north[sMax - i - 1][1] = dummy[i][1];
      }

      // Reverse the west array.
      dummy = NULL;
      delete[] dummy;   // Cuneyt: check this deallocation
      dummy = new float[tMax][2];

      for (i = 0; i < tMax; i++) {
         dummy[i][0] = west[i][0];
         dummy[i][1] = west[i][1];
      }

      for (i = 0; i < tMax; i++) {
         west[tMax - i - 1][0] = dummy[i][0];
         west[tMax - i - 1][1] = dummy[i][1];
      }

      dummy = NULL;
      delete[] dummy;

      for (i = 0; i < sMax; i++) {
         s = float(i) / float(sMax - 1);

         for (j = 0; j < tMax; j++) {
            t = float(j) / float(tMax - 1);
            k = i + j * sMax;  // Will be used in blocks[b].coordinates[k][2]

            for (n = 0; n < 2; n++) {   // Space dimensions
               u = (1-s) * west[j][n] + s * east[j][n];
               v = (1-t) * south[i][n] + t * north[i][n];
               uv= (1-s) * (1-t) * south[0][n] + (1-s) * t * north[0][n] + s * (1-t) * east[0][n] + s * t * north[sMax - 1][n];

               blocks[b].coordinates[2*k+n] = u + v - uv;
            }
         }
      }

      south = NULL; delete[] south;
      north = NULL; delete[] north;
      east  = NULL; delete[] east;
      west  = NULL; delete[] west;

   } // End of block loop

} // End of function generateLinearTFIMesh()




void StructuredMesh::generateLaplaceMesh()
{
   int b, i, j, ind, m, n, loop;
   float g11, g12, g22, xtemp, ytemp;

   // This version uses no control functions.
   // Gauss Seidel iterative solution is used.

   // First obtain an initial mesh with the linearTFIMesh
   generateLinearTFIMesh();

   for (b = 0; b < problem->mesh->getnBlocks(); b++) {
      m = blocks[b].getnXpoints();  // Number of points in the s (ksi) direction
      n = blocks[b].getnYpoints();  // Number of points in the t (eta) direction

      for (loop=1; loop<=10; loop++)
      {
         for (j=1; j<n-1; j++)
         {
            for (i=1; i<m-1; i++)
            {
               ind = i+ m*j;

               g11 = ((blocks[b].coordinates[2*(ind+1)] - blocks[b].coordinates[2*(ind-1)]) * (blocks[b].coordinates[2*(ind+1)] - blocks[b].coordinates[2*(ind-1)]) +
                     (blocks[b].coordinates[2*(ind+1)+1] - blocks[b].coordinates[2*(ind-1)+1]) * (blocks[b].coordinates[2*(ind+1)+1] - blocks[b].coordinates[2*(ind-1)+1]) ) / 4;
               
               g22 = ((blocks[b].coordinates[2*(ind+m)] - blocks[b].coordinates[2*(ind-m)]) * (blocks[b].coordinates[2*(ind+m)] - blocks[b].coordinates[2*(ind-m)]) +
                     (blocks[b].coordinates[2*(ind+m)+1] - blocks[b].coordinates[2*(ind-m)+1]) * (blocks[b].coordinates[2*(ind+m)+1] - blocks[b].coordinates[2*(ind-m)+1]) ) / 4;
               
               g12 = ((blocks[b].coordinates[2*(ind+1)] - blocks[b].coordinates[2*(ind-1)]) * (blocks[b].coordinates[2*(ind+m)] - blocks[b].coordinates[2*(ind-m)]) +
                     (blocks[b].coordinates[2*(ind+1)+1] - blocks[b].coordinates[2*(ind-1)+1]) * (blocks[b].coordinates[2*(ind+m)+1] - blocks[b].coordinates[2*(ind-m)+1]) ) / 4;

               xtemp = 1/(2*(g11+g22))*(
                      g22*blocks[b].coordinates[2*(ind+1)] - 0.5*g12*blocks[b].coordinates[2*(ind+1+m)] + 0.5*g12*blocks[b].coordinates[2*(ind+1-m)] + 
                     g11*blocks[b].coordinates[2*(ind+m)] + g11*blocks[b].coordinates[2*(ind-m)] + 
                     g22*blocks[b].coordinates[2*(ind-1)] - 0.5*g12*blocks[b].coordinates[2*(ind-1-m)] + 0.5*g12*blocks[b].coordinates[2*(ind-1+m)] );

               ytemp = 1/(2*(g11+g22))*(
                      g22*blocks[b].coordinates[2*(ind+1)+1] - 0.5*g12*blocks[b].coordinates[2*(ind+1+m)+1] + 0.5*g12*blocks[b].coordinates[2*(ind+1-m)+1] + 
                     g11*blocks[b].coordinates[2*(ind+m)+1] + g11*blocks[b].coordinates[2*(ind-m)+1] + 
                     g22*blocks[b].coordinates[2*(ind-1)+1] - 0.5*g12*blocks[b].coordinates[2*(ind-1-m)+1] + 0.5*g12*blocks[b].coordinates[2*(ind-1+m)+1] );
   
               blocks[b].coordinates[2*ind] = xtemp;
               blocks[b].coordinates[2*ind+1] = ytemp;
            }
         }
      }
   }

} // End of function generateLaplaceMesh()
