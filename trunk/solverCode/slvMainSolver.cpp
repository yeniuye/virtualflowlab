/***********************************************************************************
  FLOW SOLVER of VIRTUAL FLOW LAB

  This flow solver is initially written
  
  by Mr. Gunes Nakiboglu (gunesnakib@gmail.com) for a masters degree study
  
  under the supervision of Dr. Cuneyt Sert (csert@metu.edu.tr)
  
  at Department of Mechanical Engrg., Middle East Technical Uni., Ankara, Turkey

***********************************************************************************/

#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "slvFunctions.h"
#include "slvGlobals.h"
#include "../guiCode/guiSolverThread.h"
#include "../guiCode/guiProblem.h"
#include "../guiCode/guiTypedefs.h"

using namespace std;

extern Problem *problem;
extern bool pauseSolveThread;
extern bool terminateSolveThread;
extern QWaitCondition continueSolveThread;


/**************************   IMPORTANT SOLVER VARIABLES   **************************

fullProblemName      Name of the CFD file including the folder, e.g. C:\VFL\Problems\MyProblem

numXcells            Number of cells in x direction
numYcells            Number of cells in y direction

Numi, NumI           Number of U and V velocity nodes in x direction
Numj, NumJ           Number of V and U velocity nodes in y direction

Xji, Yji             Coordinates of the corners of pressure cells
XJi, YJi             Coordinates of the West and East face centers of pressure cells
XjI, YjI             Coordinates of the South and North face centers of pressure cells
XJI, YJI             Coordinates of pressure cell centers

controlPointIndex    Control point indices
BlockCellIndex       Upper and lower indices of blocked cells (This follows Gunes' original implementation,
                     but seems a little bit weird now)

density              Density of the fluid
kinvis               Kinematic viscosity of the fluid

outerIterMax         Maximum outer iteration number
outerTolerance       This is the convergence criteria for mass flux

discSchm             Type of space discretization scheme. Can be CENTRAL, UPWIND, HYBRID, POWER-LAW
solverAlgorithm      Solver algorithm. Can be SIMPLE, SIMPLER, SIMPLEC
solverApproach       Solver aproach. Can be ITERATIVE, TIME MARCHING

isTransient          Determine whether the problem is transient
isTecplot            Flag for Tecplot file creation
isRestart            Flag for starting from a previous solution
timeStep             Time step used in the time marching approach
saveInterval         A Tecplot file will be created at every saveInterval iterations

pJI                  Pressure values
UJi                  u velocity values
VjI                  v velocity values
ScalarVar            Scalar variable values

pJIOLD               Pressure values of previous iteration
UJiOLD               u-velocity values of previous iteration
VjIOLD               v-velocity values of previous iteration

pRelax               Pressure relaxation parameter
uRelax               u velocity relaxation parameter
vRelax               v velocity relaxation parameter
scalarRelax          Scalar variable relaxation parameter

uRelaxTM             Relaxation parameter used in u momentum equation (TM: Used in time marching solution only)
vRelaxTM             Relaxation parameter used in v momentum equation                  ( " )
scalarRelaxTM        Relaxation parameter used in scalar variable equation             ( " )

pJIResidual          Pressure residual
UJiResidual          U velocity residual
VjIResidual          V velocity residual
ResidualArray[4]     Keeps all form of residuals in the main

nControlPoints       Number of track points
nBlockCells          Number of blocked cells
controlPointSaveInterval   Save interval for track points

BoundaryLeft         Boundary type and magnitude (if necessary) for left boundary
BoundaryRight        Boundary type and magnitude (if necessary) for right boundary
BoundaryTop          Boundary type and magnitude (if necessary) for top boundary
BoundaryBottom       Boundary type and magnitude (if necessary) for bottom boundary

UmomXConvFlux        Convective fluxes for U cells at West and East faces
UmomYConvFlux        Convective fluxes for U cells at South and North faces
VmomXConvFlux        Convective fluxes for V cells at West and East faces
VmomYConvFlux        Convective fluxes for V cells at South and North faces

UmomXDiffCond        Diffusive fluxes for U cells at West and East faces
UmomYDiffCond        Diffusive fluxes for U cells at South and North faces
VmomXDiffCond        Diffusive fluxes for V cells at West and East faces
VmomYDiffCond        Diffusive fluxes for V cells at South and North faces

UmomaWest            West side coefficients of U momentum matrix
UmomaEast            East side coefficients of U momentum matrix
UmomaSouth           South side coefficients of U momentum matrix
UmomaNorth           North side coefficients of U momentum matrix
UmomaCenter          Center coefficients of U momentum matrix
UmomaSource          Source coefficients of U momentum matrix

VmomaWest            West side coefficients of V momentum matrix
VmomaEast            East side coefficients of V momentum matrix
VmomaSouth           South side coefficients of V momentum matrix
VmomaNorth           North side coefficients of V momentum matrix
VmomaCenter          Center coefficients of V momentum matrix
VmomaSource          Source coefficients of V momentum matrix

PJICorrect           Pressure correction values
PresCrctWest         West side coefficients of pressure correction matix
PresCrctEast         West side coefficients of pressure correction matrix
PresCrctSouth        South side coefficients of pressure correction matrix
PresCrctNorth        North side coefficients of pressure correction matrix
PresCrctCenter       Center coefficients of pressure correction matrix
PresCrctSource       Source coefficients of pressure correction matrix
PresCrctdJi          Coefficient arrays used in the determination of pressure correction matrix coefficients
PresCrctdjI          Coefficient arrays used in the determination of pressure correction matrix coefficients

UJi_pseudo           U pseudo velocity                           (SIMPLER algorithm only)
VjI_pseudo           V pseudo velocity                                    ( " )
UmomaSource_pseudo   Source term for U pseudo velocity matrix             ( " )
VmomaSource_pseudo   Source term for V pseudo velocity matrix             ( " )
PresWest             West side coefficients of pressure matix             ( " )
PresEast             West side coefficients of pressure matrix            ( " )
PresSouth            South side coefficients of pressure matrix           ( " )
PresNorth            North side coefficients of pressure matrix           ( " )
PresCenter           Center coefficients of pressure matrix               ( " )
PresSource           Source coefficients of pressure matrix               ( " )

Ujiavrg              U velocity values interpolated for collocated grid arrangement
Vjiavrg              V velocity values interpolated for collocated grid arrangement
Pjiavrg              Pressure values interpolated for collocated grid arrangement

UjiResidual_avrg     U velocity residual values interpolated for collocated grid arrangement
VjiResidual_avrg     V velocity residual values interpolated for collocated grid arrangement
PjiResidual_avrg     Pressure residual values interpolated for collocated grid arrangement
Vorticity_ji         Vorticity values interpolated for collocated grid arrangement

************************************************************************************/




/**************************   GLOBAL SOLVER VARIABLES   **************************/
// These variables are used in the slvGlobals.h file with keyword extern.
// Therefore all other slv...cpp files can access them.

string fullProblemName;

double density, kinvis;

int numXcells, numYcells;

int Numi, Numj, NumI, NumJ;

int discSchm, solverAlgorithm, solverApproach;

int outerIterMax;

double outerTolerance;

int saveInterval, controlPointSaveInterval;

double timeStep;

bool isTecplot, isRestart, isTransient;

double **pJI, **UJi, **VjI, **ScalarVar;

double pRelax, uRelax, vRelax, scalarRelax;

int nControlPoints, nBlockCells;

double **Xji, **Yji, **XJi, **YJi, **XjI, **YjI, **XJI, **YJI;

double **BoundaryLeft, **BoundaryRight, **BoundaryTop, **BoundaryBottom;

/**********************   END OF GLOBAL SOLVER VARIABLES   ***********************/



void SolverThread::mainSolver()
{
   QString message;

   double ResidualArray[4];

   time_t ComptSecStart;
   time_t ComptSecStop;
   clock_t startTime, endTime;
   double cpu_time_used;

   int** controlPointIndex;
   int** BlockCellIndex;

   // Convective flux and diffusive conductance arrays for u momentum equation
   double** UmomXConvFlux;
   double** UmomYConvFlux;
   double** UmomXDiffCond;
   double** UmomYDiffCond;

   // Convective flux and diffusive conductance arrays for v momentum equation
   double** VmomXConvFlux;
   double** VmomYConvFlux;
   double** VmomXDiffCond;
   double** VmomYDiffCond;

   // Convective flux and diffusive conductance arrays for scalar variable equation
   double** ScalarXConvFlux;
   double** ScalarYConvFlux;
   double** ScalarXDiffCond;
   double** ScalarYDiffCond;

   // Coefficients of u momentum equation
   double** UmomaWest;
   double** UmomaEast;
   double** UmomaSouth;
   double** UmomaNorth;
   double** UmomaCenter;
   double** UmomaSource;

   // Coefficients of v momentum equation
   double** VmomaWest;
   double** VmomaEast;
   double** VmomaSouth;
   double** VmomaNorth;
   double** VmomaCenter;
   double** VmomaSource;

   // Coefficients of scalar variable conservation equation
   double** ScalaraWest;
   double** ScalaraEast;
   double** ScalaraSouth;
   double** ScalaraNorth;
   double** ScalaraCenter;
   double** ScalaraSource;

   // Coefficients of pressure correction equation
   double** PJICorrect;
   double** PresCrctWest;
   double** PresCrctEast;
   double** PresCrctSouth;
   double** PresCrctNorth;
   double** PresCrctCenter;
   double** PresCrctSource;
   double** PresCrctdJi;
   double** PresCrctdjI;

   // These variables are only used in TIME MARCHING approach
   double** uRelaxTM;
   double** vRelaxTM;
   double** scalarRelaxTM;

   // These variables are only used in SIMPLER Algorithm
   double** UJi_pseudo;
   double** VjI_pseudo;
   double** UmomaSource_pseudo;
   double** VmomaSource_pseudo;
   double** PresWest;
   double** PresEast;
   double** PresSouth;
   double** PresNorth;
   double** PresCenter;
   double** PresSource;

   // Primary variables of the previous iteration that are used in residual calculation
   double** pJIOLD;
   double** UJiOLD;
   double** VjIOLD;

   // Variables to generate residual plots
   double** pJIResidual;
   double** UJiResidual;
   double** VjIResidual;

   // Variables used in staggered to collocated grid transformation
   double** Ujiavrg;
   double** Vjiavrg;
   double** Pjiavrg;
   double** UjiResidual_avrg;
   double** VjiResidual_avrg;
   double** PjiResidual_avrg;
   double** Vorticity_ji;


   int ConvergenceFlag = 0;
   double DivergenceLimit = 100000;
   
   double ThermalConduct = 5;  // Not used in this version of the solver

   fullProblemName = problem->getCfdFileName();     // e.g. C:\VFL\Problems\MyProblem.cfd
   fullProblemName.resize(fullProblemName.length() - 4);  // Get rid of the .cfd extension

   message = QString(fullProblemName.c_str());
   emit appendOutput(message, Qt::black);

   // Timer is started here to calculate the computation time
   ComptSecStart = time(NULL);
   startTime = clock();

   // Read the input (.INP) file
   int readFlag = ReadInputFile(controlPointIndex, BlockCellIndex);

   if (readFlag == -1) {  // Mesh parameter error in the input file.
      emit appendOutput(tr("ERROR: Mesh parameter error in the input file. Solution can not start."), Qt::red);
      return;
   }

   if (readFlag == -2) {  // Restart file error.
      emit appendOutput(tr("ERROR: Restart file does not exist. Solution can not start."), Qt::red);
      return;
   }

   // Create coordinate points that are needed in the staggered arangement
   CoordinateCreater();

   TotalMemoryAllocate(UmomXConvFlux, UmomYConvFlux, UmomXDiffCond, UmomYDiffCond, VmomXConvFlux, VmomYConvFlux,
                       VmomXDiffCond, VmomYDiffCond, ScalarXConvFlux, ScalarYConvFlux, ScalarXDiffCond, ScalarYDiffCond,
                       UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter, UmomaSource, VmomaWest, VmomaEast,
                       VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource, ScalaraWest, ScalaraEast, ScalaraSouth,
                       ScalaraNorth, ScalaraCenter, ScalaraSource, PJICorrect, PresCrctWest, PresCrctEast, PresCrctSouth,
                       PresCrctNorth, PresCrctCenter,PresCrctSource, PresCrctdJi,PresCrctdjI, uRelaxTM, vRelaxTM,
                       scalarRelaxTM, UJi_pseudo,VjI_pseudo, UmomaSource_pseudo, VmomaSource_pseudo, PresWest, PresEast,
                       PresSouth, PresNorth, PresCenter, PresSource, pJIOLD, UJiOLD, VjIOLD,
                       pJIResidual, UJiResidual, VjIResidual, Ujiavrg, Vjiavrg, Pjiavrg, UjiResidual_avrg, VjiResidual_avrg,
                       PjiResidual_avrg, Vorticity_ji);

   // This function calculates the relaxation parameters used in U & V momentum equations.
   InitilizeRelaxationParameters(uRelaxTM, vRelaxTM, scalarRelaxTM);

   // This function applies the specified velocity BC. This is the only place this kind of BC is treated.
   SpecifiedVelBC ();


   // This part is not used in the current version of the solver.
   for (int I=0; I<NumI; I++) {
      ScalarVar[0][I] = 5;         // TEMPORAL VALUE
   }

   // If everything goes fine upto here than the solution is really started.
   appendOutput(tr("Solution is started."), Qt::black);
   sendRunStatus(tr("RUNNING"));




   //*****************************************************
   // Outer iterations start here
   //*****************************************************

   int outerIter;
   for (outerIter = 1; outerIter <= outerIterMax; outerIter++) {

      // Calculate convective fluxes for u and v momentum equations.
      ConvFluxCalcUmom(UmomXConvFlux, UmomYConvFlux);
      ConvFluxCalcVmom(VmomXConvFlux, VmomYConvFlux);

      // Calculate convective fluxes for scalar variable equation.
      // NOTE: Scalar variable solution is not fully performed in this version of the solver.
      ConvFluxCalcScalar(ScalarXConvFlux, ScalarYConvFlux);

      // Calculate diffusive fluxes for u and v momentum equations.
      DiffConducCalcUmom(UmomXDiffCond, UmomYDiffCond);
      DiffConducCalcVmom(VmomXDiffCond, VmomYDiffCond);

      // Calculate diffusive fluxes for scalar variable equation.
      // NOTE: Scalar variable solution is not fully performed in this version of the solver.
      DiffConducCalcScalar(ScalarXDiffCond, ScalarYDiffCond, ThermalConduct);

      // Form the coefficient matrix for U and V momentum equations
      UmomMatrixFormer(UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter, UmomaSource,
                       UmomXConvFlux, UmomYConvFlux, UmomXDiffCond, UmomYDiffCond,
                       uRelaxTM, BlockCellIndex);
      VmomMatrixFormer(VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource,
                       VmomXConvFlux, VmomYConvFlux, VmomXDiffCond, VmomYDiffCond,
                       vRelaxTM, BlockCellIndex);




      if (solverApproach == 2) {  // For TIME MARCHING solution
         // Calculate relaxation parameters that will be used in time marching solution
         CalTimeRelaxationForMomentumEqn(uRelaxTM, vRelaxTM, UmomaCenter, VmomaCenter);
      }




      /***********************************************************************
        This part is for SIMPLER algorithm only
      ***********************************************************************/
      if (solverAlgorithm == 2) {
         // Calculate pseudo velocities in x and y directions.
         pseudoUcalculator(UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter, UmomaSource,
                           UJi_pseudo, UmomaSource_pseudo);
         pseudoVcalculator(VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource,
                           VjI_pseudo, VmomaSource_pseudo);

         // Take a copy of pressure values of the current iteration for residual calculation.
         copyPressure(pJIOLD);

         // Calculate coefficients of pressure equation (NOT pressure correction equation. Function name is misleading)
         // The last argument is 0, which means that we're working on pressure, NOT pressure correction.
         PressureCorrectionMatrix(PresWest, PresEast, PresSouth, PresNorth, PresCenter, PresSource,
                                  UmomaCenter, VmomaCenter, UJi_pseudo, VjI_pseudo,
                                  PresCrctdJi, PresCrctdjI,
                                  UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, 0);

         // Solve pressure matrix
         LineTDMASolver(PresNorth, PresSouth, PresWest, PresEast, PresCenter, PresSource, NumI, NumJ, pJI, 4, 1);

         // Update source coefficients of u and v momentum equations.
         SourceModifierSimpler(UmomaSource, VmomaSource, UmomaSource_pseudo, VmomaSource_pseudo);
      }




      // Solve u momentum matrix
      LineTDMASolver(UmomaNorth, UmomaSouth, UmomaWest, UmomaEast, UmomaCenter, UmomaSource, Numi, NumJ, UJi, 4, 1);

      // Solve v momentum matrix
      LineTDMASolver(VmomaNorth, VmomaSouth, VmomaWest, VmomaEast, VmomaCenter, VmomaSource, NumI, Numj, VjI, 4, 2);

      // Outlet BC (Part 1/4)
      OutletBC();

      // Calculate coefficients of pressure correction equation
      // The last argument is 1, which means that we're working on pressure correction, NOT pressure.
      PressureCorrectionMatrix(PresCrctWest, PresCrctEast, PresCrctSouth, PresCrctNorth,
                               PresCrctCenter, PresCrctSource, UmomaCenter, VmomaCenter,
                               UJi, VjI, PresCrctdJi, PresCrctdjI, UmomaWest, UmomaEast, UmomaSouth, UmomaNorth,
                               VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, 1);

      // Solve pressure correction matrix
      LineTDMASolver(PresCrctNorth, PresCrctSouth, PresCrctWest, PresCrctEast, PresCrctCenter, PresCrctSource, NumI, NumJ, PJICorrect, 4, 1);




      /***********************************************************************
        This part is for SIMPLE and SIMPLEC algorithms only
      ***********************************************************************/
      if (solverAlgorithm == 1 || solverAlgorithm == 3) {
         // Take a copy of pressure values of the current iteration for residual calculation.
         copyPressure(pJIOLD);

         // Perform pressure correction
         PressureCorrector(PJICorrect);
      }




      // Take a copy of velocity values of the current iteration for residual calculation.
      copyVelocity(UJiOLD, VjIOLD);

      // Perform velocity correction and calculate velocity residuals
      VelocityCorrector(PJICorrect, PresCrctdJi, PresCrctdjI);




      // Calculate residual changes of all primary variables and perform convergence/divergence check
      CalculateResidual(pJIOLD, UJiOLD, VjIOLD,
                        PresCrctSource, DivergenceLimit,
                        ConvergenceFlag, ResidualArray, pJIResidual, UJiResidual, VjIResidual);




      // NOTE: Scalar variable equation is NOT solved in this version of the code.
      ScalarVarMatrixFormer(ScalaraWest, ScalaraEast,ScalaraSouth, ScalaraNorth, ScalaraCenter, ScalaraSource,
                            ScalarXConvFlux, ScalarYConvFlux, ScalarXDiffCond, ScalarYDiffCond,
                            ScalarVar, ThermalConduct, scalarRelaxTM);

      // Outlet BC for scalar variable (Part 2/2)
      for(int J=1; J<NumJ-1; J++) {
         ScalarVar[J][NumI-1] = ScalarVar[J][NumI-2];
      }




      // Write Tecplot (DAT) and OUT files
      if (outerIter%saveInterval == 0) {
         if (outerIter > 0) {
            writeDATfile(pJIResidual, UJiResidual, VjIResidual, Ujiavrg,
                         Vjiavrg, Pjiavrg, UjiResidual_avrg, VjiResidual_avrg, PjiResidual_avrg, Vorticity_ji,
                         BlockCellIndex, outerIter);

            writeOUTfile(outerIter);
         }
      }




      /***********************************************************************
        Check for convergence.
      ***********************************************************************/
      if (ConvergenceFlag == 1) {  // Solution is converged.
         // If convergence is achieved create another DAT file.
         // Cuneyt: It is possible that this DAT file is already created above.
         writeDATfile(pJIResidual, UJiResidual, VjIResidual, Ujiavrg,
                      Vjiavrg, Pjiavrg, UjiResidual_avrg, VjiResidual_avrg, PjiResidual_avrg, Vorticity_ji,
                      BlockCellIndex, outerIter);

         writeOUTfile(outerIter);

         message = QString(tr("The solution converged at iteration %1.")).arg(outerIter);
         emit appendOutput(message, Qt::black);
         emit sendRunStatus(QString(tr("CONVERGED")));
         emit sendIteration(outerIter);

         break;  // Break outer iteration loop.

      } else if (ConvergenceFlag == 2) {  // Solution is diverged
         message = QString(tr("The solution diverged at iteration %1.")).arg(outerIter);
         emit appendOutput(message, Qt::black);
         emit sendRunStatus(QString(tr("DIVERGED")));
         emit sendIteration(outerIter);

         break;  // Break outer iteration loop.
      }


      // If Pause button is clicked
      mutex.lock();
      if (pauseSolveThread) {
         message = QString(tr("Solution is paused at iteration %1.")).arg(outerIter);
         emit appendOutput(message, Qt::black);
         continueSolveThread.wait(&mutex);
      }
      mutex.unlock();

      // If Terminate button is clicked
      mutex.lock();
      if (terminateSolveThread) {
         mutex.unlock();
         message = QString(tr("Solution is terminated at iteration %1.")).arg(outerIter);
         emit appendOutput(message, Qt::black);
         break;
      }
      mutex.unlock();


      emit sendIteration(outerIter);

      
      /***********************************************************************
       Update residual control point plots
      ***********************************************************************/
      // Cuneyt: Is the following if statement inefficient ?
      if (outerIter == 1 || outerIter%problem->getPlotUpdateInterval() == 0) {
         // Send residual data to plot.
         emit sendResidual(outerIter, ResidualArray[2], ResidualArray[3], ResidualArray[0], ResidualArray[1]);

         for(int id=0; id<nControlPoints; id++) {
            problem->mesh->blocks[0].controlPointData[id][0] = UJi[controlPointIndex[id][0]][controlPointIndex[id][1]];
            problem->mesh->blocks[0].controlPointData[id][1] = VjI[controlPointIndex[id][0]][controlPointIndex[id][1]];
            problem->mesh->blocks[0].controlPointData[id][2] = pJI[controlPointIndex[id][0]][controlPointIndex[id][1]];
         }
         emit sendControlPointData(outerIter);
      }

   }  // End of the outer iterations (outIter loop)


   if (outerIter == outerIterMax + 1) {
      message = QString(tr("Iterations are done but convergence is not achieved."));
      emit appendOutput(message, Qt::black);
      emit sendRunStatus(QString(tr("DONE")));
   }


   // Stop computation timer
   ComptSecStop = time (NULL);
   endTime = clock();
   cpu_time_used = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;


   TotalMemoryDeAllocate(pJIOLD, UJiOLD, VjIOLD, UmomXConvFlux, UmomYConvFlux, UmomXDiffCond, UmomYDiffCond, VmomXConvFlux,
                         VmomYConvFlux, VmomXDiffCond, VmomYDiffCond, UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter,
                         UmomaSource, VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource, PJICorrect, PresCrctWest,
                         PresCrctEast, PresCrctSouth, PresCrctNorth, PresCrctCenter, PresCrctSource, PresCrctdJi, PresCrctdjI,
                         UJi_pseudo,VjI_pseudo, UmomaSource_pseudo, VmomaSource_pseudo, PresWest, PresEast, PresSouth, PresNorth,
                         PresCenter, PresSource, uRelaxTM, vRelaxTM, Ujiavrg, Vjiavrg, Pjiavrg, UjiResidual_avrg,
                         VjiResidual_avrg, PjiResidual_avrg, Vorticity_ji);

}  // End of function mainSolver()
