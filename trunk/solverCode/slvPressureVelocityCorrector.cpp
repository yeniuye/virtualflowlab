#include "slvFunctions.h"




void PressureCorrector(double** &PJICorrect)
{
   /*****************************************************************************
    This function updates (corrects) pressure values (with relaxation).

    Initial pressure correction values for each iteration should be zero.
    However, VelocityCorrecter function also use PJICorrect values, so
    "initializing to zero" part is done in VelocityCorrecter function.

    Pressure relaxation is a value between 0 and 1.
   *****************************************************************************/

   for(int J=1; J<NumJ-1; J++) {  // ROW
      for(int I=1; I<NumI-1; I++) {  // COLUMN
         pJI[J][I] = pJI[J][I] + (pRelax) * PJICorrect[J][I];
      }
   }
}  // End of function PressureCorrector()




void VelocityCorrector(double** PJICorrect, double** PresCrctdJi, double** PresCrctdjI)
{
   /*****************************************************************************
    This function 
    1) corrects both u and v velocities using pressure correction values and 
       PresCrctdJi and PresCrctdjI arrays that were calculated in
       PressureCorrectionMatrix function.
    2) sets pressure correction values to zero for the next iteration

    IMPORTANT : Velocity relaxations were already implimented in
    UmomMatrixFormer and VmomMatrixFormer functions.
   *****************************************************************************/

   // u velocity correction
   for(int J=1; J<NumJ-1; J++) {
      for(int i=1; i<Numi-1; i++) {
         UJi[J][i] = UJi[J][i] + PresCrctdJi[J][i] * (PJICorrect[J][i] - PJICorrect[J][i+1]);
      }
   }

   // v velocity correction
   for(int j=1; j<Numj-1; j++) {
      for(int I=1; I<NumI-1; I++) {
         VjI[j][I] = VjI[j][I] + PresCrctdjI[j][I] * (PJICorrect[j][I] - PJICorrect[j+1][I]);
      }
   }

   // Set pressure correction to zero for the next iteration
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         PJICorrect[J][I] = 0.0;
      }
   }
}  // End of function VelocityCorrector()
