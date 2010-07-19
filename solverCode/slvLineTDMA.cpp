#include "slvFunctions.h" 




void LineTDMASolver(double** NorthArray,double** SouthArray,double** WestArray,
double** EastArray,double** CenterArray,double** SourceArray,int Num1,int Num2,
double** UnknownArray,int nSweep,int sweepDir)
{
   /*****************************************************************************
    References:
    (1) "Numerical Heat Transfer and Fluid Flow", S.V. Patankar
    (2) "An Introduction to Computational Fluid Dynamics. The Finite Volume Method",
        H.K. Versteeg and W. Malalasekera
    (3) "Computational Fluid Dynamics Vol. 1", K.A. Hoffmann, S.T. Chiang

    In line-by-line TDMA method, distribution of an unknown is determined in a
    2D plane by first applying TDMA on lines aligned with one coordinate direction
    and sweeping the solution in the other direction. In the following code
    initial TDMA and sweep directions can be determined by the user (from West to
    East or from South to North)

    This function needs 7 arrays. 4 of them are for neigbouring cells and 1 is for
    center cell. One array is the source array, which is also used to impliment
    boundary conditions. The last array keeps the unknowns, e.g. u velocities.
    Num1 and Num2 are the number of unknowns in x and y directions. nSweep
    determines the number of sweep loops and sweepDir determines the direction of
    sweep.

    IMPORTANT: To use LineTDMASolver the size of the domain should be increased
    by 2 in both directions. One column of zeros before the first column and
    one more column of zeros after the last column are added. Similarly size
    increase applies for rows too. By adding these columns and rows it is possible
    to treat boundary nodes same as internal nodes.
   *****************************************************************************/

   double* Aarray;
   double* Barray;
   double* Carray;
   double* Darray;
   double* Tarray;

   /*****************************************************************************
    Memory is allocated for the 4 arrays that are to be sent to TDMA function.
    Tarray is a temporary array. It is used to store the output of TDMA function.
   *****************************************************************************/

   if (sweepDir == 1) {
      Aarray = new double[Num2 - 2];
 	   Barray = new double[Num2 - 2];
 	   Carray = new double[Num2 - 2];
 	   Darray = new double[Num2 - 2];
 	   Tarray = new double[Num2 - 2];
   } else {
 	   Aarray = new double[Num1 - 2];
 	   Barray = new double[Num1 - 2];
 	   Carray = new double[Num1 - 2];
 	   Darray = new double[Num1 - 2];
 	   Tarray = new double[Num1 - 2];
   }

   /*****************************************************************************
    Outmost loop is for sweeping. Loop [2] is used to fill the 4 arrays that are
    going to be send to TDMASolver function. Loop [3] is used to transfer the
    output of succesive TDMASolver calls to UnknownArray.
   *****************************************************************************/
   
   if (sweepDir == 1) {
      for (int s=0; s<nSweep; s++){                // Sweep loop
         for(int k=1; k<Num1-1; k++) {             // COLUMNN             [1]
            for(int i=0; i<Num2-2; i++) {          // ROW                 [2]
               Aarray[i] = CenterArray[i+1][k];
               Carray[i] = SouthArray[i+1][k];
               Barray[i] = NorthArray[i+1][k];
               Darray[i] = WestArray[i+1][k] * UnknownArray[i+1][k-1] +
                           EastArray[i+1][k] * UnknownArray[i+1][k+1] +
                           SourceArray[i+1][k];
            }

            TDMASolver(Carray, Aarray, Barray, Darray, Tarray, Num2-2);

            for(int i=0; i<Num2-2; i++)           //                      [3]
                UnknownArray[i+1][k] = Tarray[i];
         }
      }
   } else {
   	for (int s=0; s<nSweep; s++) {              // Sweep loop
         for(int j=1; j<Num2-1; j++) {            // ROW                  [1]
            for(int I=0; I<Num1-2; I++) {         // COLUMN               [2]
               Aarray[I] = CenterArray[j][I+1];
               Carray[I] = WestArray[j][I+1];
               Barray[I] = EastArray[j][I+1];
               Darray[I] = SouthArray[j][I+1] * UnknownArray[j-1][I+1] +
                           NorthArray[j][I+1] * UnknownArray[j+1][I+1] +
                           SourceArray[j][I+1];
            }

            TDMASolver(Carray, Aarray, Barray, Darray, Tarray, Num1-2);

            for(int I=0; I<Num1-2; I++)           //                      [3]
               UnknownArray[j][I+1] = Tarray[I];
         }
      }
   }

   delete[] Aarray;
   delete[] Barray;
   delete[] Carray;
   delete[] Darray;
   delete[] Tarray;
   Aarray = NULL;
   Barray = NULL;
   Carray = NULL;
   Darray = NULL;
   Tarray = NULL;

}  // End of function LineTDMASolver()




void TDMASolver(double* Carray, double* Aarray, double* Barray, double* Darray, double* Tarray, int size)
{
   /*****************************************************************************
    This function takes 4 array parameters Carray, Aarray, Barray, Darray,
    and an integer to specify the size of these arrays. Tarray is used to store
    the results.

    TDMA takes the arrays according to the following structure
    A(i) * T(i) = B(i) * T(i+1) + C(i) * T(i-1) + D(i)
    
    which is a different representation of the following
    - C(i) * T(i-1) + A(i) * T(i) -  B(i) * T(i+1) = D(i)

    Aarray = Diagonal array of the corresponding matrix
    Barray = (-1) * Right neighbouring array of the diagonal array
    Carray = (-1) * Left neighbouring array of the diagonal array
    Darray = Right hand side array
   *****************************************************************************/

   // Create two temporary arrays and fill them in.
   double *Parray;
   double *Qarray;

   Parray = new double[size];
   Qarray = new double[size];
   
   for(int i=0; i<size; i++) {
      Parray[i] = 0.0;
      Qarray[i] = 0.0;
   }

   Parray[0] = Barray[0] / Aarray[0];  // Note that Carray[0] = 0
   Qarray[0] = Darray[0] / Aarray[0];  // Note that Carray[0] = 0

   for(int i=1; i<size; i++) {
      Parray[i] = Barray[i] / (Aarray[i] - Carray[i] * Parray[i-1]);
      Qarray[i] = (Darray[i] + Carray[i] * Qarray[i-1]) / (Aarray[i] - Carray[i] * Parray[i-1]);
   }

   Tarray[size-1] = Qarray[size-1];

   for(int i=size-2; i>-1; i--) {
      Tarray[i] = Tarray[i+1] * Parray[i] + Qarray[i];
   }

   delete[] Parray;
   delete[] Qarray;
   Parray = NULL;
   Qarray = NULL;

}  // End of function TDMASolver()
