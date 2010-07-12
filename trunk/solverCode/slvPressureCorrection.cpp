/*****************************************************************************/
/* Reference:                                                                */
/* "An Introduction to computational fluid dynamics. The finite volume       */
/* method",H.K.VERSTEEG and W.MALALASEKERE  (Pg 143)                         */
/*****************************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* The following PressureCorrectionMatrix function forms the pressure        */
/* correction matrix that is to be solved by the line TDMA method. The       */
/* function has many parameters some of them are reference parameters which  */
/* are modified in the function.                                            */
/* The function forms the 4 neigbouring node coefficient and 1 center node   */
/* coefficient arrays.                                                       */
/* The remaining part is explained inside the function with further comments */
/*****************************************************************************/

void PressureCorrectionMatrix(double** &PJICorrect, double** &PresCrctWest, double** &PresCrctEast,
                              double** &PresCrctSouth, double** &PresCrctNorth,
                              double** &PresCrctCenter, double** &PresCrctSource,
                              int Numi, int Numj, int NumI, int NumJ, double** UmomaCenter,
                              double** VmomaCenter, double** XjicoorCorners,
                              double** YjicoorCorners, double** UJivelocity, double** VjIvelocity,
                              double** &PresCrctdJi, double** &PresCrctdjI, double** UmomaWest,
                              double** UmomaEast, double** UmomaSouth, double** UmomaNorth,
                              double** VmomaWest, double** VmomaEast, double** VmomaSouth,
                              double** VmomaNorth, int SolverType, double Ro,
                              bool PressureCorrectFlag, double** BoundaryLeft,
                              double** BoundaryRight, double** BoundaryTop, double** BoundaryBottom)
{
	/*****************************************************************************/
	/* Memory is allocaded for the arrays in the main by using a predefined      */
	/* function.The type of the memory allocated is double by default.           */
	/* IMPORTANT : The array size allocated is bigger than the needed since not  */
	/* all of the pressure nodes are computational nodes. The one in the         */
	/* boundaries remains the same but for ease of understanding and keeeping    */
	/* it done so                                                                */
	/*****************************************************************************/

	/*****************************************************************************/
	/* Here two new arrays are formed these are used in the coefficients of the  */
	/* pressure correction matrix. As explained in the book referenced below.    */
	/* Since they are used both in the pressure corection matrix and velocity    */
	/* corection later on these are defined as reference parameters.             */
	/*****************************************************************************/

   if (SolverType == 1 || SolverType == 2) {
      // For x direction and U momentum
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            PresCrctdJi[J][i] = (YjicoorCorners[J][i] - YjicoorCorners[J-1][i]) / UmomaCenter[J][i];
         }
      }

      // For Y direction and V momentum
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            PresCrctdjI[j][I] = (XjicoorCorners[j][I] - XjicoorCorners[j][I-1]) / VmomaCenter[j][I];
         }
      }
   } else if (SolverType == 3) {
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            PresCrctdJi[J][i] = (YjicoorCorners[J][i] - YjicoorCorners[J-1][i]) /
                                (UmomaCenter[J][i] - (UmomaWest[J][i] + UmomaEast[J][i]
                                + UmomaSouth[J][i] + UmomaNorth[J][i]));
         }
      }

      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            PresCrctdjI[j][I] = (XjicoorCorners[j][I] - XjicoorCorners[j][I-1]) /
                                (VmomaCenter[j][I] - (VmomaWest[j][I] + VmomaEast[j][I]
                                + VmomaSouth[j][I] + VmomaNorth[j][I]));
         }
      }
   }

	/*****************************************************************************/
	/* At this point the matrix for the pressure correction is formed. The       */
	/* IMPORTANT point here is this matrix is no different than the momentum     */
	/* matrices solved before however the "PJICorrect" Pressure correction       */
   /* matrix is always formed of zeros as an initial condition. It is impliment */
	/* in the "PressureVelocityCorrecter" function but it is important to        */
	/* speak of it now.                                                          */
	/*****************************************************************************/
        
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         PresCrctWest[J][I] = PresCrctdJi[J][I-1] * Ro *
                              (YjicoorCorners[J][I-1] - YjicoorCorners[J-1][I-1]);

         PresCrctEast[J][I] = PresCrctdJi[J][I] * Ro *
                              (YjicoorCorners[J][I] - YjicoorCorners[J-1][I]);

         PresCrctSouth[J][I] = PresCrctdjI[J-1][I] * Ro *
                               (XjicoorCorners[J-1][I] - XjicoorCorners[J-1][I-1]);

         PresCrctNorth[J][I] = PresCrctdjI[J][I] * Ro *
                               (XjicoorCorners[J][I] - XjicoorCorners[J][I-1]);

         PresCrctCenter[J][I] = PresCrctWest[J][I] + PresCrctEast[J][I] + PresCrctSouth[J][I] +
                                PresCrctNorth[J][I];

         PresCrctSource[J][I] = Ro * (- UJivelocity[J][I] * (YjicoorCorners[J][I] - YjicoorCorners[J-1][I])
                                      + UJivelocity[J][I-1] * (YjicoorCorners[J][I-1] - YjicoorCorners[J-1][I-1])
                                      - VjIvelocity[J][I] * (XjicoorCorners[J][I] - XjicoorCorners[J][I-1])
                                      + VjIvelocity[J-1][I] * (XjicoorCorners[J-1][I] - XjicoorCorners[J-1][I-1]) );
      }
   }

   // This part is about the outlet BC. It should be ommited while solving pressure equation rather
   // than the pressure correction equation

   if (PressureCorrectFlag) {
      // OUTLET BC (part (4/4)
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

} // End of function PressureCorrectionMatrix()

