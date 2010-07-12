/*****************************************************************************/
/*          2D DYNAMIC MEMORY ALLOCATION and DEALLOCATION FUNCTION           */
/*****************************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* MemoryAllocater2D takes two parameters the row and column size            */
/* (RowSize & ColumnSize) of the matrix to be allocated in the memory.       */
/* And check each time for available memory.                                 */
/* The function returns (**double) adress of the allocated matrix            */
/*****************************************************************************/

double** MemoryAllocater2D(int RowSize, int ColumnSize)
{
   double** AllocatedArray;
   AllocatedArray = new double*[RowSize];
   if (AllocatedArray ==0)
 		cout << "Error: memory could not be allocated";
   for (int i=0;i<RowSize;i++) {
      AllocatedArray[i] = new double[ColumnSize];
      if (AllocatedArray[i]==0)
         cout << "Error: memory could not be allocated";
   }

	// The allocated memory is initilized to zero
   for (int j=0;j<RowSize;j++) {
   	for (int i=0;i<ColumnSize;i++)
         AllocatedArray[j][i]=0.0;
   }

   return(AllocatedArray);
}  // End of function MemoryAllocater2D()




int** IntMemoryAllocater2D(int RowSize, int ColumnSize)
{
   int** AllocatedArray;
   AllocatedArray = new int*[RowSize];
   if (AllocatedArray ==0)
      cout << "Error: memory could not be allocated";
   for (int i=0;i<RowSize;i++) {
      AllocatedArray[i] = new int[ColumnSize];
      if (AllocatedArray[i]==0)
         cout << "Error: memory could not be allocated";
   }
   
   // The allocated memory is initilized to zero
   for (int j=0;j<RowSize;j++) {
      for (int i=0;i<ColumnSize;i++)
         AllocatedArray[j][i]=0;
   }
   return(AllocatedArray);
}  // End of function IntMemoryAllocater2D()




/*****************************************************************************/
/* MemoryDeAllocater2D function takes the adress of the matrix which is      */
/* to be de-allocated from the memory, the RowSize and ColumnSize of the     */
/* of the matrix. The function DOES NOT use the ColumnSize however it makes  */
/* the function more convenient and readible. Another IMPORTANT              */
/* point is, use always these functions (MemoryAllocater2D                   */
/* & MemoryDeAllocater) together and ALWAYS give attention to the order      */
/* of the memory allocation FIRST ROWS then COLUMNS.                         */
/*****************************************************************************/

void MemoryDeAllocater2D(double** DeAllocatedArray,int RowSize, int ColumnSize)
{
   for(int i=0;i<RowSize;i++) {
      delete[] DeAllocatedArray[i];
   }
   delete[] DeAllocatedArray;
   DeAllocatedArray = NULL;
}



void IntMemoryDeAllocater2D(int** DeAllocatedArray,int RowSize, int ColumnSize)
{
   for(int i=0;i<RowSize;i++) {
      delete[] DeAllocatedArray[i];
   }
   delete[] DeAllocatedArray;
   DeAllocatedArray = NULL;
}

