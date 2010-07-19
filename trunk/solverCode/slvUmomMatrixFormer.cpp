#include "slvFunctions.h"




void UmomMatrixFormer(double** &UmomaWest, double** &UmomaEast, double** &UmomaSouth,
                      double** &UmomaNorth, double** &UmomaCenter, double** &UmomaSource,
                      double** UmomXConvFlux, double** UmomYConvFlux, double** UmomXDiffCond,
                      double** UmomYDiffCond, double** uRelaxTM, int** BlockCellIndex)
{
   /*****************************************************************************
    This function calculates the coefficients of the u momentum matrix stored in
    6 matrices. 4 of these are for neighbouring nodes, 1 for the u cell of
    interest and 1 for the source term.

    IMPORTANT : These 6 arrays are allocated in the main function as larger than
    the necessary size. Some of the u momentum cells are not computational
    cells e.g. u cells at the West and East boundaries. However, this extra
    memory allocation makes the indexing easier to follow.

    IMPORTANT : u momentum relaxation is also implemented in this function.
    Relaxation effects center and source terms.
   *****************************************************************************/


   /*************************  IMPLEMENTATION OF BCs ****************************
    First part is for the implementation of BCs of u momentum equation. Boundary
    effects are added to the system as source terms. For a stationary solid wall
    the driving force is the shear stress and the effect of it is implemented as
    a source (BndryCntrSrc) to UmomaCenter term.
    
    Moving walls have two source terms, one is similar to the stationary wall
    case as explained above. The other source term (BndryRHSSrc) is added to
    UmomaSource term.

    For u momentum equation right and left walls do not need any special
    treatment because u velocity is perpendicular to these walls.

    // Cuneyt: Think about this comment

    *****************************************************************************/

   double** BndryCntrSrc;
   double** BndryRHSSrc;
   BndryCntrSrc = MemoryAllocater2D(NumJ,Numi);
   BndryRHSSrc = MemoryAllocater2D(NumJ,Numi);

   // WALL BC (part 1/3)
   // Following for loop creates the necessary sources for wall BCs
   for(int i=1; i<Numi-1; i++) {
      if (BoundaryBottom[i][0] == 1) {
         BndryCntrSrc[1][i] = -1 * kinvis / (YJi[1][i] - YJi[0][i]) * (XjI[0][i+1] - XjI[0][i]);
         BndryRHSSrc[1][i]  =      kinvis / (YJi[1][i] - Yji[0][i]) * (XjI[0][i+1] - XjI[0][i]) * BoundaryBottom[i][1];
      }

      if (BoundaryTop[i][0] == 1) {
         BndryCntrSrc[NumJ-2][i] = -1 * kinvis / (YJi[NumJ-1][i] - YJi[NumJ-2][i]) * (XjI[NumJ-2][i+1] - XjI[NumJ-2][i]);
         BndryRHSSrc[NumJ-2][i]  =      kinvis / (Yji[NumJ-2][i] - YJi[NumJ-2][i]) * (XjI[NumJ-2][i+1] - XjI[NumJ-2][i]) * BoundaryTop[i][1];
      }
   }

   // Modification for blocked cells which are treated as solid walls
   for(int NumBlck=0; NumBlck<nBlockCells; NumBlck++) {
      for(int J=BlockCellIndex[NumBlck][0]+1; J<BlockCellIndex[NumBlck][1]+1; J++) {
         for(int i=BlockCellIndex[NumBlck][2]; i<BlockCellIndex[NumBlck][3]+1; i++) {
            BndryCntrSrc[J][i] = -1 * pow(10.0, 30);
            BndryRHSSrc[J][i]= 0;
         }
      }
   }


   // Calculate the coefficient of u momentum equation. It is important to remember
   // that NOT all u cells are computational cells.

   /************************   CENTRAL DIFFERENCING ******************************/
   if (discSchm == 1) {
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            UmomaWest[J][i]  = (UmomXDiffCond[J][i]   + 0.5 * UmomXConvFlux[J][i])   * (YjI[J][i]     - YjI[J-1][i]);
            UmomaEast[J][i]  = (UmomXDiffCond[J][i+1] - 0.5 * UmomXConvFlux[J][i+1]) * (YjI[J][i+1]   - YjI[J-1][i+1]);
            UmomaSouth[J][i] = (UmomYDiffCond[J-1][i] + 0.5 * UmomYConvFlux[J-1][i]) * (XjI[J-1][i+1] - XjI[J-1][i]);
            UmomaNorth[J][i] = (UmomYDiffCond[J][i]   - 0.5 * UmomYConvFlux[J][i])   * (XjI[J][i+1]   - XjI[J][i]);
         }
      }
   }
   /***********************   UPWIND DIFFERENCING ********************************/
   else if (discSchm == 2) {
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            UmomaWest[J][i]  = (UmomXDiffCond[J][i]   + max(0.0, UmomXConvFlux[J][i])     ) * (YjI[J][i]     - YjI[J-1][i]);
            UmomaEast[J][i]  = (UmomXDiffCond[J][i+1] + max(0.0, -1*UmomXConvFlux[J][i+1])) * (YjI[J][i+1]   - YjI[J-1][i+1]);
            UmomaSouth[J][i] = (UmomYDiffCond[J-1][i] + max(0.0, UmomYConvFlux[J-1][i])   ) * (XjI[J-1][i+1] - XjI[J-1][i]);
            UmomaNorth[J][i] = (UmomYDiffCond[J][i]   + max(0.0, -1*UmomYConvFlux[J][i])  ) * (XjI[J][i+1]   - XjI[J][i]);
         }
      }
   }
   /**********************   HYBRID DIFFERENCING *********************************/
   else if (discSchm == 3) {
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            UmomaWest[J][i]  = (max(max(0.0, UmomXConvFlux[J][i]     ), (UmomXDiffCond[J][i]   + 0.5 * UmomXConvFlux[J][i]))  ) * (YjI[J][i]     - YjI[J-1][i]);
            UmomaEast[J][i]  = (max(max(0.0, -1*UmomXConvFlux[J][i+1]), (UmomXDiffCond[J][i+1] - 0.5 * UmomXConvFlux[J][i+1]))) * (YjI[J][i+1]   - YjI[J-1][i+1]);
            UmomaSouth[J][i] = (max(max(0.0, UmomYConvFlux[J-1][i]   ), (UmomYDiffCond[J-1][i] + 0.5 * UmomYConvFlux[J-1][i]))) * (XjI[J-1][i+1] - XjI[J-1][i]);
            UmomaNorth[J][i] = (max(max(0.0, -1*UmomYConvFlux[J][i]  ), (UmomYDiffCond[J][i]   - 0.5 * UmomYConvFlux[J][i]))  ) * (XjI[J][i+1]   - XjI[J][i]);
         }
      }
   }
   /**********************   POWERLAW DIFFERENCING *******************************/
   else if (discSchm == 4) {
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            UmomaWest[J][i]  = (UmomXDiffCond[J][i]   * max(0.0, pow(1 - 0.1 * fabs(UmomXConvFlux[J][i]   / UmomXDiffCond[J][i]), 5)  ) + max(0.0, UmomXConvFlux[J][i])     ) * (YjI[J][i]     - YjI[J-1][i]);
            UmomaEast[J][i]  = (UmomXDiffCond[J][i+1] * max(0.0, pow(1 - 0.1 * fabs(UmomXConvFlux[J][i+1] / UmomXDiffCond[J][i+1]), 5)) + max(0.0, -1*UmomXConvFlux[J][i+1])) * (YjI[J][i+1]   - YjI[J-1][i+1]);
            UmomaSouth[J][i] = (UmomYDiffCond[J-1][i] * max(0.0, pow(1 - 0.1 * fabs(UmomYConvFlux[J-1][i] / UmomYDiffCond[J-1][i]), 5)) + max(0.0, UmomYConvFlux[J-1][i])   ) * (XjI[J-1][i+1] - XjI[J-1][i]);
            UmomaNorth[J][i] = (UmomYDiffCond[J][i]   * max(0.0, pow(1 - 0.1 * fabs(UmomYConvFlux[J][i]   / UmomYDiffCond[J][i]), 5)  ) + max(0.0, -1*UmomYConvFlux[J][i])  ) * (XjI[J][i+1]   - XjI[J][i]);
         }
      }
   }

   // STEADY or TRANSIENT part

   if (isTransient == 0) {          // STEADY
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            UmomaCenter[J][i] = (UmomaWest[J][i] + UmomaEast[J][i] + UmomaSouth[J][i] + UmomaNorth[J][i] +
                                 UmomXConvFlux[J][i+1] * (YjI[J][i+1]   - YjI[J-1][i+1]) -
                                 UmomXConvFlux[J][i]   * (YjI[J][i]     - YjI[J-1][i]  ) +
                                 UmomYConvFlux[J][i]   * (XjI[J][i+1]   - XjI[J][i]    ) -
                                 UmomYConvFlux[J-1][i] * (XjI[J-1][i+1] - XjI[J-1][i]  ) -
                                 BndryCntrSrc[J][i] ) / uRelaxTM[J][i];

            UmomaSource[J][i] = (pJI[J][i] - pJI[J][i+1]) * (Yji[J][i] - Yji[J-1][i]) + BndryRHSSrc[J][i] +
                                (1 - uRelaxTM[J][i]) * UmomaCenter[J][i] * UJi[J][i];
         }
      }
   } else if (isTransient == 1) {   // TRANSIENT
      for(int J=1; J<NumJ-1; J++) {
         for(int i=1; i<Numi-1; i++) {
            UmomaCenter[J][i] = (UmomaWest[J][i] + UmomaEast[J][i] + UmomaSouth[J][i] + UmomaNorth[J][i] +
                                 UmomXConvFlux[J][i+1] * (YjI[J][i+1]   - YjI[J-1][i+1]) -
                                 UmomXConvFlux[J][i]   * (YjI[J][i]     - YjI[J-1][i]  ) +
                                 UmomYConvFlux[J][i]   * (XjI[J][i+1]   - XjI[J][i]    ) -
                                 UmomYConvFlux[J-1][i] * (XjI[J-1][i+1] - XjI[J-1][i]  ) -
                                 BndryCntrSrc[J][i] ) + density * (Yji[J][i] - Yji[J-1][i]) * (XJI[J][i+1] - XJI[J][i]) / timeStep;

            UmomaSource[J][i] = (pJI[J][i] - pJI[J][i+1]) * (Yji[J][i] - Yji[J-1][i]) + BndryRHSSrc[J][i] +
                                 density * (Yji[J][i]-Yji[J-1][i]) * (XJI[J][i+1] - XJI[J][i]) / timeStep * UJi[J][i];
         }
      }
   }


   // OUTLET BC (part (2/4)
   for(int J=1; J<NumJ-1; J++) {
      if (BoundaryRight[J][0] == 3) {
         UmomaCenter[J][Numi-2] = UmomaCenter[J][Numi-2] - UmomaEast[J][Numi-2];
         UmomaEast[J][Numi-2] = 0;
      }
      if (BoundaryLeft[J][0] == 3) {
         UmomaCenter[J][1] = UmomaCenter[J][1] - UmomaWest[J][1];
         UmomaWest[J][1] = 0;
      }
   }

   for(int i=1; i<Numi-1; i++) {
      if (BoundaryTop[i][0] == 3) {
         UmomaCenter[NumJ-2][i] = UmomaCenter[NumJ-2][i] - UmomaNorth[NumJ-2][i];
         UmomaNorth[NumJ-2][i] = 0;
      }
      if (BoundaryBottom[i][0] == 3) {
         UmomaCenter[1][i] = UmomaCenter[1][i] - UmomaSouth[1][i];
         UmomaSouth[1][i] = 0;
      }
   }

   MemoryDeAllocater2D(BndryCntrSrc, NumJ, Numi);
   MemoryDeAllocater2D(BndryRHSSrc, NumJ, Numi);

}  // End of function UmomMatrixFormer()
