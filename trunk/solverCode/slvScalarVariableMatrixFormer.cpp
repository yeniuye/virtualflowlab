#include "slvFunctions.h"




void ScalarVarMatrixFormer(double** &ScalaraWest, double** &ScalaraEast, double** &ScalaraSouth,
                           double** &ScalaraNorth, double** &ScalaraCenter, double** &ScalaraSource,
                           double** ScalarXConvFlux, double** ScalarYConvFlux, double** ScalarXDiffCond,
                           double** ScalarYDiffCond, double** ScalarVar, double ThermalConduct,
                           double** scalarRelaxTM)
{
   // Note that the scalar variable is not fully solved in the current version of the code.
   // This function is NOT tested fully.
   
   double** BndryCntrSrc;
   double** BndryRHSSrc;
   BndryCntrSrc = MemoryAllocater2D(NumJ,NumI);
   BndryRHSSrc = MemoryAllocater2D(NumJ,NumI);

   for(int I=1; I<NumI-1; I++) {
      //if (BoundaryBottom[i][0]==1) {
         BndryCntrSrc[1][I] = -1 * ThermalConduct / (YJI[1][I] - YJI[0][I]) * (Xji[0][I] - Xji[0][I-1]);

         BndryRHSSrc[1][I] = ThermalConduct / (YJI[1][I] - YjI[0][I]) * (Xji[0][I] - Xji[0][I-1]) * 5;

         // BndryRHSSrc[1][I] = ThermalConduct / (YJI[1][I] - YjI[0][I]) * (Xji[0][I]-Xji[0][I-1]) * BoundaryBottom[I][1];
      //}

      //if (BoundaryTop[i][0]==1) {
         BndryCntrSrc[NumJ-2][I] = -1 * ThermalConduct / (YJI[NumJ-1][I] - YJI[NumJ-2][I]) * (Xji[NumJ-2][I] - Xji[NumJ-2][I-1]);

         BndryRHSSrc[NumJ-2][I] = ThermalConduct / (YjI[NumJ-2][I] - YJI[NumJ-2][I]) * (Xji[NumJ-2][I] - Xji[NumJ-2][I-1]) * 0;

         // BndryRHSSrc[NumJ-2][I] = ThermalConduct / (YjI[NumJ-2][I] - YJI[NumJ-2][I]) * (Xji[NumJ-2][I] - Xji[NumJ-2][I-1]) * BoundaryTop[I][1];
      //}
   }

   for(int J=1; J<NumJ-1; J++) {
      //if (BoundaryLeft[j][0]==1) {
         BndryCntrSrc[J][1] = -1 * ThermalConduct / (XJI[J][1]-XJI[J][0]) * (Yji[J][0]-Yji[J-1][0]);

         BndryRHSSrc[J][1] = ThermalConduct / (XJI[J][1]-XJi[J][0]) * (Yji[J][0]-Yji[J-1][0]) * 0; //BoundaryLeft[J][1];
      //}

      //if (BoundaryRight[j][0]==1) {
      //   BndryCntrSrc[J][NumI-2] = -1 * ThermalConduct / (XJI[J][NumI-1]-XJI[J][NumI-2]) * (Yji[J][NumI-2]-Yji[J-1][NumI-2]);
      //   BndryRHSSrc[J][NumI-2] = ThermalConduct / (XJi[J][NumI-2]-XJI[J][NumI-2]) * (Yji[J][NumI-2]-Yji[J-1][NumI-2]) * 0; //BoundaryRight[J][1];
      //}
   } 

   //if (discSchm == 1) {
      for(int J=1; J<NumJ-1; J++) {
         for(int I=1; I<NumI-1; I++) {
            ScalaraWest[J][I] = (ScalarXDiffCond[J][I-1] + 0.5 * ScalarXConvFlux[J][I-1]) * (Yji[J][I-1]-Yji[J-1][I-1]);

            ScalaraEast[J][I] = (ScalarXDiffCond[J][I] - 0.5 * ScalarXConvFlux[J][I]) * (Yji[J][I]-Yji[J-1][I]);

            ScalaraSouth[J][I] = (ScalarYDiffCond[J-1][I] + 0.5 * ScalarYConvFlux[J-1][I]) * (Xji[J-1][I]-Xji[J-1][I-1]);

            ScalaraNorth[J][I] = (ScalarYDiffCond[J][I] - 0.5 * ScalarYConvFlux[J][I]) * (Xji[J][I]-Xji[J][I-1]);

            ScalaraCenter[J][I] = (ScalaraWest[J][I] + ScalaraEast[J][I] + ScalaraSouth[J][I] + ScalaraNorth[J][I] +
                                   ScalarXConvFlux[J][I]   * (Yji[J][I]   - Yji[J-1][I]) -
                                   ScalarXConvFlux[J][I-1] * (Yji[J][I-1] - Yji[J-1][I-1])+
                                   ScalarYConvFlux[J][I]   * (Xji[J][I]   - Xji[J][I-1])-
                                   ScalarYConvFlux[J-1][I] * (Xji[J-1][I] - Xji[J-1][I-1]) -
                                   BndryCntrSrc[J][I] ) / scalarRelaxTM[J][I];

            ScalaraSource[J][I] = BndryRHSSrc[J][I] + (1 - scalarRelaxTM[J][I]) * ScalaraCenter[J][I] * ScalarVar[J][I];
         }
      }
   //}

   // Outlet BC for scalar variable (Part 2/2)
   for(int J=1; J<NumJ-1; J++) {
      //if (BoundaryRight[J][0]==3) {
         ScalaraCenter[J][NumI-2] = ScalaraCenter[J][NumI-2] - ScalaraEast[J][NumI-2];
         ScalaraEast[J][NumI-2]=0;
      //}
      //if (BoundaryLeft[J][0]==3) {
         //UmomaCenter[J][1] = UmomaCenter[J][1] - UmomaWest[J][1];
         //UmomaWest[J][1]=0;
      //}
   }

   MemoryDeAllocater2D(BndryCntrSrc, NumJ, NumI);
   MemoryDeAllocater2D(BndryRHSSrc, NumJ, NumI);
}
