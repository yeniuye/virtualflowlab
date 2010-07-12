/*****************************************************************************/
/*                          LINE BY LINE TDMA METHOD                         */
/*****************************************************************************/

/*****************************************************************************/
/* References:                                                               */
/* (1)  "Numerical heat transfer and fluid flow", PATANKAR                   */
/* (2)  "An Introduction to computational fluid dynamics. The finite volume  */
/* method",H.K.VERSTEEG and W.MALALASEKERE                                   */
/* (3)  "Computational fluid dynamics volume 1", KLAUS A.HOFFMANN, STEVE T.  */
/* CHIANG                                                                    */
/*****************************************************************************/

#include "slvFunctions.h" 

/*****************************************************************************/
/* LINE BY LINE TDMA method is used in 2D or 3D case. In this code it is     */
/* designed for 2D case. The idea of the method is described in the          */
/* references given below but simply. Distribution of a variable is          */
/* determined (e.g. Temperature) in a plane. This is done by using TDMA on   */
/* on lines and sweep it through the other perpendicular line. In the        */
/* following code the TDMA and the sweep directions can be determined by the */
/* user(from west to east or from south to north)                            */
/*                                                                           */
/* The Function needs 7 arrays. 4 of them are the neigbouring cells and      */
/* 1 of them is the center cells. One of the remaining two arrays is the     */
/* source array, which also serves to impliment the boundary conditions.And  */
/* the last array keeps the variable of interest (e.g. U velocities).        */
/* The function has four integer parameters the number of cells in x and y   */
/* direction are the two of them. "IterationNum" determines the number of    */
/* sweep loops that is to be performed. And the last integer parameter       */
/* TypeSweep determines the direction of sweep.                              */
/*                                                                           */
/* NOTE: The "TemperatureArray" is the array that keeps the variable of      */
/* interest although it is name is the TemperatureArray it may keep the      */
/* U & V velocities or Pressure values. It depends on what kind of matrix    */
/* does the LineTDMA solves currently.                                       */
/*                                                                           */
/* IMPORTANT POINTS :                                                        */
/* (1) To use the LineTDMASolver the size of the domain should be increased  */
/* by 2 in both directions. One Column of zeros before the first column and  */
/* one more column of zeros after the last column. Similarly for rows. The   */
/* aim of these zero columns and rows is to tread the boundary nodes same    */
/* as the internal nodes. Not only the variable array (TemperatureArray) but */
/* also the remaining arrays are increased in size although they are NEVER   */
/* used but it is sure increase the readibility of the code.                 */
/*****************************************************************************/

void LineTDMASolver(double** NorthArray,double** SouthArray,double** WestArray,
double** EastArray,double** CenterArray,double** SourceArray,int NumXcell,
int NumYcell,double** TemperatureArray,int IterationNum,int TypeSweep)
{
   /*****************************************************************************/
   /* Memory is allocated for the 4 arrays that are to be sent to TDMA function */
   /* Tarray is defined for temporary use. It is used to take the outputs of    */
   /* TDMA function and fill a matrix from these arrays.                        */
   /*****************************************************************************/

   double* Aarray;
   double* Barray;
   double* Carray;
   double* Darray;
   double* Tarray;

   if (TypeSweep == 1) {
      Aarray = new double[NumYcell-2];
 	   Barray = new double[NumYcell-2];
 	   Carray = new double[NumYcell-2];
 	   Darray = new double[NumYcell-2];
 	   Tarray = new double[NumYcell-2];
   } else if (TypeSweep == 2) {
 	   Aarray = new double[NumXcell-2];
 	   Barray = new double[NumXcell-2];
 	   Carray = new double[NumXcell-2];
 	   Darray = new double[NumXcell-2];
 	   Tarray = new double[NumXcell-2];
   } else {
      cout << "Error in LineTDMASolver() function\n";
   }

   /*****************************************************************************/
   /* The most outer "for loop" is to determine the # of sweeps that are to be  */
   /* performed. The most inner loop[2] is used to fill the 4 arrays that are   */
   /* proper for the use of TDMA function. The other inner loop [3] is for      */
   /* check purpose it can be comment out. The [4] inner loop is used to form   */
   /* a matrix from the outputs of succesive TDMASolver calls.                  */
   /*****************************************************************************/
   
   if (TypeSweep == 1) {
      for (int iteration=0; iteration<IterationNum; iteration++){
         for(int k=1; k<NumXcell-1; k++) {             // COLUMNN             [1]
            for(int i=0; i<NumYcell-2; i++) {          // ROW                 [2]
               Aarray[i] = CenterArray[i+1][k];
               Carray[i] = SouthArray[i+1][k];
               Barray[i] = NorthArray[i+1][k];
               Darray[i] = WestArray[i+1][k] * TemperatureArray[i+1][k-1] +
                           EastArray[i+1][k] * TemperatureArray[i+1][k+1] + SourceArray[i+1][k];
            }

            TDMASolver(Carray, Aarray, Barray, Darray, Tarray, NumYcell-2);

            for(int i=0; i<NumYcell-2; i++)           //                      [4]
                TemperatureArray[i+1][k] = Tarray[i];
         }
      }
   } else if (TypeSweep == 2) {
   	for (int iteration=0; iteration<IterationNum; iteration++) {
         for(int j=1; j<NumYcell-1; j++) {            // ROW                  [1]
            for(int I=0; I<NumXcell-2; I++) {         // COLUMN               [2]
               Aarray[I] = CenterArray[j][I+1];
               Carray[I] = WestArray[j][I+1];
               Barray[I] = EastArray[j][I+1];
               Darray[I] = SouthArray[j][I+1] * TemperatureArray[j-1][I+1] +
                           NorthArray[j][I+1] * TemperatureArray[j+1][I+1] + SourceArray[j][I+1];
            }

            TDMASolver(Carray, Aarray, Barray, Darray, Tarray, NumXcell-2);

            for(int I=0; I<NumXcell-2; I++)           //                      [4]
               TemperatureArray[j][I+1] = Tarray[I];
         }
      }
   }

   /* Memories that are used in the TDMA are deallocated */
   delete[] Aarray;
   Aarray = NULL;
   delete[] Barray;
   Barray = NULL;
   delete[] Carray;
   Carray = NULL;
   delete[] Darray;
   Darray = NULL;
   delete[] Tarray;
   Tarray = NULL;

}  // End of function LineTDMASolver()


/*****************************************************************************/
/* The TDMA fonction has 4 parameter arrays "Carray,Aarray,Barray,Darray"    */
/* and an integer which carry the size of arrays.                            */
/* TDMA takes the arrays according to the below mathematical model.          */
/* A(@i) * T(@i) = B(@i) * T(@i+1) + C(@i) * T(@i-1) + D(@i)       (FORM-1)  */
/* @ "at" used to denote subscripts e.g. T(@i+1) means subscript of T is i+1 */
/* The general matrix form is a bit different than the mathematical model    */
/* above.                                                                    */
/* - C(@i) * T(@i-1) + A(@i) * T(@i) -  B(@i) * T(@i+1) = D(@i)    (FORM-2)  */
/* Aarray = The diagonal array of the corresponding matrix                   */
/* Barray =(-1)*Right neighbouring array of the diagonal array in the matrix */
/* Carray =(-1)*Left neighbouring array of the diagonal array in the matrix  */
/* Darray = Resulting array (Constant array)                                 */
/* However we do not need to make any manipulations in the arrays since      */
/* the LineTDMA sends the array according to the FORM-1.                     */
/*****************************************************************************/


void TDMASolver(double* Carray, double* Aarray, double* Barray, double* Darray, double* Tarray, int size)
{

   /* The arrays that are to be used in the calculation process is created
   dynamically, with the size paremeter of the TDMASolver function. */

   double *Parray;
   Parray = new double[size];
   double *Qarray;
   Qarray = new double[size];
   
   /* Created arrays are initilized to zero */
   for(int i=0; i<size; i++) {
      Parray[i] = 0.0;
      Qarray[i] = 0.0;
   }

   /*****************************************************************************/
   /* First elements of the newly created arrays can be calculated easily since */
   /* Carray[0] = 0 which makes the corresponding equation simpler              */
   /* Parray[0] = Barray[0] / Aarray[0];                                        */
   /* Qarray[0] = Darray[0] / Aarray[0];                                        */
   /* However NO SPRECIAL treatment is needed. The same equation can also be    */
   /* applied for these first elements of the arrays.                           */
   /*****************************************************************************/

   Parray[0] = Barray[0] / Aarray[0];
   Qarray[0] = Darray[0] / Aarray[0];

   for(int i=1;i<size;i++) {
      Parray[i]= Barray[i] / (Aarray[i] - Carray[i] * Parray[i-1]);
      Qarray[i]=(Darray[i] + Carray[i] * Qarray[i-1]) / (Aarray[i] - Carray[i] * Parray[i-1]);
   }

   Tarray[size-1] = Qarray[size-1];
   for(int i=size-2; i>-1; i--) {
      Tarray[i] = Tarray[i+1] * Parray[i] + Qarray[i];
   }

   /* The memory allocated for the arrays are deallocated here */

   delete[] Parray;
   Parray = NULL;
   delete[] Qarray;
   Qarray = NULL;

}  // End of function TDMASolver()

