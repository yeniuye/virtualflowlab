#include "slvFunctions.h"

/******************************************************************************
The aim is to create a DAT file in which both velocities and pressure are
stored at the corners of pressure cells.
   
u velocities are kept at Ji connections. So u velocities are already kept at
the physical boundaries for the East and West sides. For the North and South
boundaries however u velocities kept at ghost cells. So the values of
u velocities are carried to the physical boundary for the North and South
boundaries below (FIRST PART).
   
In the SECOND PART u velocities of interior ij nodes are calculated by
averaging.

Similar procedure applies to v velocities
/******************************************************************************/




void TransformColocate(double** pJIResidual, double** UJiResidual, double** VjIResidual,
                       double** &Ujiavrg, double** &Vjiavrg, double** &Pjiavrg,
                       double** &UjiResidual_avrg, double** &VjiResidual_avrg,
                       double** &PjiResidual_avrg, double** &Vorticity_ji,
                       int** BlockCellIndex)
{

   //------------------------  U Velocity Part ----------------------------------

   // WALL BC (Part 3/3)
   // For the collocated representation (physical domain) the boundaries are
   // modified  both in the U velocity and V velocity part

   for(int i=0; i<Numi; i++) {    // FIRST PART
      // South boundary values are carried to pyhsical boundaries.
      if (BoundaryBottom[i][0]==1) {
         Ujiavrg[0][i] = BoundaryBottom[i][1];
      }
      UjiResidual_avrg[0][i] = UJiResidual [0][i];

      // North boundary values are carried to pyhsical boundaries.
      if (BoundaryTop[i][0]==1) {
         Ujiavrg[Numj-1][i] = BoundaryTop[i][1];
      }
      UjiResidual_avrg[Numj-1][i] = UJiResidual [Numj][i];
   }

   // u velocity at interior nodes are obtained by averaging (SECOND PART)
   for(int j=1; j<Numj-1; j++) {
      for(int i=0;i<Numi;i++) {
         Ujiavrg[j][i] = 0.5 * (UJi[j][i]+UJi[j+1][i]);
         UjiResidual_avrg[j][i] = 0.5 * (UJiResidual[j][i]+UJiResidual[j+1][i]);
      }
   }



   //------------------------  V Velocity Part ---------------------------------

   for(int j=0; j<Numj; j++) {     // FIRST PART
      // West boundary values are carried to pyhsical boundaries.
      if (BoundaryLeft[j][0]==1) {
         Vjiavrg[j][0] = BoundaryLeft[j][1];
      }
      VjiResidual_avrg[j][0] = VjIResidual[j][0];
      // East boundary values are carried to pyhsical boundaries.
      if (BoundaryRight[j][0]==1) {
         Vjiavrg[j][Numi-1] = BoundaryRight[j][1];
      }
      VjiResidual_avrg[j][Numi-1] = VjIResidual[j][Numi];
   }

   // v velocity at interior nodes are obtained by averaging (SECOND PART)
   for(int j=0; j<Numj; j++) {
      for(int i=1;i<Numi-1;i++) {
         Vjiavrg[j][i] = 0.5 * (VjI[j][i]+VjI[j][i+1]);
         VjiResidual_avrg[j][i] = 0.5 * (VjIResidual[j][i]+VjIResidual[j][i+1]);
      }
   }



   // This part is about walls due to blocked cells. They require special attention.

   for(int NumBlck = 0; NumBlck < nBlockCells; NumBlck++) {
      for(int j = BlockCellIndex[NumBlck][0]; j < BlockCellIndex[NumBlck][1]+1; j++) {
         Vjiavrg[j][BlockCellIndex[NumBlck][2]] = 0;
         Vjiavrg[j][BlockCellIndex[NumBlck][3]] = 0;
      }
      for(int i = BlockCellIndex[NumBlck][2]; i < BlockCellIndex[NumBlck][3]+1; i++) {
         Ujiavrg[BlockCellIndex[NumBlck][0]][i] = 0;
         Ujiavrg[BlockCellIndex[NumBlck][1]][i] = 0;
      }
   }



   //--------------------------  Pressure Part ---------------------------------

   /******************************************************************************
    For the pressure nodes all ghost cells have zero pressures, so taking the
    average of the four pressure nodes will not be wise. Pressure at the boundary
    nodes are obtained by averaging the two closest nodes.

    At the 4 corners of the flow domain pressure is taken to be the same as the
    one at the closest pressure node.
   /******************************************************************************/

   for(int j=1; j<Numj-1; j++) {
      // West boundary
      Pjiavrg[j][0] = 0.5 * (pJI[j][1] + pJI[j+1][1]);
      PjiResidual_avrg[j][0] = 0.5 * (pJIResidual[j][1] + pJIResidual[j+1][1]);
      // East boundary
      Pjiavrg[j][Numi-1] = 0.5 * (pJI[j][Numi-1] + pJI[j+1][Numi-1]);
      PjiResidual_avrg[j][Numi-1] = 0.5 * (pJIResidual[j][Numi-1] + pJIResidual[j+1][Numi-1]);
   }

   for(int i=1; i<Numi-1; i++) {
      // South boundary
      Pjiavrg[0][i] = 0.5 * (pJI[1][i] + pJI[1][i+1]);
      PjiResidual_avrg[0][i] = 0.5 * (pJIResidual[1][i] + pJIResidual[1][i+1]);
      // North boundary
      Pjiavrg[Numj-1][i] = 0.5 * (pJI[Numj-1][i] + pJI[Numj-1][i+1]);
      PjiResidual_avrg[Numj-1][i] = 0.5 * (pJIResidual[Numj-1][i] + pJIResidual[Numj-1][i+1]);
   }

   // Corners of the flow domain
   Pjiavrg[0][0]           = pJI[1][1];             // South-West corner
   Pjiavrg[0][Numi-1]      = pJI[1][Numi-1];        // South-East corner
   Pjiavrg[Numj-1][0]      = pJI[Numj-1][1];        // North-West corner
   Pjiavrg[Numj-1][Numi-1] = pJI[Numj-1][Numi-1];   // North-East corner

   PjiResidual_avrg[0][0]           = pJIResidual[1][1];            // South-West corner
   PjiResidual_avrg[0][Numi-1]      = pJIResidual[1][Numi-1];       // South-East corner
   PjiResidual_avrg[Numj-1][0]      = pJIResidual[Numj-1][1];       // North-West corner
   PjiResidual_avrg[Numj-1][Numi-1] = pJIResidual[Numj-1][Numi-1];  // North-East corner

   // Pressure at interior nodes are obtained by averaging
   for(int j=1; j<Numj-1; j++) {
      for(int i=1; i<Numi-1; i++) {
         Pjiavrg[j][i] = 0.25 * (pJI[j][i] + pJI[j+1][i] + pJI[j][i+1] + pJI[j+1][i+1]);
         PjiResidual_avrg[j][i] = 0.25 * (pJIResidual[j][i] + pJIResidual[j+1][i] + pJIResidual[j][i+1] + pJIResidual[j+1][i+1]);
      }
   }



   //--------------------------  Vorticity Part ---------------------------------
   
   for(int j=1; j<Numj-1; j++) {
      for(int i=1; i<Numi-1; i++) {
         Vorticity_ji[j][i] = (VjI[j][i+1] - VjI[j][i]) / (XjI[j][i+1] - XjI[j][i])
                            - (UJi[j+1][i] - UJi[j][i]) / (YJi[j+1][i]  - YJi[j][i]);
      }
   }

} // End of function TransformColocate()
