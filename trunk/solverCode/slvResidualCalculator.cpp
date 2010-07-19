#include "slvFunctions.h"




void CalculateResidual (double** pJIOLD, double** UJiOLD, double** VjIOLD, double** PresCrctSource,
                        double DivergenceLimit, int &ConvergenceFlag, double* ResidualArray,
                        double** &pJIResidual, double** &UJiResidual, double** &VjIResidual)
{
   /*****************************************************************************
    This function
    1) calculates the velocity and pressure residuals.
    2) declares convergence or divergence based on mass source residual.
   *****************************************************************************/

   double PresResidual = 0;        // Residual values are initilized to zero
   double UvelResidual = 0 ;
   double VvelResidual = 0 ;
   double MassSourceResidual = 0;


   // Calculate pressure residual
   for(int J=1; J<NumJ-1; J++) {  // ROW
      for(int I=1; I<NumI-1; I++) {  // COLUMN
         // Next line will be used for residual plot
         pJIResidual[J][I] = fabs(pJI[J][I] - pJIOLD[J][I]);
         // Add up individual pressure residuals
         PresResidual = PresResidual + pJIResidual[J][I];
      }
   }
   // Pressure residual is divided by the node number to find an average
   PresResidual = PresResidual / ((NumJ-2)*(NumI-2));
   ResidualArray[0] = PresResidual;




   // Calculate mass source residual      // Cuneyt: Does this need modification ?
   for(int J=1; J<NumJ-1; J++) {
      for(int I=1; I<NumI-1; I++) {
         if ( fabs(PresCrctSource[J][I]) > MassSourceResidual ) {    // Cuneyt: What is this line doing ?
            MassSourceResidual = fabs(PresCrctSource[J][I]);
         }
      }
      // MassSourceResidual = MassSourceResidual / ((NumJ-2)*(NumI-2));
   }
   ResidualArray[1] = MassSourceResidual;




   // Calculate u velocity residual
   for(int J=1; J<NumJ-1; J++) {  // ROW
      for(int i=1; i<Numi-1; i++) {  // COLUMN
         // Next line will be used for residual plot
         UJiResidual[J][i] = fabs(UJi[J][i] - UJiOLD[J][i]);
         // Add up individual u velocity residuals
         UvelResidual = UvelResidual + UJiResidual[J][i];
      }
   }
   // u velocity residual is divided by the node number to find an average
   UvelResidual = UvelResidual / ((NumJ-2)*(Numi-2));
   ResidualArray[2] = UvelResidual;




   // Calculate v velocity residual
   for(int j=1; j<Numj-1; j++) {  // ROW
      for(int I=1; I<NumI-1; I++) {  // COLUMN
         // Next line will be used for residual plot
         VjIResidual[j][I] = fabs(VjI[j][I]-VjIOLD[j][I]);
         // Add up individual v velocity residuals
         VvelResidual = VvelResidual + fabs(VjI[j][I]-VjIOLD[j][I]);
      }
   }
   // v velocity residual is divided by the node number to find an average
   VvelResidual = VvelResidual / ((Numj-2)*(NumI-2));
   ResidualArray[3]= VvelResidual;



   // Perform convergence / divergence check
   if (MassSourceResidual < outerTolerance) {
      ConvergenceFlag = 1;   // Convergence
   }
   else if (MassSourceResidual > DivergenceLimit) {
      ConvergenceFlag = 2;   // Divergence
   }

} // End of function CalculateResidual()




void copyVelocity(double** UJiOLD, double** VjIOLD)
{
   // Take a copy of velocity values of the current iteration for residual calculation.
   
   for(int J=1;J<NumJ-1;J++) {
      for(int i=1;i<Numi-1;i++) {
         UJiOLD[J][i] = UJi[J][i];
      }
   }
   for(int j=1;j<Numj-1;j++) {
      for(int I=1;I<NumI-1;I++) {
         VjIOLD[j][I] = VjI[j][I];
      }
   }
}




void copyPressure (double** pJIOLD)
{
   // Take a copy of pressure values of the current iteration for residual calculation.
   
   for(int J=1;J<NumJ-1;J++) {
      for(int I=1;I<NumI-1;I++) {
         pJIOLD[J][I] = pJI[J][I];
      }
   }
}
