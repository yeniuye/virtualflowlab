#include "slvFunctions.h"




void ConvFluxCalcUmom(double** &UmomXConvFlux, double** &UmomYConvFlux)
{
   /*****************************************************************************
    This function calculates mass fluxes at cell faces (x-faces [J][I] and
    y-faces [j][i]) of the u-momentum cells. Calculated fluxes are stored
    in two arrays, one for the x direction and one for the y direction.

    IMPORTANT : The arrays storing convective fluxes are allocated larger
    than the required size. Actually mass flux is not calculated at all [J][I]
    and [j][i] faces but using larger arrays make the code more readable.
   *****************************************************************************/

   // Mass flux for cell faces [J][I] in x-direction
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         UmomXConvFlux[J][I] = 0.5 * density * (UJi[J][I-1] + UJi[J][I]);
      }
   }

   // Mass flux for cell faces [j][i] in y-direction
   for(int j=0; j<Numj; j++) {
      for(int i=1; i<Numi-1; i++) {
         UmomYConvFlux[j][i] = 0.5 * density * (VjI[j][i] + VjI[j][i+1]);
      }
   }

}  // End of function ConvFluxCalcUmom()




void ConvFluxCalcVmom(double** &VmomXConvFlux, double** &VmomYConvFlux)
{
   /*****************************************************************************
    This function calculates mass fluxes at cell faces (x-faces [j][i] and
    y-faces [J][I]) of the v-momentum cells. Calculated fluxes are stored
    in two arrays, one for the x direction and one for the y direction.

    Same memory allocation warning applies as the previous function.
   *****************************************************************************/


   // Mass flux for cell faces [j][i] in x-direction
   for(int j=1; j<Numj-1; j++) {
      for(int i=0; i<Numi; i++) {
         VmomXConvFlux[j][i] = 0.5 * density * (UJi[j][i] + UJi[j+1][i]);
      }
   }

   // Mass flux for cell faces [J][I] in y-direction
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         VmomYConvFlux[J][I] = 0.5 * density * (VjI[J][I] + VjI[J-1][I]);
      }
   }

}  // End of function ConvFluxCalcVmom()




void ConvFluxCalcScalar(double** &ScalarXConvFlux, double** &ScalarYConvFlux)
{
   /****************************************************************************
    This function calculates convective fluxes for the scalar variable.
    Fortunatelly, staggered grid errangement enables us to do this calculation
    without making any interpolation.
    
    This function is added at 08 Jan 2007.
   ****************************************************************************/

   // Mass flux for cell faces [J][i] in x-direction
   for(int J=1; J<NumJ-1; J++) {
      for(int i=0; i<Numi; i++) {
         ScalarXConvFlux[J][i] = density * UJi[J][i];
      }
   }

   // Mass flux for cell faces [j][I] in y-direction
   for(int j=0; j<Numj; j++) {
      for(int I=1; I<NumI-1; I++) {
         ScalarYConvFlux[j][I] = density * VjI[j][I];
      }
   }
}  // End of function ConvFluxCalcScalar()
