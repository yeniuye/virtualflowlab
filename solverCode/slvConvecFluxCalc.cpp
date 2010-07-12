
/***************************************************************/
/*                 CONVECTIVE FLUX CALCULATOR                  */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* "ConvFluxCalcUmom" function calculates the mass flux in cell faces of the */
/* U-momentum cells. It calculates the mass flux for both the X-faces        */
/* [J][I] (west and east) and Y-faces [j][i] (south and north).              */
/* Two arrays are created one for the mass flux in x direction and one for   */
/* the y direction.                                                          */
/*****************************************************************************/
void ConvFluxCalcUmom(double** &UmomXConvFlux,double** &UmomYConvFlux,
double** UJivelocity,double** VjIvelocity,int Numi, int Numj, int NumI,
int NumJ,double Ro)
{
   /*****************************************************************************/
   /* Memory is allocaded for the two arrays in the main by using a predefined  */
   /* function. The type of the memory allocated is double by default           */
   /* IMPORTANT : The array size allocated is bigger than the needed since the  */
   /* mass flux is not calculated at all [J][I] and [j][i] nodes but to         */
   /* make the code more readible we keep it as it is.                          */
   /*****************************************************************************/


   /* Mass Flux is calculated for the cell faces[J][I] in x-direction first */
   //cout << "Convective Mass Flux for U momentum at faces in x-direction\n";
   for(int J=1;J<NumJ-1;J++) {
      for(int I=1;I<NumI-1;I++) {
         UmomXConvFlux[J][I]=(0.5)*Ro*(UJivelocity[J][I-1]+UJivelocity[J][I]);
      }
   }

   /* Mass Flux is calculated for the cell faces[j][i] in y-direction second */

   //cout << "Convective Mass Flux for U momentum at faces in y-direction\n";
   for(int j=0;j<Numj;j++) {
		for(int i=1;i<Numi-1;i++) {
         UmomYConvFlux[j][i]=(0.5)*Ro*(VjIvelocity[j][i]+VjIvelocity[j][i+1]);
      }
   }

}  // End of function ConvFluxCalcUmom()




/*****************************************************************************/
/* "ConvFluxCalcVmom" function calculates the mass flux in cell faces of the */
/* V-momentum cells. It calculates the mass flux for both the X-faces        */
/* [j][i] (west and east) and Y-faces [J][I] (south and north).              */
/* Two arrays are created one for the mass flux in x direction and one for   */
/* the y direction.                                                          */
/*****************************************************************************/

void ConvFluxCalcVmom(double** &VmomXConvFlux,double** &VmomYConvFlux,
double** UJivelocity,double** VjIvelocity,int Numi, int Numj, int NumI,
int NumJ,double Ro)
{
   /* Memory allocation policy is same as the ConvFluxCalcUmom function */

   /* Mass Flux is calculated for the cell faces[j][i] in x-direction first */

   for(int j=1;j<Numj-1;j++) {
      for(int i=0;i<Numi;i++) {
         VmomXConvFlux[j][i]=(0.5)*Ro*(UJivelocity[j][i]+UJivelocity[j+1][i]);
      }
   }

   /* Mass Flux is calculated for the cell faces[J][I] in y-direction second */

   for(int J=1;J<NumJ-1;J++) {
      for(int I=1;I<NumI-1;I++) {
         VmomYConvFlux[J][I]=(0.5)*Ro*(VjIvelocity[J][I]+VjIvelocity[J-1][I]);
      }
   }

}  // End of function ConvFluxCalcVmom()


/****************************************************************************/
/* This function calculates the convective fluxes for the scalar variables, */
/* like temperature.Fortunatelly, staggered grid errangement enables us to  */
/* calculate convective fluxes without making any interpolation.            */
/* This function added 08.01.2007                                           */
/****************************************************************************/
void ConvFluxCalcScalar(double** &ScalarXConvFlux,double** &ScalarYConvFlux,
double** UJivelocity,double** VjIvelocity,int Numi, int Numj, int NumI,
int NumJ,double Ro)
{
   /* Mass Flux is calculated for the cell faces[J][i] in x-direction first */
   for(int J=1;J<NumJ-1;J++) {
      for(int i=0;i<Numi;i++) {
         ScalarXConvFlux[J][i]=Ro*UJivelocity[J][i];
      }
   }

   /* Mass Flux is calculated for the cell faces[j][I] in y-direction second */
   for(int j=0;j<Numj;j++) {
      for(int I=1;I<NumI-1;I++) {
         ScalarYConvFlux[j][I]=Ro*VjIvelocity[j][I];
      }
   }
}  // End of function ConvFluxCalcScalar()


