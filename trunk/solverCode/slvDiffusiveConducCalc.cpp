/***************************************************************/
/*              DIFFUSIVE CONDUCTANCE CALCULATOR               */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* The function "DiffConducCalcUmom" calculates diffusive conductance for    */
/* U-momemtum cell faces. The diffusion conductance is kept at two different */
/* arrays one for the west and east faces and the other for south and north  */
/* faces.                                                                    */
/*****************************************************************************/
void DiffConducCalcUmom(double** &UmomXDiffCond,double** &UmomYDiffCond,
double** XJicoorSideFaces,double** YJicoorSideFaces,int Numi,int Numj,int NumI,
int NumJ,double Kinvis)
{
   /*****************************************************************************/
   /* Memory is allocaded for the two arrays in the main by using a predefined  */
   /* function. The type of the memory allocated is double by default           */
   /* IMPORTANT : The array size allocated is bigger than the needed since the  */
   /* mass flux is not calculated at all [J][I] and [j][i] nodes but to         */
   /* make the code more readible we keep it as it is.                          */
   /*****************************************************************************/

   /* Diffusive Conductance is calculated for the cell faces[J][I], using the
   "XcoorSideFaces" in x-direction first.*/

   for(int J=1;J<NumJ-1;J++) {
 		for(int I=1;I<NumI-1;I++) {
         UmomXDiffCond[J][I]=(Kinvis/(XJicoorSideFaces[J][I]-XJicoorSideFaces[J][I-1]));
      }
   }

   /* Diffusive Conductance is calculated for the cell faces[j][i], using the
   "YcoorSideFaces" in y-direction second. */

   for(int j=0;j<Numj;j++) {
      for(int i=1;i<Numi-1;i++) {
         UmomYDiffCond[j][i]=(Kinvis/(YJicoorSideFaces[j+1][i]-YJicoorSideFaces[j][i]));
      }
   }

}  // End of function DiffConducCalcUmom()




/*****************************************************************************/
/* The function "DiffConducCalcVmom" calculates diffusive conductance for    */
/* V-momemtum cell faces. The diffusion conductance is kept at two different */
/* arrays one for the west and east faces and the other for south and north  */
/* faces.                                                                    */
/*****************************************************************************/
void DiffConducCalcVmom(double** &VmomXDiffCond,double** &VmomYDiffCond,
double** XjIcoorFrontFaces,double** YjIcoorFrontFaces,int Numi,int Numj,
int NumI,int NumJ,double Kinvis)
{

   /*****************************************************************************/
   /* Memory is allocaded for the two arrays in the main by using a predefined  */
   /* function. The type of the memory allocated is double by default           */
   /* IMPORTANT : The array size allocated is bigger than the needed since the  */
   /* mass flux is not calculated at all [J][I] and [j][i] nodes but to         */
   /* make the code more readible we keep it as it is.                          */
   /*****************************************************************************/

   /* Diffusive Conductance is calculated for the cell faces[j][i],using the
   "XcoorFrontFaces" in x-direction first. */

   for(int j=1;j<Numj-1;j++) {
 		for(int i=0;i<Numi;i++) {
         VmomXDiffCond[j][i]=(Kinvis/(XjIcoorFrontFaces[j][i+1] - XjIcoorFrontFaces[j][i]));
      }
   }
   
   /* Diffusive Conductance is calculated for the cell faces[J][I], using the
   "YcoorFrontFaces" in y-direction second. */

   for(int J=1;J<NumJ-1;J++) {
  		for(int I=1;I<NumI-1;I++) {
  			VmomYDiffCond[J][I]=(Kinvis/(YjIcoorFrontFaces[J][I] - YjIcoorFrontFaces[J-1][I]));
      }
   }
}  // End of function DiffConducCalcVmom()




/******************************************************************************/
/* This function calculates the diffusive fluxes for the scalar variables,    */
/* like temperature.                                                          */
/* This function added 08.01.2007                                             */
/******************************************************************************/
void DiffConducCalcScalar(double** &ScalarXDiffCond,double** &ScalarYDiffCond,
double** XJIcoorCenter,double** YJIcoorCenter,int Numi,int Numj,int NumI,
int NumJ,double ThermalConduct)
{
   /* Diffusive Conductance is calculated for the cell faces[J][i], using the
   XJIcoorCenter" in x-direction first.*/

   for(int J=1;J<NumJ-1;J++) {
      for(int i=0;i<Numi;i++) {
         ScalarXDiffCond[J][i]=(ThermalConduct/(XJIcoorCenter[J][i+1]-XJIcoorCenter[J][i]));
      }
   }

   /* Diffusive Conductance is calculated for the cell faces[j][I], using the
   "YJIcoorCenter" in y-direction second. */

   for(int j=0;j<Numj;j++) {
      for(int I=1;I<NumI-1;I++) {
         ScalarYDiffCond[j][I]=(ThermalConduct/(YJIcoorCenter[j+1][I]-YJIcoorCenter[j][I]));
      }
   }

}  // End of function DiffConducCalcScalar()

