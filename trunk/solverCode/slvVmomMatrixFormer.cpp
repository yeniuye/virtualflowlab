#include "slvFunctions.h"




void VmomMatrixFormer(double** &VmomaWest, double** &VmomaEast, double** &VmomaSouth,
                      double** &VmomaNorth, double** &VmomaCenter, double** &VmomaSource,
                      double** VmomXConvFlux, double** VmomYConvFlux, double** VmomXDiffCond,
                      double** VmomYDiffCond, double** vRelaxTM, int** BlockCellIndex)
{
   /*****************************************************************************
    This function calculates the coefficients of the v momentum matrix stored in
    6 matrices. 4 of these are for neighbouring nodes, 1 for the u cell of
    interest and 1 for the source term.

    IMPORTANT : These 6 arrays are allocated in the main function as larger than
    the necessary size. Some of the v momentum cells are not computational
    cells e.g. v cells at the North and South boundaries. However, this extra
    memory allocation makes the indexing easier to follow.

    IMPORTANT : v momentum relaxation is also implemented in this function.
    Relaxation effects center and source terms.
   *****************************************************************************/


   /*************************  IMPLEMENTATION OF BCs ****************************
    First part is for the implementation of BCs of v momentum equation. Boundary
    effects are added to the system as source terms. For a stationary solid wall
    the driving force is the shear stress and the effect of it is implemented as
    a source (BndryCntrSrc) to UmomaCenter term.
    
    Moving walls have two source terms, one is similar to the stationary wall
    case as explained above. The other source term (BndryRHSSrc) is added to
    VmomaSource term.

    For v momentum equation top and bottom walls do not need any special
    treatment because v velocity is perpendicular to these walls.
   
   // Cuneyt: Think about this comment

   *****************************************************************************/
   
   double** BndryCntrSrc;
   BndryCntrSrc = MemoryAllocater2D(Numj,NumI);
   double** BndryRHSSrc;
   BndryRHSSrc = MemoryAllocater2D(Numj,NumI);

   // WALL BC (Part 2/3)
   // Following for loop creates the necessary sources for wall BCs
   for(int j=1; j<Numj-1; j++) {
      if (BoundaryLeft[j][0]==1) {
         BndryCntrSrc[j][1] = -1 * kinvis / (XjI[j][1] - XjI[j][0]) * (YJi[j+1][0] - YJi[j][0]);
         BndryRHSSrc[j][1]  =      kinvis / (XjI[j][1] - Xji[j][0]) * (YJi[j+1][0] - YJi[j][0]) * BoundaryLeft[j][1];
      }

      if (BoundaryRight[j][0]==1) {
         BndryCntrSrc[j][NumI-2] = -1 * kinvis / (XjI[j][NumI-1] - XjI[j][NumI-2]) * (YJi[j+1][NumI-2] - YJi[j][NumI-2]);
         BndryRHSSrc[j][NumI-2]  =      kinvis / (Xji[j][NumI-2] - XjI[j][NumI-2]) * (YJi[j+1][NumI-2] - YJi[j][NumI-2]) * BoundaryRight[j][1];
      }
   }

   // BLOCK CELL CALCULATION
   for(int NumBlck=0; NumBlck<nBlockCells; NumBlck++) {
      for(int j=BlockCellIndex[NumBlck][0]; j<BlockCellIndex[NumBlck][1]+1; j++) {
         for(int I=BlockCellIndex[NumBlck][2]+1; I<BlockCellIndex[NumBlck][3]+1; I++) {
            BndryCntrSrc[j][I] = -1 * pow(10.0,30);
            BndryRHSSrc[j][I]= 0;
         }
      }
   }


   // Calculate the coefficient of v momentum equation. It is important to remember
   // that NOT all v cells are computational cells.

   /**********************   CENTRAL DIFFERENCING ********************************/
   if (discSchm == 1) {
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            VmomaWest[j][I]  = (VmomXDiffCond[j][I-1] + 0.5 * VmomXConvFlux[j][I-1]) * (YJi[j+1][I-1] - YJi[j][I-1]);
            VmomaEast[j][I]  = (VmomXDiffCond[j][I]   - 0.5 * VmomXConvFlux[j][I])   * (YJi[j+1][I]   - YJi[j][I]);
            VmomaSouth[j][I] = (VmomYDiffCond[j][I]   + 0.5 * VmomYConvFlux[j][I])   * (XJi[j][I]     - XJi[j][I-1]);
            VmomaNorth[j][I] = (VmomYDiffCond[j+1][I] - 0.5 * VmomYConvFlux[j+1][I]) * (XJi[j+1][I]   - XJi[j+1][I-1]);
         }
      }
   }
   /***********************   UPWIND DIFFERENCING ********************************/
   else if (discSchm == 2) {
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            VmomaWest[j][I]  = (VmomXDiffCond[j][I-1] + max(0.0, VmomXConvFlux[j][I-1])   ) * (YJi[j+1][I-1] - YJi[j][I-1]);
            VmomaEast[j][I]  = (VmomXDiffCond[j][I]   + max(0.0, -1*VmomXConvFlux[j][I])  ) * (YJi[j+1][I]   - YJi[j][I]);
            VmomaSouth[j][I] = (VmomYDiffCond[j][I]   + max(0.0, VmomYConvFlux[j][I])     ) * (XJi[j][I]     - XJi[j][I-1]);
            VmomaNorth[j][I] = (VmomYDiffCond[j+1][I] + max(0.0, -1*VmomYConvFlux[j+1][I])) * (XJi[j+1][I]   - XJi[j+1][I-1]);
         }
      }
   }
   /**********************   HYBRID DIFFERENCING *********************************/
   else if (discSchm == 3) {
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            VmomaWest[j][I]  = (max(max(0.0, VmomXConvFlux[j][I-1]   ), (VmomXDiffCond[j][I-1] + 0.5 * VmomXConvFlux[j][I-1]))) * (YJi[j+1][I-1] - YJi[j][I-1]);
            VmomaEast[j][I]  = (max(max(0.0, -1*VmomXConvFlux[j][I]  ), (VmomXDiffCond[j][I]   - 0.5 * VmomXConvFlux[j][I]))  ) * (YJi[j+1][I]   - YJi[j][I]);
            VmomaSouth[j][I] = (max(max(0.0, VmomYConvFlux[j][I]     ), (VmomYDiffCond[j][I]   + 0.5 * VmomYConvFlux[j][I]))  ) * (XJi[j][I]     - XJi[j][I-1]);
            VmomaNorth[j][I] = (max(max(0.0, -1*VmomYConvFlux[j+1][I]), (VmomYDiffCond[j+1][I] - 0.5 * VmomYConvFlux[j+1][I]))) * (XJi[j+1][I]   - XJi[j+1][I-1]);
         }
      }
   }
   /**********************   POWERLAW DIFFERENCING *******************************/
   else if (discSchm == 4) {
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            VmomaWest[j][I]  = (VmomXDiffCond[j][I-1] * max(0.0, pow(1 - 0.1 * fabs(VmomXConvFlux[j][I-1] / VmomXDiffCond[j][I-1]), 5)) + max(0.0, VmomXConvFlux[j][I-1])   ) * (YJi[j+1][I-1] - YJi[j][I-1]);
            VmomaEast[j][I]  = (VmomXDiffCond[j][I]   * max(0.0, pow(1 - 0.1 * fabs(VmomXConvFlux[j][I]   / VmomXDiffCond[j][I]), 5)  ) + max(0.0, -1*VmomXConvFlux[j][I])  ) * (YJi[j+1][I]   - YJi[j][I]);
            VmomaSouth[j][I] = (VmomYDiffCond[j][I]   * max(0.0, pow(1 - 0.1 * fabs(VmomYConvFlux[j][I]   / VmomYDiffCond[j][I]), 5)  ) + max(0.0, VmomYConvFlux[j][I])     ) * (XJi[j][I]     - XJi[j][I-1]);
            VmomaNorth[j][I] = (VmomYDiffCond[j+1][I] * max(0.0, pow(1 - 0.1 * fabs(VmomYConvFlux[j+1][I] / VmomYDiffCond[j+1][I]), 5)) + max(0.0, -1*VmomYConvFlux[j+1][I])) * (XJi[j+1][I]   - XJi[j+1][I-1]);
         }
      }
   }

   // STEADY or TRANSIENT part

   if (isTransient == 0) {          // STEADY
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            VmomaCenter[j][I] = (VmomaWest[j][I] + VmomaEast[j][I] + VmomaSouth[j][I] + VmomaNorth[j][I] +
                                 VmomXConvFlux[j][I]   * (YJi[j+1][I]   - YJi[j][I]    ) -
                                 VmomXConvFlux[j][I-1] * (YJi[j+1][I-1] - YJi[j][I-1]  ) +
                                 VmomYConvFlux[j+1][I] * (XJi[j+1][I]   - XJi[j+1][I-1]) -
                                 VmomYConvFlux[j][I]   * (XJi[j][I]     - XJi[j][I-1]  ) -
                                 BndryCntrSrc[j][I] ) / vRelaxTM[j][I];

            VmomaSource[j][I] = (pJI[j][I] - pJI[j+1][I]) * (Xji[j][I]-Xji[j][I-1]) + BndryRHSSrc[j][I] +
                                (1 - vRelaxTM[j][I]) * VmomaCenter[j][I] * VjI[j][I];
         }
      }
   } else if (isTransient == 1)  {  // TRANSIENT
      for(int j=1; j<Numj-1; j++) {
         for(int I=1; I<NumI-1; I++) {
            VmomaCenter[j][I] = (VmomaWest[j][I] + VmomaEast[j][I] + VmomaSouth[j][I] + VmomaNorth[j][I] +
                                 VmomXConvFlux[j][I]   * (YJi[j+1][I]   - YJi[j][I]    ) -
                                 VmomXConvFlux[j][I-1] * (YJi[j+1][I-1] - YJi[j][I-1]  ) +
                                 VmomYConvFlux[j+1][I] * (XJi[j+1][I]   - XJi[j+1][I-1]) -
                                 VmomYConvFlux[j][I]   * (XJi[j][I]     - XJi[j][I-1]  )-
                                 BndryCntrSrc[j][I] ) + density * (YJI[j+1][I] - YJI[j][I]) * (Xji[j][I] - Xji[j][I-1]) / timeStep ;

            VmomaSource[j][I] = (pJI[j][I] - pJI[j+1][I]) * (Xji[j][I] - Xji[j][I-1]) + BndryRHSSrc[j][I]  +
                                 density * (YJI[j+1][I] - YJI[j][I]) * (Xji[j][I] - Xji[j][I-1]) / timeStep  * VjI[j][I];
         }
      }
   }


   // OUTLET BC (Part (3/4)
   for(int j=1; j<Numj-1; j++) {
      if (BoundaryRight[j][0] == 3) {
         VmomaCenter[j][NumI-2] = VmomaCenter[j][NumI-2] - VmomaEast[j][NumI-2];
         VmomaEast[j][NumI-2] = 0;
      }
      if (BoundaryLeft[j][0] == 3) {
         VmomaCenter[j][1] = VmomaCenter[j][1] - VmomaWest[j][1];
         VmomaWest[j][1] = 0;
      }
   }

   for(int I=1; I<NumI-1; I++) {
      if (BoundaryTop[I][0] == 3) {
         VmomaCenter[Numj-2][I] = VmomaCenter[Numj-2][I] - VmomaNorth[Numj-2][I];
         VmomaNorth[Numj-2][I] = 0;
      }
      if (BoundaryBottom[I][0] == 3) {
         VmomaCenter[1][I] = VmomaCenter[1][I] - VmomaSouth[1][I];
         VmomaSouth[1][I] = 0;
      }
   }

   MemoryDeAllocater2D(BndryCntrSrc, Numj, NumI);
   MemoryDeAllocater2D(BndryRHSSrc, Numj, NumI);

}  // End of function VmomMatrixFormer()
