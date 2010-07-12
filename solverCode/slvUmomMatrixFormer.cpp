/***************************************************************/
/*                 U MOMENTUM MATRIX FORMER                    */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* The function "UmomMatrixFormer" has many prameters and it forms 6 matrix  */
/* Each of these matrices keeps the constants in the U-momentum equation.    */
/* 4 of them is for neighbouring nodes, 1 for the U-cell of interest and 1   */
/* for the source terms. The memory is allocated  outside the function.      */
/*                                                                           */
/* IMPORTANT : The array size which is allocated in the main function is     */
/* larger than the needed size. Since some of the U-momentum cells are not   */
/* computational cells e.g. U cells at the west and east boundaries.         */
/* However these exstra memory allocation makes the indexing more reader     */
/* friendly.                                                                 */
/*                                                                           */
/* MOST IMPORTANT : The U-momentum relaxation is impilimented at this setp   */
/* in the formation of the matrices. The relaxation effects the center and   */
/* source matrices (below).                                                  */
/*****************************************************************************/

void UmomMatrixFormer(double** &UmomaWest,double** &UmomaEast,
double** &UmomaSouth,double** &UmomaNorth, double** &UmomaCenter,
double** &UmomaSource,int Numi,int NumJ,double** UmomXConvFlux,
double** UmomYConvFlux,double** YjIcoorFrontFaces,double** XjIcoorFrontFaces,
double** YjicoorCorners, double** YJicoorSideFaces,
double** UmomXDiffCond,double** UmomYDiffCond,double** PJIpressure,
double** UJivelocity,double Kinvis,int DiffSchm,
double** TimeURelaxPar,double** BoundaryLeft,double** BoundaryRight,double** BoundaryTop,
double** BoundaryBottom,double Ro,double** XJIcoorCenter,double TimeStep,
bool isTransient,int nBlockCells, int** BlockCellCoor)
{
   /**************  IMPLIMENTATION OF BOUNDARY CONDITIONS ***********************/
   /* This part is the implimentation of boundary conditions on the u-momentum  */
   /* equation. The boundary effects are added to the system as source terms.   */
   /* For a solidwall (Not moving) the driving force is the shear stress and    */
   /* the effect of it is implimented as a source "BndryCntrSrc" to             */
   /* "UmomaCenter" term.                                                       */
   /* The moving wall has two source terms one is common with the solid wall    */
   /* and explained above. The other source term "BndryRHSSrc" is added to the  */
   /* "UmomaSource" term.                                                       */
   /* In the current problem we have a solidwall at the bottom and a movig wall */
   /* at the top so the boundary conditions are developed for this case below.  */
   /* Since it is U-momentum equation the right and left walls do not need any  */
   /* special treatment because u velocity is perpendicular to the walls, the   */
   /* idea is explained in detail in the sheets.                                */
   /*****************************************************************************/

   double** BndryCntrSrc;
   BndryCntrSrc = MemoryAllocater2D(NumJ,Numi);
   double** BndryRHSSrc;
   BndryRHSSrc = MemoryAllocater2D(NumJ,Numi);


   /* WALL BC (part 1/3)
   The following for loop is the related with the wall boundary condition
   it creates the necessary sources for the implimentation of wall boundary
   condition to the U-momentum equtation a similar procedure is followed in
   V-momentum equation */

   for(int i=1;i<Numi-1;i++) {
      if (BoundaryBottom[i][0]==1) {
         BndryCntrSrc[1][i]= -1 * Kinvis /(YJicoorSideFaces[1][i]-YJicoorSideFaces[0][i])
         * (XjIcoorFrontFaces[0][i+1]-XjIcoorFrontFaces[0][i]);

         BndryRHSSrc[1][i] = Kinvis /(YJicoorSideFaces[1][i]-YjicoorCorners[0][i])
         * (XjIcoorFrontFaces[0][i+1]-XjIcoorFrontFaces[0][i])* BoundaryBottom[i][1];
      }


      if (BoundaryTop[i][0]==1) {
         BndryCntrSrc[NumJ-2][i] = -1 * Kinvis /(YJicoorSideFaces[NumJ-1][i]-YJicoorSideFaces[NumJ-2][i])
         * (XjIcoorFrontFaces[NumJ-2][i+1]-XjIcoorFrontFaces[NumJ-2][i]);

         BndryRHSSrc[NumJ-2][i] = Kinvis /(YjicoorCorners[NumJ-2][i]-YJicoorSideFaces[NumJ-2][i])
         * (XjIcoorFrontFaces[NumJ-2][i+1]-XjIcoorFrontFaces[NumJ-2][i])* BoundaryTop[i][1];
      }
   }

   // BLOCK CELL CALCULATION
   for(int NumBlck=0; NumBlck<nBlockCells; NumBlck++) {
      for(int J=BlockCellCoor[NumBlck][0]+1;J<BlockCellCoor[NumBlck][1]+1;J++) {
         for(int i=BlockCellCoor[NumBlck][2];i<BlockCellCoor[NumBlck][3]+1;i++) {
            BndryCntrSrc[J][i] = -1 * pow(10.0,30);
            BndryRHSSrc[J][i]= 0;
         }
      }
   }

   /*****************************************************************************/
   /* This part calculates the coefficients as it is in the references listed   */
   /* below.It is important to realize that NOT ALL OF THE U-CELLS ARE          */
   /* COMPUTATIONAL CELLS                                                       */
   /*****************************************************************************/

   /************************   CENTRAL DIFFERENCING ******************************/
   if (DiffSchm == 1) {
      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
            UmomaWest[J][i] = (UmomXDiffCond[J][i] + 0.5*UmomXConvFlux[J][i]) *
            (YjIcoorFrontFaces[J][i]-YjIcoorFrontFaces[J-1][i]);

            UmomaEast[J][i] = (UmomXDiffCond[J][i+1] - 0.5*UmomXConvFlux[J][i+1]) *
            (YjIcoorFrontFaces[J][i+1]-YjIcoorFrontFaces[J-1][i+1]);

            UmomaSouth[J][i] = (UmomYDiffCond[J-1][i] + 0.5*UmomYConvFlux[J-1][i]) *
            (XjIcoorFrontFaces[J-1][i+1]-XjIcoorFrontFaces[J-1][i]);

            UmomaNorth[J][i] = (UmomYDiffCond[J][i] - 0.5*UmomYConvFlux[J][i]) *
            (XjIcoorFrontFaces[J][i+1]-XjIcoorFrontFaces[J][i]);
         }
      }
   }
   /***********************   UPWIND DIFFERENCING ********************************/
   else if (DiffSchm == 2) {
      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
            UmomaWest[J][i] =(UmomXDiffCond[J][i] + max (UmomXConvFlux[J][i],0.0))*
            (YjIcoorFrontFaces[J][i]-YjIcoorFrontFaces[J-1][i]);

            UmomaEast[J][i] =(UmomXDiffCond[J][i+1] + max (0.0,-1*UmomXConvFlux[J][i+1]))*
            (YjIcoorFrontFaces[J][i+1]-YjIcoorFrontFaces[J-1][i+1]);

            UmomaSouth[J][i] =(UmomYDiffCond[J-1][i] + max (UmomYConvFlux[J-1][i],0.0))*
            (XjIcoorFrontFaces[J-1][i+1]-XjIcoorFrontFaces[J-1][i]);

            UmomaNorth[J][i] =(UmomYDiffCond[J][i] + max (0.0,-1*UmomYConvFlux[J][i]))*
            (XjIcoorFrontFaces[J][i+1]-XjIcoorFrontFaces[J][i]);
         }
      }
   }
   /**********************   HYBRID DIFFERENCING *********************************/
   else if (DiffSchm == 3) {
      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
            UmomaWest[J][i] = (max(max(UmomXConvFlux[J][i],0.0),
            (UmomXDiffCond[J][i]+0.5*UmomXConvFlux[J][i])))*
            (YjIcoorFrontFaces[J][i]-YjIcoorFrontFaces[J-1][i]);

            UmomaEast[J][i] = (max(max(-1*UmomXConvFlux[J][i+1],0.0),
            (UmomXDiffCond[J][i+1]-0.5*UmomXConvFlux[J][i+1])))*
            (YjIcoorFrontFaces[J][i+1]-YjIcoorFrontFaces[J-1][i+1]);

            UmomaSouth[J][i] = (max(max(UmomYConvFlux[J-1][i],0.0),
            (UmomYDiffCond[J-1][i]+0.5*UmomYConvFlux[J-1][i])))*
            (XjIcoorFrontFaces[J-1][i+1]-XjIcoorFrontFaces[J-1][i]);

            UmomaNorth[J][i] = (max(max(-1*UmomYConvFlux[J][i],0.0),
            (UmomYDiffCond[J][i]-0.5*UmomYConvFlux[J][i])))*
            (XjIcoorFrontFaces[J][i+1]-XjIcoorFrontFaces[J][i]);
         }
      }
   }
   /**********************   POWERLAW DIFFERENCING *******************************/
   else if (DiffSchm == 4) {
      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
            UmomaWest[J][i] = (UmomXDiffCond[J][i] * max(0.0,
            pow(1-0.1*fabs(UmomXConvFlux[J][i]/UmomXDiffCond[J][i]),5))
            + max(UmomXConvFlux[J][i],0.0))
            *(YjIcoorFrontFaces[J][i]-YjIcoorFrontFaces[J-1][i]);

            UmomaEast[J][i] = (UmomXDiffCond[J][i+1] * max (0.0,
            pow(1-0.1*fabs(UmomXConvFlux[J][i+1]/UmomXDiffCond[J][i+1]),5))
            + max(-1*UmomXConvFlux[J][i+1],0.0))
            *(YjIcoorFrontFaces[J][i+1]-YjIcoorFrontFaces[J-1][i+1]);

            UmomaSouth[J][i] = (UmomYDiffCond[J-1][i] * max(0.0,
            pow(1-0.1*fabs(UmomYConvFlux[J-1][i]/UmomYDiffCond[J-1][i]),5))
            + max(UmomYConvFlux[J-1][i],0.0))
            *(XjIcoorFrontFaces[J-1][i+1]-XjIcoorFrontFaces[J-1][i]);

            UmomaNorth[J][i] = (UmomYDiffCond[J][i] * max(0.0,
            pow(1 - 0.1*fabs(UmomYConvFlux[J][i]/UmomYDiffCond[J][i]),5))
            + max(-1*UmomYConvFlux[J][i],0.0))
            *(XjIcoorFrontFaces[J][i+1]-XjIcoorFrontFaces[J][i]);
         }
      }
   }

   // The STEADY or TRANSIENT part

   if (isTransient == 0) {      // STEADY
      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
            UmomaCenter[J][i] =
            (UmomaWest[J][i]+UmomaEast[J][i]+UmomaSouth[J][i]+UmomaNorth[J][i]+
            UmomXConvFlux[J][i+1]*(YjIcoorFrontFaces[J][i+1]-YjIcoorFrontFaces[J-1][i+1])-
            UmomXConvFlux[J][i]*(YjIcoorFrontFaces[J][i]-YjIcoorFrontFaces[J-1][i])+
            UmomYConvFlux[J][i]*(XjIcoorFrontFaces[J][i+1]-XjIcoorFrontFaces[J][i])-
            UmomYConvFlux[J-1][i]*(XjIcoorFrontFaces[J-1][i+1]-XjIcoorFrontFaces[J-1][i])
            -BndryCntrSrc[J][i] ) / TimeURelaxPar[J][i];

            UmomaSource[J][i] = (PJIpressure[J][i]-PJIpressure[J][i+1])*
            (YjicoorCorners[J][i]-YjicoorCorners[J-1][i]) + BndryRHSSrc[J][i] +
            (1-TimeURelaxPar[J][i])* UmomaCenter[J][i] * UJivelocity[J][i];
         }
      }
   } else if (isTransient == 1) {    // TRANSIENT
      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
            UmomaCenter[J][i] =
            (UmomaWest[J][i]+UmomaEast[J][i]+UmomaSouth[J][i]+UmomaNorth[J][i]+
            UmomXConvFlux[J][i+1]*(YjIcoorFrontFaces[J][i+1]-YjIcoorFrontFaces[J-1][i+1])-
            UmomXConvFlux[J][i]*(YjIcoorFrontFaces[J][i]-YjIcoorFrontFaces[J-1][i])+
            UmomYConvFlux[J][i]*(XjIcoorFrontFaces[J][i+1]-XjIcoorFrontFaces[J][i])-
            UmomYConvFlux[J-1][i]*(XjIcoorFrontFaces[J-1][i+1]-XjIcoorFrontFaces[J-1][i])
            -BndryCntrSrc[J][i] ) +
            Ro * (YjicoorCorners[J][i]-YjicoorCorners[J-1][i]) *
            (XJIcoorCenter[J][i+1] - XJIcoorCenter[J][i]) / TimeStep;

            UmomaSource[J][i] = (PJIpressure[J][i]-PJIpressure[J][i+1])*
            (YjicoorCorners[J][i]-YjicoorCorners[J-1][i]) + BndryRHSSrc[J][i] +
            Ro * (YjicoorCorners[J][i]-YjicoorCorners[J-1][i]) *
            (XJIcoorCenter[J][i+1] - XJIcoorCenter[J][i]) / TimeStep * UJivelocity[J][i];
         }
      }
   }


   // OUTLET BC (part (2/4)
   for(int J=1;J<NumJ-1;J++) {
      if (BoundaryRight[J][0]==3) {
         UmomaCenter[J][Numi-2] = UmomaCenter[J][Numi-2] - UmomaEast[J][Numi-2];
         UmomaEast[J][Numi-2]=0;
      }
      if (BoundaryLeft[J][0]==3) {
         UmomaCenter[J][1] = UmomaCenter[J][1] - UmomaWest[J][1];
         UmomaWest[J][1]=0;
      }
   }

   for(int i=1;i<Numi-1;i++) {
      if (BoundaryTop[i][0]==3) {
         UmomaCenter[NumJ-2][i] = UmomaCenter[NumJ-2][i] - UmomaNorth[NumJ-2][i];
         UmomaNorth[NumJ-2][i] = 0;
      }
      if (BoundaryBottom[i][0]==3) {
         UmomaCenter[1][i] = UmomaCenter[1][i] - UmomaSouth[1][i];
         UmomaSouth[1][i] = 0;
      }
   }

   MemoryDeAllocater2D(BndryCntrSrc,NumJ,Numi);
   MemoryDeAllocater2D(BndryRHSSrc,NumJ,Numi);

}  // End of function UmomMatrixFormer()

