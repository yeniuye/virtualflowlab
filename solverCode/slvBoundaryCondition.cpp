#include "slvFunctions.h"




// Necessary operation for inlet BC
void SpecifiedVelBC ()
{
   // Specified velocity BC for left and right boundaries
   for(int J=0; J<NumJ; J++) {
      if (BoundaryLeft[J][0] == 2) {
         UJi[J][0] = BoundaryLeft[J][1];
      }
      if (BoundaryRight[J][0] == 2) {
         UJi[J][Numi-1] = BoundaryRight[J][1];
      }
   }

   // Specified velocity BC for top and bottom boundaries
   for(int I=0; I<NumI; I++) {
      if (BoundaryTop[I][0] == 2) {
         VjI[Numj-1][I] = BoundaryTop[I][1];
      }
      if (BoundaryBottom[I][0] == 2) {
         VjI[0][I] = BoundaryBottom[I][1];
      }
   }

} // End of function SpecifiedVelBC()




// Necessary operation for outlet BC
void OutletBC()
{
   // West and East boundaries
   for(int J=1; J<NumJ-1; J++) {
      if (BoundaryRight[J][0] == 3) {
         UJi[J][Numi-1] = UJi[J][Numi-2];
      }
      if (BoundaryLeft[J][0] == 3) {
         UJi[J][0] = UJi[J][1];
      }
   }

   for(int j=1;j<Numj-1;j++) {
      if (BoundaryRight[j][0]==3) {
         VjI[j][NumI-1] = VjI[j][NumI-2];
      }
      if (BoundaryLeft[j][0]==3) {
         VjI[j][0] = VjI[j][1];
      }
   }

   // North and South boundaries
   for(int I=1; I<NumI-1; I++) {
      if (BoundaryTop[I][0] == 3) {
         VjI[Numj-1][I] = VjI[Numj-2][I];
      }
      if (BoundaryBottom[I][0] == 3) {
         VjI[0][I] = VjI[1][I];
      }
   }

   for(int i=1; i<Numi-1; i++) {
      if (BoundaryTop[i][0] == 3) {
         UJi[NumJ-1][i] = UJi[NumJ-2][i];
      }
      if (BoundaryBottom[i][0] == 3) {
         UJi[0][i] = UJi[1][i];
      }
   }

} // End of function OutletBC()
