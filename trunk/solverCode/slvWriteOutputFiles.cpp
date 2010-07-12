/***************************************************************/
/*                   WRITE DAT and OUT FILES                   */
/***************************************************************/

#include "slvFunctions.h"

/*****************************************************************************/
/* This function writes the calculated velocity and pressure values to a     */
/* text file in the same format of the initial value text file. So that      */
/* it can be used to continue to the current analysis.                       */
/*****************************************************************************/
void WriteOUTfile(double** &PJIpressure,double** &UJivelocity,
double** &VjIvelocity,int NumI,int NumJ,int Numi,int Numj,string Directory,
int OutIte)
{
   string OUTfileName;

   stringstream ss;
   ss << OutIte;
   OUTfileName = Directory + "_" + ss.str() + ".out";

   ofstream OUTfile;
   OUTfile.open(OUTfileName.data());

   OUTfile << "/********************************************/" << "\n";
   OUTfile << "/***      PRESSURE Initial Condition      ***/" << "\n";
   OUTfile << "/********************************************/" << "\n";

   for(int J=0; J<NumJ; J++) {
      for(int I=0; I<NumI; I++) {
         OUTfile << PJIpressure[J][I] << "\n";
      }
   }

   OUTfile << "/********************************************/" << "\n";
   OUTfile << "/***    U VELOCITY Initial Condition      ***/" << "\n";
   OUTfile << "/********************************************/" << "\n";

   for(int J=0; J<NumJ; J++) {
      for(int i=0; i<Numi; i++) {
         OUTfile << UJivelocity[J][i] << "\n";
      }
   }

   OUTfile << "/********************************************/" << "\n";
   OUTfile << "/***    V VELOCITY Initial Condition      ***/" << "\n";
   OUTfile << "/********************************************/" << "\n";

   for(int j=0; j<Numj; j++) {
      for(int I=0; I<NumI; I++) {
         OUTfile << VjIvelocity[j][I] << "\n";
      }
   }

   OUTfile.close();

}  // End of function WriteOUTfile()




/* This function creates DAT files for Tecplot */
void WriteDATfile(int OutIte,string Directory,
double** UJivelocity,double** VjIvelocity,double** PJIpressure,
double** XjicoorCorners,double** YjicoorCorners,int Numi,int Numj,
bool isTecplot,double** PJIResidual,double** UJiResidual,
double** VjIResidual,double** XjIcoorFrontFaces, double** YJicoorSideFaces,
double** BoundaryTop,double** BoundaryBottom,double** BoundaryLeft,
double** BoundaryRight,
double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
double** &Vorticity_ji,int nBlockCells, int** BlockCellCoor)
{
   if (isTecplot == 0)
      return;

   string DATfileName;
   ofstream DATfile;

   stringstream ss;
   ss << OutIte;
   DATfileName = Directory + "_" + ss.str() + ".dat";

   DATfile.open(DATfileName.data());

   /******************************************************************************/
   /* The U velocities are kept at Ji connections. So U velocities kept at the   */
   /* physical boundaries for the east and west side boundaries. For the north   */
   /* and south boundaries however the U velocities kept at the ghost cells.     */
   /* So the values of U velocities carried to the physical boundary for the     */
   /* north and south boundaries below (First).                                  */
   /*                                                                            */
   /* The reaming part takes the average of the U velocities and keep it at the  */
   /* ij connections for interior nodes (Second).                                */
   /* The aim is to show both U and V velocities and Pressure at the same node   */
   /* which is the corner of scalar (pressure) cells that do not keep any        */
   /* variable before.                                                           */
   /*                                                                            */
   /* So what is seen in the TECPLOT is the real physical case. And since we     */
   /* keep all the variables at the same node we can draw vector plots.          */
   /******************************************************************************/

   TransformColocate(UJivelocity,VjIvelocity,PJIpressure,
   XjicoorCorners,YjicoorCorners,Numi,Numj,PJIResidual,UJiResidual,VjIResidual,
   XjIcoorFrontFaces,YJicoorSideFaces,
   BoundaryTop,BoundaryBottom,BoundaryLeft,BoundaryRight,
   Ujiavrg,Vjiavrg,Pjiavrg,
   UjiResidual_avrg,VjiResidual_avrg,PjiResidual_avrg,
   Vorticity_ji,nBlockCells,BlockCellCoor);

   DATfile << "TITLE = \"Uvelocity_Vvelocity_Pressure\"\n";
   DATfile << "VARIABLES = X,Y,U,V,P,U_res,V_res,P_res,Vorticity\n";
   DATfile <<  "ZONE T =  \"ZONE 1\" ,i = " << Numi << ",j = " << Numj << ",F=POINT\n";
   DATfile << fixed << setprecision(10);
   for(int j=0;j<Numj;j++) {
      for(int i=0;i<Numi;i++) {
         DATfile << XjicoorCorners[j][i] << " " << YjicoorCorners[j][i] << " " <<
                    Ujiavrg[j][i] <<" " << Vjiavrg[j][i] << " " << Pjiavrg[j][i] << " " <<
                    UjiResidual_avrg[j][i] << " " << VjiResidual_avrg[j][i] << " " <<
                    PjiResidual_avrg[j][i] << " " << Vorticity_ji[j][i] << "\n";
      }
   }

   DATfile.close();

}  // End of function WriteDATfile()

