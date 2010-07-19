#include "slvFunctions.h"




void CoordinateCreater()
{
   /*****************************************************************************
    Xij and Yij are the corner coordinates of the pressure cells and they were
    previously read from the input file. Here all remaining coordinate arrays
    of staggered grid arrangement are calculated.
   *****************************************************************************/
   
   XJi = MemoryAllocater2D(NumJ, Numi);
   YJi = MemoryAllocater2D(NumJ, Numi);
   XjI = MemoryAllocater2D(Numj, NumI);
   YjI = MemoryAllocater2D(Numj, NumI);
   XJI = MemoryAllocater2D(NumJ, NumI);
   YJI = MemoryAllocater2D(NumJ, NumI);

   // First coordinates at the inner (not boundary) cells are calculated using
   // the known coordinates of pressure cell corners.

   // Coordinates of the center of West and East faces of inner pressure cells
   // are calculated here as XJi and YJi
   for(int j=0; j<NumJ-2; j++) {
      for(int i=0; i<Numi; i++) {
         XJi[j+1][i] = 0.5*(Xji[j][i] + Xji[j+1][i]);
         YJi[j+1][i] = 0.5*(Yji[j][j] + Yji[j+1][i]);
      }
   }

   // Coordinates of the center of South and North faces of inner pressure
   // cells are calculated here as XjI and YjI
   for(int j=0; j<Numj; j++) {
      for(int i=0; i<NumI-2; i++) {
         XjI[j][i+1] = 0.5*(Xji[j][i] + Xji[j][i+1]);
         YjI[j][i+1] = 0.5*(Yji[j][i] + Yji[j][i+1]);
      }
   }

   // Coordinates of the center of inner pressure cells are calculated
   // here as XJI and YJI
   for(int j=0; j<NumJ-2; j++) {
      for(int i=0; i<NumI-2; i++) {
         XJI[j+1][i+1] = 0.25*(Xji[j][i] + Xji[j][i+1] + Xji[j+1][i] + Xji[j+1][i+1]);
         YJI[j+1][i+1] = 0.25*(Yji[j][i] + Yji[j][i+1] + Yji[j+1][i] + Yji[j+1][i+1]);
      }
   }
   

   // Now coordinates related to the boundaries are calculated

   // West boundary
   for(int j=0; j<Numj; j++) {
      XjI[j][0] = 2 * Xji[j][0] - XjI[j][1];
      YjI[j][0] = 2 * Yji[j][0] - YjI[j][1];
   }
   for(int j=0; j<Numj-1; j++) {
      XJI[j+1][0] = 0.5 * (XjI[j][0] + XjI[j+1][0]);
      YJI[j+1][0] = 0.5 * (YjI[j][0] + YjI[j+1][0]);
   }

   // East boundary
   for(int j=0; j<Numj; j++) {
      XjI[j][Numi] = 2 * Xji[j][Numi-1] - XjI[j][Numi-1];
      YjI[j][Numi] = 2 * Yji[j][Numi-1] - YjI[j][Numi-1];
   }
   for(int j=0; j<Numj-1; j++) {
      XJI[j+1][Numi] = 0.5*(XjI[j][Numi] + XjI[j+1][Numi]);
      YJI[j+1][Numi] = 0.5*(YjI[j][Numi] + YjI[j+1][Numi]);
   }

   // South boundary
   for(int i=0; i<Numi; i++) {
      XJi[0][i] = 2 * Xji[0][i] - XJi[1][i];
      YJi[0][i] = 2 * Yji[0][i] - YJi[1][i];
   }
   for(int i=0; i<Numi-1; i++) {
      XJI[0][i+1] = 0.5*(XJi[0][i] + XJi[0][i+1]);
      YJI[0][i+1] = 0.5*(YJi[0][i] + YJi[0][i+1]);
   }

   // North boundary
   for(int i=0; i<Numi; i++) {
      XJi[Numj][i] = 2 * Xji[Numj-1][i] - XJi[Numj-1][i];
      YJi[Numj][i] = 2 * Yji[Numj-1][i] - YJi[Numj-1][i];
   }
   for(int i=0; i<Numi-1; i++) {
     XJI[Numj][i+1] = 0.5*(XJi[Numj][i] + XJi[Numj][i+1]);
     YJI[Numj][i+1] = 0.5*(YJi[Numj][i] + YJi[Numj][i+1]);
   }

   // Coordinates of the four corners of the domain are calculated below.
   // Although they are never used in calculations, they are used to show
   // the ghost cells in some of the Tecplot outputs e.g. TecPlotPressureWriter

   // Bottom left corner
   XJI[0][0] = 2 * XJi[0][0] - XJI[0][1];
   YJI[0][0] = 2 * YJi[0][0] - YJI[0][1];
   
   // Top left corner
   XJI[Numj][0] = 2 * XJi[Numj][0] - XJI[Numj][1];
   YJI[Numj][0] = 2 * YJi[Numj][0] - YJI[Numj][1];
   
   // Bottom right corner
   XJI[0][Numi] = 2 * XJi[0][Numi-1] - XJI[0][Numi-1];
   YJI[0][Numi] = 2 * YJi[0][Numi-1] - YJI[0][Numi-1];
   
   // Top right corner
   XJI[Numj][Numi] = 2 * XJi[Numj][Numi-1] - XJI[Numj][Numi-1];
   YJI[Numj][Numi] = 2 * YJi[Numj][Numi-1] - YJI[Numj][Numi-1];

}  // End of function CoordinateCreater()
