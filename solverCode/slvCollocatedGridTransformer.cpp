/***************************************************************/
/*               COLLOCATED GRID TRANSFORMATION                */
/***************************************************************/

#include "slvFunctions.h"

void TransformColocate(double** UJivelocity, double** VjIvelocity,
double** PJIpressure,double** XjicoorCorners, double** YjicoorCorners,int Numi,
int Numj,double** PJIResidual,double** UJiResidual,double** VjIResidual,
double** XjIcoorFrontFaces, double** YJicoorSideFaces,
double** BoundaryTop,double** BoundaryBottom,double** BoundaryLeft,double** BoundaryRight,
double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
double** &Vorticity_ji,int nBlockCells, int** BlockCellCoor)
{

   //------------------------  U Velocity Part ----------------------------------//

   /*  WALL BC (part 3/3)
   For the collocated representation (physical domain) the boundaries
   are modified  both in the U velocity and V velocity part */

   for(int i=0;i<Numi;i++) {    // First Part mentioned above
      // South Boundary values are carried to pyhsical boundaries.
      if (BoundaryBottom[i][0]==1) {
         Ujiavrg[0][i] = BoundaryBottom[i][1];
      }
      UjiResidual_avrg[0][i] = UJiResidual [0][i];

      // North Boundary values are carried to pyhsical boundaries.
      if (BoundaryTop[i][0]==1) {
         Ujiavrg[Numj-1][i] = BoundaryTop[i][1];
      }
      UjiResidual_avrg[Numj-1][i] = UJiResidual [Numj][i];
   }

   for(int j=1;j<Numj-1;j++) {   // Second Part mentioned above
      for(int i=0;i<Numi;i++) {
         // The Average is taken for the interior nodes for U velocity
         Ujiavrg[j][i] = 0.5 * (UJivelocity[j][i]+UJivelocity[j+1][i]);
         UjiResidual_avrg[j][i] = 0.5 * (UJiResidual[j][i]+UJiResidual[j+1][i]);
      }
   }

   //------------------------  V Velocity Part ---------------------------------//

   for(int j=0;j<Numj;j++) {     // First Part mentioned above
      // West Boundary values are carried to pyhsical boundaries.
      if (BoundaryLeft[j][0]==1) {
         Vjiavrg[j][0] = BoundaryLeft[j][1];
      }
      VjiResidual_avrg[j][0] = VjIResidual[j][0];
      // East Boundary values are carried to pyhsical boundaries.
      if (BoundaryRight[j][0]==1) {
         Vjiavrg[j][Numi-1] = BoundaryRight[j][1];
      }
      VjiResidual_avrg[j][Numi-1] = VjIResidual[j][Numi];
   }

   for(int j=0;j<Numj;j++) {   // Second Part mentioned above
      for(int i=1;i<Numi-1;i++) {
         // The Average is taken for the interior nodes for V velocity
         Vjiavrg[j][i] = 0.5 * (VjIvelocity[j][i]+VjIvelocity[j][i+1]);
         VjiResidual_avrg[j][i] = 0.5 * (VjIResidual[j][i]+VjIResidual[j][i+1]);
      }
   }

   // THIS PART IS ABOUT THE INNER WALL DEFINITION //
   // For the boundaries of the innerwall special attention is needed //

   for(int NumBlck=0; NumBlck<nBlockCells; NumBlck++) {
      for(int j=BlockCellCoor[NumBlck][0];j<BlockCellCoor[NumBlck][1]+1;j++) {
         Vjiavrg[j][BlockCellCoor[NumBlck][2]]=0;
         Vjiavrg[j][BlockCellCoor[NumBlck][3]]=0;
      }
      for(int i=BlockCellCoor[NumBlck][2];i<BlockCellCoor[NumBlck][3]+1;i++) {
         Ujiavrg[BlockCellCoor[NumBlck][0]][i]=0;
         Ujiavrg[BlockCellCoor[NumBlck][1]][i]=0;
      }
   }

   /******************************************************************************/
   /* For the pressure nodes all the ghost cells have zero pressures so taking   */
   /* the average of the four pressure nodes will not be wise. So for the        */
   /* value of pressure at the nodes that are at the boundaries the two closest  */
   /* node is taken into account. And for the corner nodes the closest pressure  */
   /* node is taken into account only.                                           */
   /******************************************************************************/

   //--------------------------  Pressure Part ---------------------------------//
   for(int j=1;j<Numj-1;j++) {
      // For the west boundary
      Pjiavrg[j][0] = 0.5 * (PJIpressure[j][1] + PJIpressure[j+1][1]);
      PjiResidual_avrg[j][0] = 0.5 * (PJIResidual[j][1] + PJIResidual[j+1][1]);
      // For the east boundary
      Pjiavrg[j][Numi-1] = 0.5 * (PJIpressure[j][Numi-1] + PJIpressure[j+1][Numi-1]);
      PjiResidual_avrg[j][Numi-1] = 0.5 * (PJIResidual[j][Numi-1] + PJIResidual[j+1][Numi-1]);
   }

   for(int i=1;i<Numi-1;i++) {
      // For the south boundary
      Pjiavrg[0][i] = 0.5 * (PJIpressure[1][i] + PJIpressure[1][i+1]);
      PjiResidual_avrg[0][i] = 0.5 * (PJIResidual[1][i] + PJIResidual[1][i+1]);
      // For the north boundary
      Pjiavrg[Numj-1][i] = 0.5 * (PJIpressure[Numj-1][i] + PJIpressure[Numj-1][i+1]);
      PjiResidual_avrg[Numj-1][i] = 0.5 * (PJIResidual[Numj-1][i] + PJIResidual[Numj-1][i+1]);
   }

   //For the corners
   Pjiavrg[0][0] = PJIpressure[1][1];                       // South-West Corner
   Pjiavrg[0][Numi-1] = PJIpressure[1][Numi-1];             // South-East Corner
   Pjiavrg[Numj-1][0] = PJIpressure[Numj-1][1];             // North-West Corner
   Pjiavrg[Numj-1][Numi-1] = PJIpressure[Numj-1][Numi-1];   // North-East Corner

   PjiResidual_avrg[0][0] = PJIResidual[1][1];                       // South-West Corner
   PjiResidual_avrg[0][Numi-1] = PJIResidual[1][Numi-1];             // South-East Corner
   PjiResidual_avrg[Numj-1][0] = PJIResidual[Numj-1][1];             // North-West Corner
   PjiResidual_avrg[Numj-1][Numi-1] = PJIResidual[Numj-1][Numi-1];   // North-East Corner

   // For the interior nodes
   for(int j=1;j<Numj-1;j++) {
      for(int i=1;i<Numi-1;i++) {
         // The Average is taken for the Pressure Nodes
         Pjiavrg[j][i] = 0.25 * (PJIpressure[j][i]+PJIpressure[j+1][i]+
         PJIpressure[j][i+1]+PJIpressure[j+1][i+1]);
         PjiResidual_avrg[j][i] = 0.25 * (PJIResidual[j][i]+PJIResidual[j+1][i]+
         PJIResidual[j][i+1]+PJIResidual[j+1][i+1]);
      }
   }

   //--------------------------  Vorticity Part ---------------------------------//
   for(int j=1;j<Numj-1;j++) {
      for(int i=1;i<Numi-1;i++) {
         Vorticity_ji[j][i]=(VjIvelocity[j][i+1]-VjIvelocity[j][i])/(XjIcoorFrontFaces[j][i+1]-XjIcoorFrontFaces[j][i])
         -(UJivelocity[j+1][i]-UJivelocity[j][i])/(YJicoorSideFaces[j+1][i]-YJicoorSideFaces[j][i]);
      }
   }

} // End of function TransformColocate()

