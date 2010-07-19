#include "slvFunctions.h"




void CalTimeRelaxationForMomentumEqn (double** &uRelaxTM, double** &vRelaxTM, double** UmomaCenter, double** VmomaCenter)
{
   /*****************************************************************************
    This function calculates u and v velocity relaxation parameters for a
    TIME MARCHING solution. Actually what is calculated here are not exactly
    relaxation parameters, but unsteady terms of the transient momentum
    equations. But these two are used in a similar in the solution, therefore
    the name relaxation is used. In an ITERATIVE solution relaxation parameters
    are the same at every node of the flow domain and do not change during the
    iterative solution. On the other hand in a TIME MARCHING solution relaxation
    parameters are different from node to node and they change with time, i.e.
    during the iterations.
   *****************************************************************************/

   for(int J=1; J<NumJ-1; J++) {
     for(int i=1; i<Numi-1; i++) {
        uRelaxTM[J][i] = UmomaCenter[J][i] / (UmomaCenter[J][i] + density * (Yji[J][i]-Yji[J-1][i]) * (XJI[J][i+1] - XJI[J][i]) / timeStep);
     }
   }

   for(int j=1; j<Numj-1; j++) {
      for(int I=1; I<NumI-1; I++) {
         vRelaxTM[j][I] = VmomaCenter[j][I] / (VmomaCenter[j][I] + density * (YJI[j+1][I]- YJI[j][I]) * (Xji[j][I] - Xji[j][I-1]) / timeStep);
      }
   }
} // End of function CalTimeRelaxationForMomentumEqn()




void InitilizeRelaxationParameters(double** &uRelaxTM,double** &vRelaxTM,double** &scalarRelaxTM)
{
   /*****************************************************************************
    This function inilizes relaxation parameters of u and v momentum equations.
    It is performed only once in a run. Relaxation values determined by the
    user are assigned to the whole computational domain for ITERATIVE approach.
    For TIME MARCHING approach valocity relaxation is initialized to 0.9.
   *****************************************************************************/

   if (solverApproach == 1) {                // ITERATIVE solution
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            uRelaxTM[J][i] = uRelax;
         }
      }
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            vRelaxTM[j][I] = vRelax;
         }
      }
      for(int J=1; J<NumJ-1; J++) {
         for(int I=1; I<NumI-1; I++) {
            scalarRelaxTM[J][I] = scalarRelax;
         }
      }
   } else if (solverApproach == 2) {         // TIME MARCHING solution
      // The following 0.9 values as relaxation parameters are used only in the first
      // global iteration. New values will be calculated by the code for the other
      // iteratios, i.e. time steps.

      // For a TIME MARCHING solution using SIMPLEC if 1.0 instead of 0.9 is used
      // division by zero occurs.

      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
           uRelaxTM[J][i] = 0.9;
         }
      }
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            vRelaxTM[j][I] = 0.9;
         }
      }
      for(int J=1; J<NumJ-1; J++) {
         for(int I=1; I<NumI-1; I++) {
            scalarRelaxTM[J][I] = 0.9;
         }
      }
   }
} // End of function InitilizeRelaxationParameters()
