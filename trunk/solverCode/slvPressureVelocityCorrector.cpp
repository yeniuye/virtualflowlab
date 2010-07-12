#include "slvFunctions.h"

/*****************************************************************************/
/* PressureCorrector function has 1 objects.                                 */
/* 1) Update the pressure values (correct) using the pressure correction     */
/*    values that are determined previously (with relaxation ofcourse).      */
/*                                                                           */
/* NOTE 1 > As mentioned before initial pressure correction values for each  */
/* iteration should be zero. So that after this function use the pressure    */
/* correction values, they should be initilized to zero. However             */
/* VelocityCorrecter function also use these values, so this part is done    */
/* in VelocityCorrecter function.                                            */
/*                                                                           */
/* NOTE 2 > Pressure relaxation values are very much dependent on the        */
/* problem of interest, differencing scheme,solver and mesh. The value       */
/* of the pressure relaxation is between 0 and 1.                            */
/*                                                                           */
/* NOTE 3 > The relaxation paremeter simply means, how much you want to      */
/* change the values of pressure in a single iteration. As you increase      */
/* the parameter, the change will be more in each iteration. As a result     */
/* it will converge faster if it does not diverges.                          */
/*****************************************************************************/

void PressureCorrector(double** &PJIpressure, double** &PJICorrect, int NumJ, int NumI,
                       double PresRelax)
{
   for(int J=1; J<NumJ-1; J++) {  // ROW
      for(int I=1; I<NumI-1; I++) {  // COLUMN
         PJIpressure[J][I] = PJIpressure[J][I] + (PresRelax) * PJICorrect[J][I];
      }
   }
}  // End of function PressureCorrector()



/*****************************************************************************/
/* The following VelocityCorrector function has 2 objects                    */
/* 1) Corrects both U and V velocities. By using the pressure correction     */
/*    values and the "PresCrctdJi" and "PresCrctdjI" arrays that are         */
/*    found in the PressureCorrectionMatrix function.                        */
/* 2) Initilize the pressure correction values to zero for the next          */
/*    iteration                                                              */
/*                                                                           */
/* IMPORTANT : In the versions before 1.2 velocity                           */
/* relaxation parameters are used in this function. However it               */
/* is a big MISTAKE, they have to be implimented before                      */
/* this stage. Since pressure correction equation "PressureCorrectionMatrix  */
/* function" is also affected by velocity under-relaxation which is          */
/* executed before. The velocity relaxations are implimented in the          */
/* "UmomMatrixFormer" & "VmomMatrixFormer" functions.                        */
/*****************************************************************************/

void VelocityCorrector(double** &UJivelocity, double** &VjIvelocity, double** PJICorrect,
                       double** PresCrctdJi, double** PresCrctdjI, int NumJ, int NumI, int Numj,
                       int Numi)
{
   // U Velocity Calculation
   for(int J=1; J<NumJ-1; J++) {
      for(int i=1; i<Numi-1; i++) {
         UJivelocity[J][i] = UJivelocity[J][i] +
                             PresCrctdJi[J][i] * (PJICorrect[J][i] - PJICorrect[J][i+1]);
      }
   }

   // V Velocity Calculation
   for(int j=1; j<Numj-1; j++) {
      for(int I=1; I<NumI-1; I++) {
         VjIvelocity[j][I] = VjIvelocity[j][I] +
                             PresCrctdjI[j][I] * (PJICorrect[j][I] - PJICorrect[j+1][I]);
      }
   }

   // Set pressure correction to zero for the next iteration
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         PJICorrect[J][I] = 0.0;
      }
   }
}  // End of function VelocityCorrector()

