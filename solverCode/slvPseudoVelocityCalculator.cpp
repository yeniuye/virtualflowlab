#include "slvFunctions.h"

// Pseudo velocities are required only for SIMPLER algorithm.




void pseudoUcalculator(double** UmomaWest, double** UmomaEast, double** UmomaSouth,
                       double** UmomaNorth, double** UmomaCenter, double** UmomaSource,
                       double** &UJi_pseudo, double** &UmomaSource_pseudo)
{
   for(int J=1;J<NumJ-1;J++) {  // Rows
      for(int i=1;i<Numi-1;i++) {  // Columns
         UmomaSource_pseudo[J][i] = UmomaSource[J][i] - ((pJI[J][i] - pJI[J][i+1]) * (Yji[J][i] - Yji[J-1][i]));
         
         UJi_pseudo[J][i] = (UmomaWest[J][i]  * UJi[J][i-1] +
                             UmomaEast[J][i]  * UJi[J][i+1] +
                             UmomaSouth[J][i] * UJi[J-1][i] +
                             UmomaNorth[J][i] * UJi[J+1][i] +
                             UmomaSource_pseudo[J][i]) / UmomaCenter[J][i];
      }
   } 
}




void pseudoVcalculator(double** VmomaWest, double** VmomaEast, double** VmomaSouth,
                       double** VmomaNorth, double** VmomaCenter, double** VmomaSource,
                       double** &VjI_pseudo, double** &VmomaSource_pseudo)
{
   for(int j=1;j<Numj-1;j++) {  // Rows
      for(int I=1;I<NumI-1;I++) {  // Columns
         VmomaSource_pseudo[j][I] = VmomaSource[j][I] - ((pJI[j][I]-pJI[j+1][I]) * (Xji[j][I]-Xji[j][I-1]));
         
         VjI_pseudo[j][I] = ( VmomaWest[j][I]  * VjI[j][I-1] +
                              VmomaEast[j][I]  * VjI[j][I+1] +
                              VmomaSouth[j][I] * VjI[j-1][I] +
                              VmomaNorth[j][I] * VjI[j+1][I] +
                              VmomaSource_pseudo[j][I] ) / VmomaCenter[j][I];
      }
   }
}  // End of function pseudoVcalculator()




void SourceModifierSimpler(double** UmomaSource, double** VmomaSource,
                           double** UmomaSource_pseudo, double** VmomaSource_pseudo)
{
   for(int J=1;J<NumJ-1;J++) {
      for(int i=1;i<Numi-1;i++) {
         UmomaSource[J][i] = (pJI[J][i]-pJI[J][i+1]) * (Yji[J][i]-Yji[J-1][i]) + UmomaSource_pseudo[J][i];
      }
   }
   
   for(int j=1;j<Numj-1;j++) {
      for(int I=1;I<NumI-1;I++) {
         VmomaSource[j][I] = (pJI[j][I]-pJI[j+1][I]) * (Xji[j][I]-Xji[j][I-1]) + VmomaSource_pseudo[j][I];
      }
   }
}  // End of function SourceModifierSimpler()
