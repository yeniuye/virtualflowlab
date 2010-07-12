/***************************************************************/
/*       COORDINATE DATA CREATER                               */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* Function CoordinateCreater takes 14 arguments,it takes all the coordinate */
/* arrays. "XijcoorCorners" and "YijcoorCorners" arrays are previously read  */
/* from a text file. All the remaining coordinate arrays of staggered grid   */
/* arrangement are calculated here.                                          */
/* "SideFaces" denotes the face which the Uvelocity is perpendicular         */
/* "FrontFaces" denotes the face which the Vvelocity is perpendicular        */
/*****************************************************************************/
void CoordinateCreater(double** XjicoorCorners,
double** YjicoorCorners,int NumXcells, int NumYcells,
int Numi,int Numj,int NumI,int NumJ,
double** &XJicoorSideFaces,double** &YJicoorSideFaces,
double** &XjIcoorFrontFaces,double** &YjIcoorFrontFaces,
double** &XJIcoorCenter,double** &YJIcoorCenter)
{
   /* Memory is allocaded for the 6 matrices by using a predefined function
   the type is double by default. */
   XJicoorSideFaces = MemoryAllocater2D(NumJ,Numi);
   YJicoorSideFaces = MemoryAllocater2D(NumJ,Numi);
   XjIcoorFrontFaces = MemoryAllocater2D(Numj,NumI);
   YjIcoorFrontFaces = MemoryAllocater2D(Numj,NumI);
   XJIcoorCenter = MemoryAllocater2D(NumJ,NumI);
   YJIcoorCenter = MemoryAllocater2D(NumJ,NumI);


   /* First the coordinates at the inner cells are calculated by using this two
   previously known coordinate arrays, x coordinates of the pressure cell
   corners and y coordinate of the presure cell corners.*/

   /*****************INNER CELL COORDINATES CALCULATED HERE**********************/

   // ->> The coordinate of the center of the west and east faces of the inner
   //     pressure cells are calculated here XJi & YJi
   for(int j=0;j<NumJ-2;j++) {
      for(int i=0;i<Numi;i++) {
         XJicoorSideFaces[j+1][i]=0.5*(XjicoorCorners[j][i]+XjicoorCorners[j+1][i]);
         YJicoorSideFaces[j+1][i]=0.5*(YjicoorCorners[j][j]+YjicoorCorners[j+1][i]);
      }
   }

   // ->> The coordinate of the center of the south and north faces of the inner
   //     pressure cells are calculated here XjI & YjI
   for(int j=0;j<Numj;j++) {
      for(int i=0;i<NumI-2;i++) {
         XjIcoorFrontFaces[j][i+1]=0.5*(XjicoorCorners[j][i]+XjicoorCorners[j][i+1]);
         YjIcoorFrontFaces[j][i+1]=0.5*(YjicoorCorners[j][i]+YjicoorCorners[j][i+1]);
      }
   }

   // ->> The coordinate of the center of the of the inner
   //     pressure cells are calculated here XJI & YJI
   for(int j=0;j<NumJ-2;j++) {
      for(int i=0;i<NumI-2;i++) {
         XJIcoorCenter[j+1][i+1] = 0.25*(XjicoorCorners[j][i]+XjicoorCorners[j][i+1]+
                                   XjicoorCorners[j+1][i]+XjicoorCorners[j+1][i+1]);
         YJIcoorCenter[j+1][i+1] = 0.25*(YjicoorCorners[j][i]+YjicoorCorners[j][i+1]+
                                   YjicoorCorners[j+1][i]+YjicoorCorners[j+1][i+1]);
      }
   }
   /*************INNER CELL COORDINATES CALCULATION ENDS HERE********************/
   
   /*************BOUNDARY CELL COORDINATES ARE CALCULATED************************/

   // ->> For West Boundary
   //     XjIcoorFrontFaces & YjIcoorFrontFaces
   for(int j=0;j<Numj;j++) {
      XjIcoorFrontFaces[j][0] = 2*XjicoorCorners[j][0] - XjIcoorFrontFaces[j][1];
      YjIcoorFrontFaces[j][0] = 2*YjicoorCorners[j][0] - YjIcoorFrontFaces[j][1];
   }
   //    XJIcoorCenter & YJIcoorCenter
   for(int j=0;j<Numj-1;j++) {
      XJIcoorCenter[j+1][0]=0.5*(XjIcoorFrontFaces[j][0]+XjIcoorFrontFaces[j+1][0]);
      YJIcoorCenter[j+1][0]=0.5*(YjIcoorFrontFaces[j][0]+YjIcoorFrontFaces[j+1][0]);
   }

   // ->> For East Boundary
   //     XjIcoorFrontFaces & YjIcoorFrontFaces
   for(int j=0;j<Numj;j++) {
      XjIcoorFrontFaces[j][Numi] = 2*XjicoorCorners[j][Numi-1] - XjIcoorFrontFaces[j][Numi-1];
      YjIcoorFrontFaces[j][Numi] = 2*YjicoorCorners[j][Numi-1] - YjIcoorFrontFaces[j][Numi-1];
   }

   //    XJIcoorCenter & YJIcoorCenter
   for(int j=0;j<Numj-1;j++) {
      XJIcoorCenter[j+1][Numi] = 0.5*(XjIcoorFrontFaces[j][Numi] + XjIcoorFrontFaces[j+1][Numi]);
      YJIcoorCenter[j+1][Numi] = 0.5*(YjIcoorFrontFaces[j][Numi] + YjIcoorFrontFaces[j+1][Numi]);
   }

   // ->> For South Boundary
   //     XJicoorSideFaces & YJicoorSideFaces
   for(int i=0;i<Numi;i++) {
      XJicoorSideFaces[0][i] = 2*XjicoorCorners[0][i] - XJicoorSideFaces[1][i];
      YJicoorSideFaces[0][i] = 2*YjicoorCorners[0][i] - YJicoorSideFaces[1][i];
   }

   //     XJIcoorCenter & YJIcoorCenter
   for(int i=0;i<Numi-1;i++) {
      XJIcoorCenter[0][i+1]=0.5*(XJicoorSideFaces[0][i] + XJicoorSideFaces[0][i+1]);
      YJIcoorCenter[0][i+1]=0.5*(YJicoorSideFaces[0][i] + YJicoorSideFaces[0][i+1]);
   }

   // ->>  For North Boundary
   //      XJicoorSideFaces & YJicoorSideFaces
   for(int i=0;i<Numi;i++) {
      XJicoorSideFaces[Numj][i]=2*XjicoorCorners[Numj-1][i]-
      XJicoorSideFaces[Numj-1][i];
      YJicoorSideFaces[Numj][i]=2*YjicoorCorners[Numj-1][i]-
      YJicoorSideFaces[Numj-1][i];
   }

   //     XJIcoorCenter & YJIcoorCenter
   for(int i=0;i<Numi-1;i++) {
     XJIcoorCenter[Numj][i+1] = 0.5*(XJicoorSideFaces[Numj][i] + XJicoorSideFaces[Numj][i+1]);
     YJIcoorCenter[Numj][i+1] = 0.5*(YJicoorSideFaces[Numj][i] + YJicoorSideFaces[Numj][i+1]);
   }

   // ->> The Four Corner of the Domain
   /* The X and Y coordinates of the four corners of the computational domain is
   calculated below. Although they are never used in calculations, they are used
   to show the ghost cells in some of the Tecplot outputs e.g.
   TecPlotPressureWriter */

   // Bottom left corner
   XJIcoorCenter[0][0]=2*XJicoorSideFaces[0][0]-XJIcoorCenter[0][1];
   YJIcoorCenter[0][0]=2*YJicoorSideFaces[0][0]-YJIcoorCenter[0][1];
   // Top left corner
   XJIcoorCenter[Numj][0]=2*XJicoorSideFaces[Numj][0]-XJIcoorCenter[Numj][1];
   YJIcoorCenter[Numj][0]=2*YJicoorSideFaces[Numj][0]-YJIcoorCenter[Numj][1];
   // Bottom right corner
   XJIcoorCenter[0][Numi]=2*XJicoorSideFaces[0][Numi-1]-XJIcoorCenter[0][Numi-1];
   YJIcoorCenter[0][Numi]=2*YJicoorSideFaces[0][Numi-1]-YJIcoorCenter[0][Numi-1];
   // Top right corner
   XJIcoorCenter[Numj][Numi]=2*XJicoorSideFaces[Numj][Numi-1] -
   XJIcoorCenter[Numj][Numi-1];
   YJIcoorCenter[Numj][Numi]=2*YJicoorSideFaces[Numj][Numi-1] -
   YJIcoorCenter[Numj][Numi-1];

   /*************BOUNDARY CELL COORDINATES CALCULATION ENDS HERE*****************/

}  // End of function CoordinateCreater()

