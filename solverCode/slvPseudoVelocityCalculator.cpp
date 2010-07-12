/***************************************************************/
/*                  Pseudo Velocity Calculator                 */
/***************************************************************/

#include "slvFunctions.h"

void PseudoUcalculator(double** UmomaWest,double** UmomaEast,
double** UmomaSouth,double** UmomaNorth, double** UmomaCenter,
double** UmomaSource,int Numi,int NumJ,double** PJIpressure,
double** YjicoorCorners,double** UJivelocity,double** &UJi_pse_vel,
double** &UmomaSource_Pseu)
{
   for(int J=1;J<NumJ-1;J++) {  // Rows
   	for(int i=1;i<Numi-1;i++) {  // Columns
         UmomaSource_Pseu[J][i] = UmomaSource[J][i] -
    		((PJIpressure[J][i]-PJIpressure[J][i+1]) *        // inefficient
    		(YjicoorCorners[J][i]-YjicoorCorners[J-1][i]) );
         
         UJi_pse_vel[J][i] = (UmomaWest[J][i] * UJivelocity[J][i-1]
                              + UmomaEast[J][i] * UJivelocity[J][i+1]
                              + UmomaSouth[J][i] * UJivelocity[J-1][i]
                              + UmomaNorth[J][i] * UJivelocity[J+1][i]
                              + UmomaSource_Pseu[J][i]) / UmomaCenter[J][i];
      }
   } 
}  // End of function PseudoUcalculator()


void PseudoVcalculator(double** VmomaWest,double** VmomaEast,
double** VmomaSouth,double** VmomaNorth, double** VmomaCenter,
double** VmomaSource,int NumI,int Numj,double** PJIpressure,
double** XjicoorCorners,double** VjIvelocity,double** &VjI_pse_vel,
double** &VmomaSource_Pseu)
{
   for(int j=1;j<Numj-1;j++) {  // Rows
    	for(int I=1;I<NumI-1;I++) {  // Columns
         VmomaSource_Pseu[j][I] = VmomaSource[j][I] -
         ((PJIpressure[j][I]-PJIpressure[j+1][I])*
         (XjicoorCorners[j][I]-XjicoorCorners[j][I-1])  );
         
         VjI_pse_vel[j][I] = ( VmomaWest[j][I] * VjIvelocity[j][I-1]
                               + VmomaEast[j][I] * VjIvelocity[j][I+1]
                               + VmomaSouth[j][I] * VjIvelocity[j-1][I]
                               + VmomaNorth[j][I] * VjIvelocity[j+1][I]
                               + VmomaSource_Pseu[j][I] ) / VmomaCenter[j][I];
      }
   }
}  // End of function PseudoVcalculator()


void SourceModifierSimpler(double** UmomaSource,double** VmomaSource,
double** PJIpressure,double**YjicoorCorners,double** XjicoorCorners,
int Numi,int Numj,int NumI,int NumJ,double** UmomaSource_Pseu,
double** VmomaSource_Pseu)
{
   for(int J=1;J<NumJ-1;J++) {
      for(int i=1;i<Numi-1;i++) {
         UmomaSource[J][i] = (PJIpressure[J][i]-PJIpressure[J][i+1])*
         (YjicoorCorners[J][i]-YjicoorCorners[J-1][i]) + UmomaSource_Pseu[J][i];
      }
   }
   
   for(int j=1;j<Numj-1;j++) {
      for(int I=1;I<NumI-1;I++) {
         VmomaSource[j][I] = (PJIpressure[j][I]-PJIpressure[j+1][I])*
         (XjicoorCorners[j][I]-XjicoorCorners[j][I-1]) + VmomaSource_Pseu[j][I];
      }
   }
}  // End of function SourceModifierSimpler()

