/***************************************************************/
/*                      Residual Calculator                    */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* Residual Calculator function has 2 objects.                               */
/* 1) Calculates the velocity and pressure residuals.                        */
/* 2) Decide on the convergence and divergence criteria.                     */
/*****************************************************************************/

void CalculateResidual (double** PJIpressure,double** UJivelocity,
double** VjIvelocity,double** PJIpressureOLD,double** UJivelocityOLD,
double** VjIvelocityOLD,int Numi,int Numj,int NumI,int NumJ,int &OutIte,
int GlobIte,double** PresCrctSource,double OuterTolerance,
double DivergenceLimit,int &ConvergenceFlag,double* ResidualArray,
double** &PJIResidual,double** &UJiResidual,double** &VjIResidual)
{
   double PresResidual = 0;        // Residual values are initilized to zero
   double UvelResidual = 0 ;       // Residual values are initilized to zero
   double VvelResidual = 0 ;       // Residual values are initilized to zero
   double MassSourceResidual = 0;  // Residual values are initilized to zero

   // PRESSURE RESIDUAL IS CALCULATED BELOW
   for(int J=1;J<NumJ-1;J++) {  // EACH ROW
      for(int I=1;I<NumI-1;I++) {  // EACH COLUMN
         // Next line will be used for residual plot
         PJIResidual[J][I] = fabs(PJIpressure[J][I]-PJIpressureOLD[J][I]);
         // Residual values for each node is calculated and summed up ==>
  			PresResidual = PresResidual + fabs(PJIpressure[J][I]-PJIpressureOLD[J][I]);
      }
   }

	// The calculated residual is divided by the node number to find an average
	PresResidual = PresResidual / ((NumJ-2)*(NumI-2));
	ResidualArray[0]= PresResidual;

   // MASS SOURCE RESIDUAL IS CALCULATED BELOW   // Should be modified (cuneyt: ???)
   for(int J=1;J<NumJ-1;J++) {
  	   for(int I=1;I<NumI-1;I++) {
         if ( fabs(PresCrctSource[J][I]) > MassSourceResidual )
  			   MassSourceResidual = fabs(PresCrctSource[J][I]);
         }
		// MassSourceResidual = MassSourceResidual / ((NumJ-2)*(NumI-2));
   }
   ResidualArray[1]= MassSourceResidual;

   // U VELOCITY RESIDUAL IS CALCULATED BELOW
   for(int J=1;J<NumJ-1;J++) {  // EACH ROW
      for(int i=1;i<Numi-1;i++) {  // EACH COLUMN
         // Next line will be used for residual plot
         UJiResidual[J][i] = fabs(UJivelocity[J][i]-UJivelocityOLD[J][i]);
         // Residual values for each node is calculated and summed up ==>
         UvelResidual = UvelResidual + fabs(UJivelocity[J][i]-UJivelocityOLD[J][i]);
      }
   }
   // The calculated residual is divided by the node number to find an average
	UvelResidual = UvelResidual / ((NumJ-2)*(Numi-2));
	ResidualArray[2]= UvelResidual;
	
   // V VELOCITY RESIDUAL IS CALCULATED BELOW
   for(int j=1;j<Numj-1;j++) {
      for(int I=1;I<NumI-1;I++) {
         // Next line will be used for residual plot
         VjIResidual[j][I] = fabs(VjIvelocity[j][I]-VjIvelocityOLD[j][I]);
  	      // Residual values for each node is calculated and summed up ==>
         VvelResidual = VvelResidual + fabs(VjIvelocity[j][I]-VjIvelocityOLD[j][I]);
      }
   }
   // The calculated residual is diveded by the node number to find an average
	VvelResidual = VvelResidual / ((Numj-2)*(NumI-2));
	ResidualArray[3]= VvelResidual;

   /*****************************************************************************/
   /* This part is the convergence and divergence criteria part. Since the      */
   /* pressure residual is the most critic parameter among the variables,       */
   /* we consantrate on it. If the average pressure residual is below critical  */
   /* limit the program will terminate (Convergence statisfied). Also if the    */
   /* residual is above the specified limit, before floting point overflow the  */
   /* program will terminate.                                                   */
   /*****************************************************************************/
   if (MassSourceResidual < OuterTolerance) {        // Convergence criteria
      ConvergenceFlag = 1; // Convergence
   }
   else if (MassSourceResidual > DivergenceLimit) {   // Divergence criteria
      ConvergenceFlag = 2; //Divergence
   }

} // End of function CalculateResidual()



/* This functions keeps the velocity values of the previous iteration
for residual calculation */
void VelocityForResidual(double** UJivelocity, double** VjIvelocity, double** UJivelocityOLD, double** VjIvelocityOLD, int Numi, int Numj, int NumI, int NumJ)
{
   for(int J=1;J<NumJ-1;J++) {
      for(int i=1;i<Numi-1;i++) {
         UJivelocityOLD[J][i] = UJivelocity[J][i];
      }
   }
   for(int j=1;j<Numj-1;j++) {
      for(int I=1;I<NumI-1;I++) {
         VjIvelocityOLD[j][I] = VjIvelocity[j][I];
      }
   }
}  // End of function VelocityForResidual()




/* This functions keeps the pressure values of the previous iteration for residual calculation */
void PressureForResidual (double** PJIpressure, double** PJIpressureOLD, int NumI,int NumJ)
{
   for(int J=1;J<NumJ-1;J++) {
      for(int I=1;I<NumI-1;I++) {
         PJIpressureOLD[J][I] = PJIpressure[J][I];
      }
 	}
}  // End of function PressureForResidual()

