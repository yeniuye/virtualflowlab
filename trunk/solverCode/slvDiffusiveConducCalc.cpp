#include "slvFunctions.h"




void DiffConducCalcUmom(double** &UmomXDiffCond,double** &UmomYDiffCond)
{
   /*****************************************************************************
    This function calculates diffusive conductance at u-momemtum cell faces.
    The diffusion conductance is kept at two different arrays one for West
    and East faces and the other for South and North faces.

    IMPORTANT : The arrays used to store diffusive conductance are allocated
    larger than the required size. Actually they are not calculated at all
    [J][I] and [j][i] faces but using larger arrays make the code more readable.
   *****************************************************************************/

   // Calculation at cell faces [J][I] (x-direction)
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         UmomXDiffCond[J][I] = kinvis / (XJi[J][I] - XJi[J][I-1]);
      }
   }

   // Calculation at cell faces [j][i] (y-direction)
   for(int j=0; j<Numj; j++) {
      for(int i=1; i<Numi-1; i++) {
         UmomYDiffCond[j][i] = kinvis / (YJi[j+1][i] - YJi[j][i]);
      }
   }
}  // End of function DiffConducCalcUmom()




void DiffConducCalcVmom(double** &VmomXDiffCond,double** &VmomYDiffCond)
{
   /*****************************************************************************
    This function calculates diffusive conductance at v-momemtum cell faces in
    a way similar to the previous function.
   *****************************************************************************/

   // Calculation at cell faces [j][i] (x-direction)
   for(int j=1; j<Numj-1; j++) {
      for(int i=0; i<Numi; i++) {
         VmomXDiffCond[j][i] = kinvis / (XjI[j][i+1] - XjI[j][i]);
      }
   }
   
   // Calculation at cell faces [J][I] (y-direction)
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         VmomYDiffCond[J][I] = kinvis / (YjI[J][I] - YjI[J-1][I]);
      }
   }
}  // End of function DiffConducCalcVmom()




void DiffConducCalcScalar(double** &ScalarXDiffCond,double** &ScalarYDiffCond,double ThermalConduct)
{
   /******************************************************************************
    This function calculates the diffusive conductance for the scalar variable.

    It is added at 08 Jan 2007.
   ******************************************************************************/

   // Calculation at cell faces [J][i] (x-direction)
   for(int J=1; J<NumJ-1; J++) {
      for(int i=0; i<Numi; i++) {
         ScalarXDiffCond[J][i] = ThermalConduct / (XJI[J][i+1] - XJI[J][i]);
      }
   }

   // Calculation at cell faces [j][I] (x-direction)
   for(int j=0; j<Numj; j++) {
      for(int I=1; I<NumI-1; I++) {
         ScalarYDiffCond[j][I] = ThermalConduct / (YJI[j+1][I] - YJI[j][I]);
      }
   }

}  // End of function DiffConducCalcScalar()
