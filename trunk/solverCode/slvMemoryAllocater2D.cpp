#include "slvFunctions.h"




double** MemoryAllocater2D(int RowSize, int ColumnSize)
{
   /*****************************************************************************
    This function takes two parameters as row and column size of the matrix
    to be allocated. It checks for available memory before doing the allocation.
    It returns (**double) adress of the allocated matrix.

    There is also an integer version of this function.
   *****************************************************************************/
   
   double** AllocatedArray;
   AllocatedArray = new double*[RowSize];
   
   if (AllocatedArray ==0)
      cout << "Error: memory could not be allocated";
   for (int i=0;i<RowSize;i++) {
      AllocatedArray[i] = new double[ColumnSize];
      if (AllocatedArray[i]==0)
         cout << "Error: memory could not be allocated";
   }

   // Initialize allocated memory to zero
   for (int j=0;j<RowSize;j++) {
      for (int i=0;i<ColumnSize;i++)
         AllocatedArray[j][i]=0.0;
   }

   return(AllocatedArray);
}  // End of function MemoryAllocater2D()




int** IntMemoryAllocater2D(int RowSize, int ColumnSize)
{
   // This is the integer version of the previous function
   
   int** AllocatedArray;
   AllocatedArray = new int*[RowSize];
   
   if (AllocatedArray ==0)
      cout << "Error: memory could not be allocated";
   for (int i=0;i<RowSize;i++) {
      AllocatedArray[i] = new int[ColumnSize];
      if (AllocatedArray[i]==0)
         cout << "Error: memory could not be allocated";
   }
   
   // Initialize allocated memory to zero
   for (int j=0;j<RowSize;j++) {
      for (int i=0;i<ColumnSize;i++)
         AllocatedArray[j][i]=0;
   }
   return(AllocatedArray);
}  // End of function IntMemoryAllocater2D()





void MemoryDeAllocater2D(double** DeAllocatedArray,int RowSize, int ColumnSize)
{
   /*****************************************************************************
    This function takes the adress of the matrix to be deallocated as well as
    the RowSize and ColumnSize of the of the matrix. Actually ColumnSize is not
    used, but it seemed more robust to code it this way.

    MemoryAllocater2D function and this function should always be used together.
   
    Note that the proper order of the memory allocation is first rows, then
    columns.

    There is also an integer version of this function.
   *****************************************************************************/
   
   for(int i=0;i<RowSize;i++) {
      delete[] DeAllocatedArray[i];
   }
   delete[] DeAllocatedArray;
   DeAllocatedArray = NULL;
}




void IntMemoryDeAllocater2D(int** DeAllocatedArray,int RowSize, int ColumnSize)
{
   // This is the integer version of the previous function.

   for(int i=0;i<RowSize;i++) {
      delete[] DeAllocatedArray[i];
   }
   delete[] DeAllocatedArray;
   DeAllocatedArray = NULL;
}
