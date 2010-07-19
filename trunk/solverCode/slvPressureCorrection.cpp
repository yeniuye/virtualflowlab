/*****************************************************************************
 Reference Book: An Introduction to Computational Fluid Dynamics. The Finite
                 Volume Method, H.K. Versteeg and W. Malalasekere (page 143)
*****************************************************************************/

#include "slvFunctions.h"




void PressureCorrectionMatrix(double** &PresCrctWest, double** &PresCrctEast, double** &PresCrctSouth,
                              double** &PresCrctNorth, double** &PresCrctCenter, double** &PresCrctSource,
                              double** UmomaCenter, double** VmomaCenter,
                              double** Uvel, double** Vvel,
                              double** &PresCrctdJi, double** &PresCrctdjI,
                              double** UmomaWest, double** UmomaEast, double** UmomaSouth, double** UmomaNorth,
                              double** VmomaWest, double** VmomaEast, double** VmomaSouth, double** VmomaNorth,
                              bool PressureCorrectFlag)
{
   /*****************************************************************************
    This function forms the pressure correction matrix. It calculates 4
    neigbor node and 1 center node coefficient arrays.

    IMPORTANT: For SIMPLER algorithm this function is also used to calculate
    coefficinents of pressure (as well as pressure correction) equation.
    PressureCorrectFlag variable controls this. If it is eaual to 0 pressure
    equation, if it is equal to 1 pressure correction equation is formed.
   *****************************************************************************/


   /*****************************************************************************
    IMPORTANT : Coefficient arrays use more than required space bacause not all
    pressure nodes are computational nodes. Coefficients of the boundary nodes
    remain the same, but it is done this way for ease of understanding
   *****************************************************************************/

   /*****************************************************************************
    Here two new arrays are formed to be used in the coefficients of pressure
    correction matrix as explained in the reference book. They are used in both
    pressure corection and velocity corection.
   *****************************************************************************/

   if (solverAlgorithm == 1 || solverAlgorithm == 2) {  // SIMPLE and SIMPLEC
      // For x direction and u momentum
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            PresCrctdJi[J][i] = (Yji[J][i] - Yji[J-1][i]) / UmomaCenter[J][i];
         }
      }

      // For y direction and v momentum
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            PresCrctdjI[j][I] = (Xji[j][I] - Xji[j][I-1]) / VmomaCenter[j][I];
         }
      }
   } else if (solverAlgorithm == 3) {  // SIMPLER
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            PresCrctdJi[J][i] = (Yji[J][i] - Yji[J-1][i]) /
                                (UmomaCenter[J][i] - (UmomaWest[J][i] + UmomaEast[J][i]
                                + UmomaSouth[J][i] + UmomaNorth[J][i]));
         }
      }

      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            PresCrctdjI[j][I] = (Xji[j][I] - Xji[j][I-1]) /
                                (VmomaCenter[j][I] - (VmomaWest[j][I] + VmomaEast[j][I]
                                + VmomaSouth[j][I] + VmomaNorth[j][I]));
         }
      }
   }

   /*****************************************************************************
   At this point the matrix for the pressure correction is formed.
   IMPORTANT: This matrix is no different than the momentum matrices solved
   before. However, pressure correction matrix (PJICorrect) is always formed of
   zeros as an initial condition. It is implemented in PressureVelocityCorrecter
   function but it is important to mention it now.
   *****************************************************************************/
        
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         PresCrctWest[J][I]   = PresCrctdJi[J][I-1] * density * (Yji[J][I-1] - Yji[J-1][I-1]);
         PresCrctEast[J][I]   = PresCrctdJi[J][I]   * density * (Yji[J][I]   - Yji[J-1][I]);
         PresCrctSouth[J][I]  = PresCrctdjI[J-1][I] * density * (Xji[J-1][I] - Xji[J-1][I-1]);
         PresCrctNorth[J][I]  = PresCrctdjI[J][I]   * density * (Xji[J][I]   - Xji[J][I-1]);

         PresCrctCenter[J][I] = PresCrctWest[J][I] + PresCrctEast[J][I] + PresCrctSouth[J][I] + PresCrctNorth[J][I];

         PresCrctSource[J][I] = density * (- Uvel[J][I]   * (Yji[J][I]   - Yji[J-1][I]   ) +
                                             Uvel[J][I-1] * (Yji[J][I-1] - Yji[J-1][I-1] ) -
                                             Vvel[J][I]   * (Xji[J][I]   - Xji[J][I-1]   ) +
                                             Vvel[J-1][I] * (Xji[J-1][I] - Xji[J-1][I-1]));
      }
   }

   // This outlet BC part is necessary only for the pressure correction equation.

   if (PressureCorrectFlag) {
      // OUTLET BC (Part 4/4)
      for(int J=1; J<NumJ-1; J++) {
         if (BoundaryRight[J][0] == 3) {
            PresCrctEast[J][NumI-2] = 0;
            PresCrctWest[J][NumI-2] = 0;
         }
         if (BoundaryLeft[J][0] == 3) {
            PresCrctEast[J][1] = 0;   // These values are already zero
            PresCrctWest[J][1] = 0;
         }
      }

      for(int I=1; I<NumI-1; I++) {
         if (BoundaryTop[NumJ-2][I] == 3) {
            PresCrctNorth[NumJ-2][I] = 0;
            PresCrctSouth[NumJ-2][I] = 0;
         }
         if (BoundaryBottom[1][I] == 3) {
            PresCrctNorth[1][I] = 0;
            PresCrctSouth[1][I] = 0;
         }
      }
   }

}  // End of function PressureCorrectionMatrix()
