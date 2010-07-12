/***************************************************************/
/*                BOUNDARY CONDITION FUNCTIONS                 */
/***************************************************************/

#include "slvFunctions.h"

// Necessary oparation for inlet boundary condition
void SpecVelBoundCond (double** BoundaryLeft,double** BoundaryRight,
double** BoundaryTop,double** BoundaryBottom,int Numj,int NumJ,int Numi,
int NumI,double** &UJivelocity,double** &VjIvelocity)
{
   // Specified Velocity Boundary condition for left and right boundaries
   for(int J=0;J<NumJ;J++) {
      if (BoundaryLeft[J][0]==2) {
         UJivelocity[J][0] = BoundaryLeft[J][1];
      }
      if (BoundaryRight[J][0]==2) {
         UJivelocity[J][Numi-1] = BoundaryRight[J][1];
      }
   }

   // Specified Velocity Boundary condition for top and bottom boundaries
   for(int I=0;I<NumI;I++) {
      if (BoundaryTop[I][0]==2) {
         VjIvelocity[Numj-1][I] = BoundaryTop[I][1];
      }
      if (BoundaryBottom[I][0]==2) {
         VjIvelocity[0][I] = BoundaryBottom[I][1];
      }
   }

} // End of function SpecVelBoundCond()



// Necessary oparation for outlet boundary condition
void OutletBC(double** BoundaryRight,double** BoundaryLeft,double** BoundaryTop,
double** BoundaryBottom,double** &UJivelocity,double** &VjIvelocity,int Numi,
int Numj,int NumI,int NumJ)
{
   /* WEST and EAST BOUNDARIES */
   for(int J=1;J<NumJ-1;J++) {
      if (BoundaryRight[J][0]==3) {
         UJivelocity[J][Numi-1] = UJivelocity[J][Numi-2];
      }
      if (BoundaryLeft[J][0]==3) {
         UJivelocity[J][0] = UJivelocity[J][1];
      }
   }

   for(int j=1;j<Numj-1;j++) {
      if (BoundaryRight[j][0]==3) {
         VjIvelocity[j][NumI-1] = VjIvelocity[j][NumI-2];
      }
      if (BoundaryLeft[j][0]==3) {
         VjIvelocity[j][0] = VjIvelocity[j][1];
      }
   }

   /* NORTH and SOUTH BOUNDARIES */
   for(int I=1;I<NumI-1;I++) {
      if (BoundaryTop[I][0]==3) {
         VjIvelocity[Numj-1][I] = VjIvelocity[Numj-2][I];
      }
      if (BoundaryBottom[I][0]==3) {
         VjIvelocity[0][I] = VjIvelocity[1][I];
      }
   }

   for(int i=1;i<Numi-1;i++) {
      if (BoundaryTop[i][0]==3) {
         UJivelocity[NumJ-1][i] = UJivelocity[NumJ-2][i];
      }
      if (BoundaryBottom[i][0]==3) {
         UJivelocity[0][i] = UJivelocity[1][i];
      }
   }

} // End of function OutletBC()


