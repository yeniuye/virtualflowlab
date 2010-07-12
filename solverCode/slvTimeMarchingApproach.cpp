/***************************************************************/
/*                  TIME MARCHING APPROACH                     */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* This function calculates the U and V velocity relaxation parameters.      */
/* Actually what is calculated here is not relaxation parameters but the     */
/* unsteady terms in the transient momentum equations,these two have similar */
/* effects on the finite difference equation. So let sat the relaxation      */
/* parameters.In the ITERATIVE approach relaxation                           */
/* parameters are same in every node in the domain and do not change         */
/* with iteration. On the other hand in the TIME MARCHING approach           */
/* the relaxation parameters is different from node to node and              */
/* changing with time (iteration).                                           */
/*****************************************************************************/

void CalTimeRelaxationForMomentumEqn (double Ro,int Numi,int Numj,int NumI,
int NumJ,double** &TimeURelaxPar,double** &TimeVRelaxPar,double** UmomaCenter,
double** VmomaCenter,double** XJIcoorCenter,double** YJIcoorCenter,
double** XjicoorCorners,double** YjicoorCorners,double TimeStep)
{
   for(int J=1;J<NumJ-1;J++) {
     for(int i=1;i<Numi-1;i++) {
        TimeURelaxPar[J][i] = UmomaCenter[J][i]
        / (UmomaCenter[J][i] + Ro * (YjicoorCorners[J][i]-YjicoorCorners[J-1][i]) *
        (XJIcoorCenter[J][i+1] - XJIcoorCenter[J][i]) / TimeStep );
     }
   }

   for(int j=1;j<Numj-1;j++) {
      for(int I=1;I<NumI-1;I++) {
         TimeVRelaxPar[j][I] = VmomaCenter[j][I] /
         ( VmomaCenter[j][I] + Ro * (YJIcoorCenter[j+1][I]- YJIcoorCenter[j][I]) *
         (XjicoorCorners[j][I]-XjicoorCorners[j][I-1]) / TimeStep );
      }
   }
} // End of function CalTimeRelaxationForMomentumEqn()




/* This function inilizes the relaxation parameters for U and V momentum
equations. It is performed once in the run.
And this function initilize the velocity relaxation values determined by the
user to the whole computational domain for ITERATIVE approach.
And it inilize these parameters to 0.9 in TIME MARCHING approach */

void InitilizeRelaxationParameters(int Numi,int Numj,int NumI,int NumJ,
double** &TimeURelaxPar,double** &TimeVRelaxPar,double** &TimeScaRelaxPar,int SolverApproach,
double UvelRelax,double VvelRelax,double ScalarRelax)
{
   if (SolverApproach == 1) {                // ITERATIVE APPROACH
      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
            TimeURelaxPar[J][i] = UvelRelax;
         }
      }
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            TimeVRelaxPar[j][I] = VvelRelax;
         }
      }
      for(int J=1;J<NumJ-1;J++) {
         for(int I=1;I<NumI-1;I++) {
            TimeScaRelaxPar[J][I] = ScalarRelax;
         }
      }
   } else if (SolverApproach == 2) {           // TIME MARCHING APPROACH
      /* These 0.9 values as relaxation parameters are used only once in the first
      global iteration the relaxation parameters for the following iterations
      will be calculated by the code */

      for(int J=1;J<NumJ-1;J++) {
         for(int i=1;i<Numi-1;i++) {
           TimeURelaxPar[J][i] = 0.9;  // for Simplec time marching approach if it is taken as 1 division by zero error occurs
         }
      }
      for(int j=1;j<Numj-1;j++) {
         for(int I=1;I<NumI-1;I++) {
            TimeVRelaxPar[j][I] = 0.9; // for Simplec time marching approach if it is taken as 1 division by zero error occurs
         }
      }
      for(int J=1;J<NumJ-1;J++) {
         for(int I=1;I<NumI-1;I++) {
            TimeScaRelaxPar[J][I] = 0.9;
         }
      }
   }
} // End of function InitilizeRelaxationParameters()
