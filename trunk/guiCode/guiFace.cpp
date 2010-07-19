#include "guiFace.h"




Face::Face (void)
{
   nPrimitives = 0;
   nBoundaryPoints = 0;

   orderedList = NULL;     // Cuneyt: Are these NULLs necessary ?
   needReversing = NULL;
   primitives = NULL;
}




Face::~Face()
{
   delete[] orderedList;
   delete[] needReversing;
   delete[] primitives;
   orderedList = NULL;     // Cuneyt: Are these NULLs necessary ?
   needReversing = NULL;
   primitives = NULL;
}




void Face::allocateData()
{
   primitives = new int[nPrimitives];
   orderedList = new int[nPrimitives];
   needReversing = new bool[nPrimitives];
}
