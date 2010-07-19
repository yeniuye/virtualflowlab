#include "slvFunctions.h"




void writeOUTfile(int outerIter)
{
   /*****************************************************************************
    This function writes the calculated velocity and pressure values to a file
    with OUT extension. The user can rename an OUT file as MyProblem.restart and
    use the velocity and pressure values in the file as an initial condition of
    a new run.
   *****************************************************************************/

   string OUTfileName;

   stringstream ss;
   ss << outerIter;
   OUTfileName = fullProblemName + "_" + ss.str() + ".out";   // e.g. C:\VFL\Problems\MyProblem_1000.out

   ofstream OUTfile;
   OUTfile.open(OUTfileName.data());

   OUTfile << "/********************************************/" << "\n";
   OUTfile << "/***      PRESSURE Initial Condition      ***/" << "\n";
   OUTfile << "/********************************************/" << "\n";

   for(int J=0; J<NumJ; J++) {
      for(int I=0; I<NumI; I++) {
         OUTfile << pJI[J][I] << "\n";
      }
   }

   OUTfile << "/********************************************/" << "\n";
   OUTfile << "/***    U VELOCITY Initial Condition      ***/" << "\n";
   OUTfile << "/********************************************/" << "\n";

   for(int J=0; J<NumJ; J++) {
      for(int i=0; i<Numi; i++) {
         OUTfile << UJi[J][i] << "\n";
      }
   }

   OUTfile << "/********************************************/" << "\n";
   OUTfile << "/***    V VELOCITY Initial Condition      ***/" << "\n";
   OUTfile << "/********************************************/" << "\n";

   for(int j=0; j<Numj; j++) {
      for(int I=0; I<NumI; I++) {
         OUTfile << VjI[j][I] << "\n";
      }
   }

   OUTfile.close();

}  // End of function writeOUTfile()




void writeDATfile(double** pJIResidual, double** UJiResidual, double** VjIResidual,
                  double** &Ujiavrg, double** &Vjiavrg, double** &Pjiavrg,
                  double** &UjiResidual_avrg, double** &VjiResidual_avrg, double** &PjiResidual_avrg,
                  double** &Vorticity_ji, int** BlockCellIndex, int outerIter)
{
   /*****************************************************************************
    This function first calls TransformColocate function to obtain velocity,
    pressure and vorticity values at the corners of the pressure cells. Than
    writes these values to a text file with extension DAT. These files can be
    opened by the Tecplot software and also by VFL to do internal post-procesing.
   *****************************************************************************/

   if (isTecplot == 0)
      return;

   string DATfileName;
   ofstream DATfile;

   stringstream ss;
   ss << outerIter;
   DATfileName = fullProblemName + "_" + ss.str() + ".dat";   // e.g. C:\VFL\Problems\MyProblem_1000.dat

   DATfile.open(DATfileName.data());

   TransformColocate(pJIResidual, UJiResidual, VjIResidual,
                     Ujiavrg, Vjiavrg, Pjiavrg, UjiResidual_avrg, VjiResidual_avrg,
                     PjiResidual_avrg, Vorticity_ji, BlockCellIndex);

   DATfile << "TITLE = \"Uvelocity_Vvelocity_Pressure\"\n";
   DATfile << "VARIABLES = X,Y,U,V,P,U_res,V_res,P_res,Vorticity\n";
   DATfile << "ZONE T =  \"ZONE 1\" ,i = " << Numi << ",j = " << Numj << ",F=POINT\n";
   DATfile << fixed << setprecision(10);

   for(int j=0; j<Numj; j++) {
      for(int i=0; i<Numi; i++) {
         DATfile << Xji[j][i] << " " << Yji[j][i] << " " <<
                    Ujiavrg[j][i] <<" " << Vjiavrg[j][i] << " " << Pjiavrg[j][i] << " " <<
                    UjiResidual_avrg[j][i] << " " << VjiResidual_avrg[j][i] << " " <<
                    PjiResidual_avrg[j][i] << " " << Vorticity_ji[j][i] << "\n";
      }
   }

   DATfile.close();

}  // End of function writeDATfile()
