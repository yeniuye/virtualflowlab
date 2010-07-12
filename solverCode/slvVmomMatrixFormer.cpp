/***************************************************************/
/*                 V MOMENTUM MATRIX FORMER                    */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* The function "VmomMatrixFormer" has many prameters and it forms 6 matrix  */
/* Each of these matrices keeps the constants in the V-momentum equation.    */
/* 4 of them is for neighbouring nodes, 1 for the V-cell of interest and 1   */
/* for the source terms. The memory is allocated  outside the function.      */
/*                                                                           */
/* IMPORTANT : The array size which is allocated in the main function is     */
/* larger than the needed size. Since some of the V-momentum cells are not   */
/* computational cells e.g. V cells at the west and east boundaries.         */
/* However these exstra memory allocation makes the indexing more reader     */
/* friendly.                                                                 */
/*                                                                           */
/* MOST IMPORTANT : The V-momentum relaxation is impilimented at this setp   */
/* in the formation of the matrices. The relaxation effects the center and   */
/* source matrices (below).                                                  */
/*****************************************************************************/

void VmomMatrixFormer(double** &VmomaWest,double** &VmomaEast,
double** &VmomaSouth,double** &VmomaNorth, double** &VmomaCenter,
double** &VmomaSource,int NumI,int Numj,double** VmomXConvFlux,
double** VmomYConvFlux,double** YJicoorSideFaces,double** XJicoorSideFaces,
double** XjicoorCorners,double** XjIcoorFrontFaces,
double** VmomXDiffCond,double** VmomYDiffCond,double** PJIpressure,
double** VjIvelocity,double Kinvis,int DiffSchm,double** TimeVRelaxPar,
double** BoundaryLeft,double** BoundaryRight,double** BoundaryTop,
double** BoundaryBottom,double Ro,double** YJIcoorCenter,double TimeStep,
bool isTransient,int nBlockCells, int** BlockCellCoor)
{
   /**************  IMPLIMENTATION OF BOUNDARY CONDITIONS ***********************/
   /* This part is the implimentation of boundary conditions on the V-momentum  */
   /* equation. The boundary effects are added to the system as source terms.   */
   /* For a solidwall (Not moving) the driving force is the shear stress and    */
   /* the effect of it is implimented as a source "BndryCntrSrc" to             */
   /* "UmomaCenter" term.                                                       */
   /* In the current problem we have solidwalls at the right and left side      */
   /* so source terms appear only at the center term. The top and bottom walls  */
   /* needs no special treatment. Since V-velocity is perpendicular to that     */
   /* walls                                                                     */
   /*****************************************************************************/

   double** BndryCntrSrc;
   BndryCntrSrc = MemoryAllocater2D(Numj,NumI);
   double** BndryRHSSrc;
   BndryRHSSrc = MemoryAllocater2D(Numj,NumI);

   /*  WALL BC (part 2/3)
   The following for loop is the related with the wall boundary condition
   it creates the necessary sources for the implimentation of wall boundary
   condition to the V-momentum equtation a similar procedure is followed in
   U-momentum equation
   */
   for(int j=1; j<Numj-1; j++) {
      if (BoundaryLeft[j][0]==1) {
         BndryCntrSrc[j][1] = -1 * Kinvis /(XjIcoorFrontFaces[j][1]-XjIcoorFrontFaces[j][0])
                              * (YJicoorSideFaces[j+1][0]-YJicoorSideFaces[j][0]);

         BndryRHSSrc[j][1] = Kinvis / (XjIcoorFrontFaces[j][1]-XjicoorCorners[j][0])
                             * (YJicoorSideFaces[j+1][0]-YJicoorSideFaces[j][0])* BoundaryLeft[j][1];
      }

      if (BoundaryRight[j][0]==1) {
         BndryCntrSrc[j][NumI-2] = -1 * Kinvis /(XjIcoorFrontFaces[j][NumI-1]-XjIcoorFrontFaces[j][NumI-2])
                                   * (YJicoorSideFaces[j+1][NumI-2]-YJicoorSideFaces[j][NumI-2]);

         BndryRHSSrc[j][NumI-2] = Kinvis / (XjicoorCorners[j][NumI-2]-XjIcoorFrontFaces[j][NumI-2])
                                  * (YJicoorSideFaces[j+1][NumI-2]-YJicoorSideFaces[j][NumI-2])* BoundaryRight[j][1];
      }
   }

   // BLOCK CELL CALCULATION
   for(int NumBlck=0; NumBlck<nBlockCells; NumBlck++) {
      for(int j=BlockCellCoor[NumBlck][0];j<BlockCellCoor[NumBlck][1]+1;j++) {
         for(int I=BlockCellCoor[NumBlck][2]+1;I<BlockCellCoor[NumBlck][3]+1;I++) {
            BndryCntrSrc[j][I] = -1 * pow(10.0,30);
            BndryRHSSrc[j][I]= 0;
         }
      }
   }



   /*****************************************************************************/
   /* This part calculates the coefficients as it is in the references listed   */
   /* below.It is important to realize that NOT ALL OF THE V-CELLS ARE          */
   /* COMPUTATIONAL CELLS                                                       */
   /*****************************************************************************/

   /**********************   CENTRAL DIFFERENCING ********************************/
   if (DiffSchm == 1) {
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            VmomaWest[j][I] = (VmomXDiffCond[j][I-1] + 0.5*VmomXConvFlux[j][I-1]) *
            (YJicoorSideFaces[j+1][I-1]-YJicoorSideFaces[j][I-1]);

            VmomaEast[j][I] = (VmomXDiffCond[j][I] - 0.5*VmomXConvFlux[j][I]) *
            (YJicoorSideFaces[j+1][I]-YJicoorSideFaces[j][I]);

            VmomaSouth[j][I] = (VmomYDiffCond[j][I] + 0.5*VmomYConvFlux[j][I]) *
            (XJicoorSideFaces[j][I]-XJicoorSideFaces[j][I-1]);

            VmomaNorth[j][I] = (VmomYDiffCond[j+1][I] - 0.5*VmomYConvFlux[j+1][I]) *
            (XJicoorSideFaces[j+1][I]-XJicoorSideFaces[j+1][I-1]);
         }
      }
   }
   /***********************   UPWIND DIFFERENCING ********************************/
   else if (DiffSchm == 2) {
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            VmomaWest[j][I] = (VmomXDiffCond[j][I-1] + max (VmomXConvFlux[j][I-1],0.0))*
            (YJicoorSideFaces[j+1][I-1]-YJicoorSideFaces[j][I-1]);

            VmomaEast[j][I] = (VmomXDiffCond[j][I] + max (0.0,-1*VmomXConvFlux[j][I]))*
            (YJicoorSideFaces[j+1][I]-YJicoorSideFaces[j][I]);

            VmomaSouth[j][I] = (VmomYDiffCond[j][I] + max (VmomYConvFlux[j][I],0.0))*
            (XJicoorSideFaces[j][I]-XJicoorSideFaces[j][I-1]);

            VmomaNorth[j][I] = (VmomYDiffCond[j+1][I]+max (0.0,-1*VmomYConvFlux[j+1][I]))*
            (XJicoorSideFaces[j+1][I]-XJicoorSideFaces[j+1][I-1]);
         }
      }
   }
   /**********************   HYBRID DIFFERENCING *********************************/
   else if (DiffSchm == 3) {
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            VmomaWest[j][I] = (max(max(VmomXConvFlux[j][I-1],0.0),
            (VmomXDiffCond[j][I-1]+0.5*VmomXConvFlux[j][I-1]))) *
            (YJicoorSideFaces[j+1][I-1]-YJicoorSideFaces[j][I-1]);

            VmomaEast[j][I] = (max(max(0.0,-1*VmomXConvFlux[j][I]),
            (VmomXDiffCond[j][I]-0.5 * VmomXConvFlux[j][I]))) *
            (YJicoorSideFaces[j+1][I]-YJicoorSideFaces[j][I]);

            VmomaSouth[j][I] = (max(max(VmomYConvFlux[j][I],0.0),
            (VmomYDiffCond[j][I]+0.5*VmomYConvFlux[j][I])))*
            (XJicoorSideFaces[j][I]-XJicoorSideFaces[j][I-1]);

            VmomaNorth[j][I] = (max(max(0.0,-1*VmomYConvFlux[j+1][I]),
            (VmomYDiffCond[j+1][I]-0.5*VmomYConvFlux[j+1][I]))) *
            (XJicoorSideFaces[j+1][I]-XJicoorSideFaces[j+1][I-1]);
         }
      }
   }
   /**********************   POWERLAW DIFFERENCING *******************************/
   else if (DiffSchm == 4) {
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            VmomaWest[j][I] = (VmomXDiffCond[j][I-1]*max(0.0,
            pow(1-0.1*fabs(VmomXConvFlux[j][I-1]/VmomXDiffCond[j][I-1]),5))
            + max(VmomXConvFlux[j][I-1],0.0))
            *(YJicoorSideFaces[j+1][I-1]-YJicoorSideFaces[j][I-1]);

            VmomaEast[j][I] = (VmomXDiffCond[j][I]*max(0.0,
            pow(1-0.1*fabs(VmomXConvFlux[j][I]/VmomXDiffCond[j][I]),5))
            + max(-1*VmomXConvFlux[j][I],0.0))
            * (YJicoorSideFaces[j+1][I]-YJicoorSideFaces[j][I]);

            VmomaSouth[j][I] = (VmomYDiffCond[j][I]*max(0.0,
            pow(1-0.1*fabs(VmomYConvFlux[j][I]/VmomYDiffCond[j][I]),5))
            + max(VmomYConvFlux[j][I],0.0))
            * (XJicoorSideFaces[j][I]-XJicoorSideFaces[j][I-1]);

            VmomaNorth[j][I] = (VmomYDiffCond[j+1][I]* max(0.0,
            pow(1-0.1*fabs(VmomYConvFlux[j+1][I]/VmomYDiffCond[j+1][I]),5))
            + max (-1*VmomYConvFlux[j+1][I],0.0))
            * (XJicoorSideFaces[j+1][I]-XJicoorSideFaces[j+1][I-1]);
         }
      }
   }


   // The STEADY or TRANSIENT part
   if (isTransient == 0) {      // STEADY
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            VmomaCenter[j][I] =
            (VmomaWest[j][I]+VmomaEast[j][I]+VmomaSouth[j][I]+VmomaNorth[j][I]+
            VmomXConvFlux[j][I]*(YJicoorSideFaces[j+1][I]-YJicoorSideFaces[j][I])-
            VmomXConvFlux[j][I-1]*(YJicoorSideFaces[j+1][I-1]-YJicoorSideFaces[j][I-1])+
            VmomYConvFlux[j+1][I]*(XJicoorSideFaces[j+1][I]-XJicoorSideFaces[j+1][I-1])-
            VmomYConvFlux[j][I]*(XJicoorSideFaces[j][I]-XJicoorSideFaces[j][I-1])
            -BndryCntrSrc[j][I] ) / TimeVRelaxPar[j][I];

            VmomaSource[j][I] = (PJIpressure[j][I]-PJIpressure[j+1][I])*
            (XjicoorCorners[j][I]-XjicoorCorners[j][I-1]) + BndryRHSSrc [j][I] +
            (1-TimeVRelaxPar[j][I])* VmomaCenter[j][I] * VjIvelocity [j][I];
         }
      }
   } else if (isTransient == 1)  {  // TRANSIENT
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            VmomaCenter[j][I] =
            (VmomaWest[j][I]+VmomaEast[j][I]+VmomaSouth[j][I]+VmomaNorth[j][I]+
            VmomXConvFlux[j][I]*(YJicoorSideFaces[j+1][I]-YJicoorSideFaces[j][I])-
            VmomXConvFlux[j][I-1]*(YJicoorSideFaces[j+1][I-1]-YJicoorSideFaces[j][I-1])+
            VmomYConvFlux[j+1][I]*(XJicoorSideFaces[j+1][I]-XJicoorSideFaces[j+1][I-1])-
            VmomYConvFlux[j][I]*(XJicoorSideFaces[j][I]-XJicoorSideFaces[j][I-1])
            -BndryCntrSrc[j][I] )   +
            Ro * (YJIcoorCenter[j+1][I]- YJIcoorCenter[j][I]) *
            (XjicoorCorners[j][I]-XjicoorCorners[j][I-1]) / TimeStep ;

            VmomaSource[j][I] = (PJIpressure[j][I]-PJIpressure[j+1][I])*
            (XjicoorCorners[j][I]-XjicoorCorners[j][I-1]) + BndryRHSSrc [j][I]  +
            Ro * (YJIcoorCenter[j+1][I]- YJIcoorCenter[j][I]) *
            (XjicoorCorners[j][I]-XjicoorCorners[j][I-1]) / TimeStep  * VjIvelocity [j][I];
         }
      }
   }


   // OUTLET BC (part (3/4)
   for(int j=1;j<Numj-1;j++) {
      if (BoundaryRight[j][0]==3) {
         VmomaCenter[j][NumI-2] = VmomaCenter[j][NumI-2] - VmomaEast[j][NumI-2];
         VmomaEast[j][NumI-2] = 0;
      }
      if (BoundaryLeft[j][0]==3) {
         VmomaCenter[j][1] = VmomaCenter[j][1] - VmomaWest[j][1];
         VmomaWest[j][1] = 0;
      }
   }

   for(int I=1;I<NumI-1;I++) {
      if (BoundaryTop[I][0]==3) {
         VmomaCenter[Numj-2][I] = VmomaCenter[Numj-2][I] - VmomaNorth[Numj-2][I];
         VmomaNorth[Numj-2][I] = 0;
      }
      if (BoundaryBottom[I][0]==3) {
         VmomaCenter[1][I] = VmomaCenter[1][I] - VmomaSouth[1][I];
         VmomaSouth[1][I] = 0;
      }
   }
   // End of Far Field Boundary Condition

   MemoryDeAllocater2D(BndryCntrSrc,Numj,NumI);
   MemoryDeAllocater2D(BndryRHSSrc,Numj,NumI);

}  // End of function VmomMatrixFormer()

