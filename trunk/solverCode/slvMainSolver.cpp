/****************************************************************************/
/*                   FLOW SOLVER OF THE VIRTUAL FLOW LAB                    */
/****************************************************************************/

#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <string>
#include <stdio.h>

#include "slvFunctions.h"
#include "slvSolverThread.h"
#include "../guiCode/guiProblem.h"
#include "../guiCode/guiTypedefs.h"

using namespace std;

extern Problem *problem;
extern bool pauseSolveThread;
extern bool terminateSolveThread;
extern QWaitCondition continueSolveThread;


/*************************   VARIABLE LIST   ***********************/

// NOTE : The variables listed below are the variables that are defined in
// the main only. There are also local variables used in the calculations.

// NumXcells            --> Number of cells in x direction
// NumYcells            --> Number of cells in y direction

// Numi                 --> Number of U velocity nodes in x direction
// Numj                 --> Number of V velocity nodes in y direction
// NumI                 --> Number of V velocity nodes in x direction
// NumJ                 --> Number of U velocity nodes in y direction

// GlobIte              --> Global Iteration Numbers
// SaveInterval         --> The number of iterations that will pass between two tecplot files
// Ro                   --> Density of the fluid
// Kinvis               --> Kinematic Viscosity

// PresRelax            --> Pressure relaxation parameter
// UvelRelax            --> U velocity relaxation parameter
// VvelRelax            --> V velocity relaxation parameter
// ScalarRelax          --> Scalar variable relaxation parameter

// OuterTolerance       --> This is the convergence criteria for mass flux.
// DiffSchm             --> Type of differencing scheme (Central,upwind,hybrid and power-law)
// SolverType           --> Solver Algorithm (SIMPLE,SIMPLER,SIMPLEC)
// SolverApproach       --> Solver Aproach (ITERATIVE,TIME MARCHING)
// ResidualArray[4]     --> Keeps all form of residuals in the main
// TimeStep             --> Time step used in the time marching approach

// isTecplot            --> Determine whether Tecplot files will be created
// isTransient          --> Determine whether the problem is transient

// nTrackPoints         --> Number of track points used in the problem
// trackPointSaveInterval   Save interval for track points
// nBlockCells          --> Number of block cells

// XjicoorCorners       --> x coordinates of the corners of the pressure cells
// YjicoorCorners       --> y coordinates of the corners of the pressure cells
// XJicoorSideFaces     --> x coordinates of the west and east face centers of the pressure cells
// YJicoorSideFaces     --> y coordinates of the west and east face centers of the pressure cells
// XjIcoorFrontFaces    --> x coordinates of the south and north face centers of the pressure cells
// YjIcoorFrontFaces    --> y coordinates of the south and north face centers of the pressure cells
// XJIcoorCenter        --> x coordinate of the pressure cell centers (nodes)
// YJIcoorCenter        --> y coordinate of the pressure cell centers (nodes)

// PJIpressure          --> Pressure values
// UJivelocity          --> U velocity values
// VjIvelocity          --> V velocity values
// ScalarVar            --> Scalar variable values

// TrackPointYX         --> Coordinates of the track cells
// BlockCellCoor        --> Coordinates of the block cells

// BoundaryLeft         --> Boundary type and magnitude (if necessary) for left boundary
// BoundaryRight        --> Boundary type and magnitude (if necessary) for right boundary
// BoundaryTop          --> Boundary type and magnitude (if necessary) for top boundary
// BoundaryBottom       --> Boundary type and magnitude (if necessary) for bottom boundary

// UmomXConvFlux        --> Convective fluxes for U cells in west and east faces
// UmomXConvFlux        --> Convective fluxes for U cells in south and north faces
// UmomXDiffCond        --> Diffusive fluxes for U cells in west and east faces
// UmomYDiffCond        --> Diffusive fluxes for U cells in south and north faces
// VmomXConvFlux        --> Convective fluxes for V cells in west and east faces
// VmomYConvFlux        --> Convective fluxes for V cells in south and north faces
// VmomXDiffCond        --> Diffusive fluxes for V cells in west and east faces
// VmomYDiffCond        --> Diffusive fluxes for V cells in south and north faces

// UmomaWest            --> West side coefficients of the U momentum matrix
// UmomaEast            --> East side coefficients of the U momentum matrix
// UmomaSouth           --> South side coefficients of the U momentum matrix
// UmomaNorth           --> North side coefficients of the U momentum matrix
// UmomaCenter          --> Center coefficients of the U momentum matrix
// UmomaSource          --> Source coefficients of the U momentum matrix

// VmomaWest            --> West side coefficients of the V momentum matrix
// VmomaEast            --> East side coefficients of the V momentum matrix
// VmomaSouth           --> South side coefficients of the V momentum matrix
// VmomaNorth           --> North side coefficients of the V momentum matrix
// VmomaCenter          --> Center coefficients of the V momentum matrix
// VmomaSource          --> Source coefficients of the V momentum matrix

// PJICorrect           --> Pressure correction values
// PresCrctWest         --> West side coefficients of the pressure correction matix
// PresCrctEast         --> West side coefficients of the pressure correction matrix
// PresCrctSouth        --> South side coefficients of the pressure correction matrix
// PresCrctNorth        --> North side coefficients of the pressure correction matrix
// PresCrctCenter       --> Center coefficients of the pressure correction matrix
// PresCrctSource       --> Source coefficients of the pressure correction matrix
// PresCrctdJi          --> Coefficient arrays used in the determination of the pressure correction matrix coefficients
// PresCrctdjI          --> Coefficient arrays used in the determination of the pressure correction matrix coefficients

// TimeURelaxPar        --> Relaxation parameters used in the U momentum equation
// TimeVRelaxPar        --> Relaxation parameters used in the V momentum equation
// TimeScaRelaxPar      --> Relaxation parameters used in the scalar variable equation

// UJi_pse_vel          --> U Pseudo velocity (SIMPLER-Algorithm only)
// VjI_pse_vel          --> V Pseudo velocity (SIMPLER-Algorithm only)
// UmomaSource_Pseu     --> Source term for U Pseudo velocity matrix (SIMPLER-Algorithm only)
// VmomaSource_Pseu     --> Source term for V Pseudo velocity matrix (SIMPLER-Algorithm only)
// PresWest             --> West side coefficients of the pressure matix (SIMPLER-Algorithm only)
// PresEast             --> West side coefficients of the pressure matrix (SIMPLER-Algorithm only)
// PresSouth            --> South side coefficients of the pressure matrix (SIMPLER-Algorithm only)
// PresNorth            --> North side coefficients of the pressure matrix (SIMPLER-Algorithm only)
// PresCenter           --> Center coefficients of the pressure matrix (SIMPLER-Algorithm only)
// PresSource           --> Source coefficients of the pressure matrix (SIMPLER-Algorithm only)

// PJIpressureOLD       --> Pressure values of previous iteration
// UJivelocityOLD       --> u-velocity values of previous iteration
// VjIvelocityOLD       --> v-velocity values of previous iteration

// PJIResidual          --> Pressure residual
// UJiResidual          --> u-velocity residual
// VjIResidual          --> v-velocity residual
// Ujiavrg              --> u-velocity values interpolated for collocated grid arrangement
// Vjiavrg              --> v-velocity values interpolated for collocated grid arrangement
// Pjiavrg              --> pressure values interpolated for collocated grid arrangement
// UjiResidual_avrg     --> u-velocity residual values interpolated for collocated grid arrangement
// VjiResidual_avrg     --> v-velocity residual values interpolated for collocated grid arrangement
// PjiResidual_avrg     --> pressure residual values interpolated for collocated grid arrangement
// Vorticity_ji         --> vorticity values interpolated for collocated grid arrangement
/****************************************************************************/


void SolverThread::mainSolver()
{
   QString message;

   int NumXcells,NumYcells;
   int Numi,Numj,NumI,NumJ;

   int GlobIte;
   int SaveInterval;
   double Ro;
   double Kinvis;
   double PresRelax;
   double UvelRelax;
   double VvelRelax;
   double ScalarRelax;
   double OuterTolerance;

   int DiffSchm;
   int SolverType;
   int SolverApproach;

   string RunID;
   double ResidualArray[4];
   double TimeStep;

   bool isTecplot;
   bool isRestart;
   bool isTransient;
   int nTrackPoints;
   int trackPointSaveInterval;
   int nBlockCells;

   time_t ComptSecStart;
   time_t ComptSecStop;
   clock_t startTime, endTime;
   double cpu_time_used;

   double** XjicoorCorners;
   double** YjicoorCorners;
   double** XJicoorSideFaces;
   double** YJicoorSideFaces;
   double** XjIcoorFrontFaces;
   double** YjIcoorFrontFaces;
   double** XJIcoorCenter;
   double** YJIcoorCenter;

   double** PJIpressure;
   double** UJivelocity;
   double** VjIvelocity;
   double** ScalarVar;

   int** TrackPointYX;
   int** BlockCellCoor;

   /****  BOUNDARY CONDITION VARIABLES    ****/
   double** BoundaryLeft;
   double** BoundaryRight;
   double** BoundaryTop;
   double** BoundaryBottom;

   double ThermalConduct =5 ;
   int ConvergenceFlag = 0;
   double DivergenceLimit = 100000;

   string Directory;
   Directory = problem->getCfdFileName();
   Directory.resize(Directory.length()-4);

   message = QString(Directory.c_str());
   emit appendOutput(message, Qt::black);

   // Timer is started here to calculate the computation time
   ComptSecStart = time(NULL);
   startTime = clock();

   // Read the input (.INP) file
   int readFlag = ReadInputFile(XjicoorCorners, YjicoorCorners, NumXcells,  NumYcells, nTrackPoints, nBlockCells,
                                BoundaryLeft, BoundaryRight, BoundaryTop, BoundaryBottom, PJIpressure, UJivelocity,
                                VjIvelocity, ScalarVar, TrackPointYX, BlockCellCoor, Ro, Kinvis, GlobIte,  PresRelax,
                                UvelRelax, VvelRelax, ScalarRelax, Numi, Numj, NumI, NumJ, DiffSchm, SaveInterval,
                                SolverType, SolverApproach, TimeStep, OuterTolerance, trackPointSaveInterval,
                                isTecplot, isTransient, isRestart, Directory);

   if (readFlag == -1) {  // Mesh parameter error in the input file.
      emit appendOutput(tr("ERROR: Mesh parameter error in the input file. Solution can not start."), Qt::red);
      return;
   }

   if (readFlag == -2) {  // Restart file error.
      emit appendOutput(tr("ERROR: Restart file does not exist. Solution can not start."), Qt::red);
      return;
   }

   /* Creates the coordinate points that are needed in the staggered
   arangement other than the coorners of the pressure cells */
   CoordinateCreater(XjicoorCorners, YjicoorCorners, NumXcells, NumYcells, Numi, Numj, NumI, NumJ, XJicoorSideFaces,
                     YJicoorSideFaces, XjIcoorFrontFaces, YjIcoorFrontFaces, XJIcoorCenter, YJIcoorCenter);

   /* Convective flux and diffusive conductance arrays for the U momentum equations */
   double** UmomXConvFlux;
   double** UmomYConvFlux;
   double** UmomXDiffCond;
   double** UmomYDiffCond;

   /* Convective flux and diffusive conductance arrays for the V momentum equations */
   double** VmomXConvFlux;
   double** VmomYConvFlux;
   double** VmomXDiffCond;
   double** VmomYDiffCond;

   /* Convective flux and diffusive conductance arrays for the Scalar variable equations */
   double** ScalarXConvFlux;
   double** ScalarYConvFlux;
   double** ScalarXDiffCond;
   double** ScalarYDiffCond;

   /* The coefficients of U momentum equations */
   double** UmomaWest;
   double** UmomaEast;
   double** UmomaSouth;
   double** UmomaNorth;
   double** UmomaCenter;
   double** UmomaSource;

   /* The coefficients of V momentum equations */
   double** VmomaWest;
   double** VmomaEast;
   double** VmomaSouth;
   double** VmomaNorth;
   double** VmomaCenter;
   double** VmomaSource;

   /* The coefficients of V momentum equations */
   double** ScalaraWest;
   double** ScalaraEast;
   double** ScalaraSouth;
   double** ScalaraNorth;
   double** ScalaraCenter;
   double** ScalaraSource;

   /* The coefficients of pressure correction equations */
   double** PJICorrect;
   double** PresCrctWest;
   double** PresCrctEast;
   double** PresCrctSouth;
   double** PresCrctNorth;
   double** PresCrctCenter;
   double** PresCrctSource;
   double** PresCrctdJi;
   double** PresCrctdjI;

   // This variables are only used in TIME MARCHING approach
   double** TimeURelaxPar;
   double** TimeVRelaxPar;
   double** TimeScaRelaxPar;

   //  These variables are only used in SIMPLER ALGORITM
   double** UJi_pse_vel;
   double** VjI_pse_vel;
   double** UmomaSource_Pseu;
   double** VmomaSource_Pseu;
   double** PresWest;
   double** PresEast;
   double** PresSouth;
   double** PresNorth;
   double** PresCenter;
   double** PresSource;

   /* These variables are used to keep track of the primary
   variables of the previous iteration which will be used
   in the residul calculation */
   double** PJIpressureOLD;
   double** UJivelocityOLD;
   double** VjIvelocityOLD;

   /* Memory for the Residual Plots */
   double** PJIResidual;
   double** UJiResidual;
   double** VjIResidual;

   /* These variables are used in staggered to collocated grid transformation */
   double** Ujiavrg;
   double** Vjiavrg;
   double** Pjiavrg;
   double** UjiResidual_avrg;
   double** VjiResidual_avrg;
   double** PjiResidual_avrg;
   double** Vorticity_ji;

   TotalMemoryAllocate(UmomXConvFlux, UmomYConvFlux, UmomXDiffCond, UmomYDiffCond, VmomXConvFlux, VmomYConvFlux,
                       VmomXDiffCond, VmomYDiffCond, ScalarXConvFlux, ScalarYConvFlux, ScalarXDiffCond, ScalarYDiffCond,
                       UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter, UmomaSource, VmomaWest, VmomaEast,
                       VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource, ScalaraWest, ScalaraEast, ScalaraSouth,
                       ScalaraNorth, ScalaraCenter, ScalaraSource, PJICorrect, PresCrctWest, PresCrctEast, PresCrctSouth,
                       PresCrctNorth, PresCrctCenter,PresCrctSource, PresCrctdJi,PresCrctdjI, TimeURelaxPar, TimeVRelaxPar,
                       TimeScaRelaxPar, UJi_pse_vel,VjI_pse_vel, UmomaSource_Pseu, VmomaSource_Pseu, PresWest, PresEast,
                       PresSouth, PresNorth, PresCenter, PresSource, PJIpressureOLD, UJivelocityOLD, VjIvelocityOLD,
                       PJIResidual, UJiResidual, VjIResidual, Ujiavrg, Vjiavrg, Pjiavrg, UjiResidual_avrg, VjiResidual_avrg,
                       PjiResidual_avrg, Vorticity_ji, Numj, NumJ, Numi, NumI);

   /* This function calculates the relaxation parameters used in
   the U & V momentum equations. */
   InitilizeRelaxationParameters(Numi, Numj, NumI, NumJ, TimeURelaxPar, TimeVRelaxPar, TimeScaRelaxPar,
                                 SolverApproach, UvelRelax, VvelRelax, ScalarRelax);

   /* This function apply the Specified boundary condition to the domain
   if there exist any     SPECIFIED VELOCITY [1/1] = Means to apply a
   specified boundary condition only one modification is needed
   and it is here */
   SpecVelBoundCond (BoundaryLeft, BoundaryRight, BoundaryTop, BoundaryBottom, Numj, NumJ, Numi, NumI,
                     UJivelocity, VjIvelocity);


   // This part is not used in the current version for scalar variable solution
   for (int I=0;I<NumI;I++) {
      ScalarVar[0][I]=5;         // TEMPORAL VALUE
   }

   // If everything goes fine upto here than the solution is really started.
   appendOutput(tr("Solution is started."), Qt::black);
   sendRunStatus(tr("RUNNING"));


   //*****************************************************
   // Global iterations start here
   //*****************************************************

   int OutIte;
   for (OutIte=1; OutIte < GlobIte+1; OutIte++) {
      /* Calculates the convective fluxes for U momentum equations */
      ConvFluxCalcUmom(UmomXConvFlux, UmomYConvFlux, UJivelocity, VjIvelocity, Numi, Numj, NumI, NumJ, Ro);

      /* Calculates the convective fluxes for V momentum equations */
      ConvFluxCalcVmom(VmomXConvFlux, VmomYConvFlux, UJivelocity, VjIvelocity, Numi, Numj, NumI, NumJ, Ro);

      /* This subroutine is not being used in this version */
      /* Calculates the convective fluxes for Scalar variable equations */
      ConvFluxCalcScalar(ScalarXConvFlux, ScalarYConvFlux, UJivelocity, VjIvelocity, Numi, Numj, NumI, NumJ, Ro);

      /* Calculates the diffusive fluxes for U momentum equations */
      DiffConducCalcUmom(UmomXDiffCond, UmomYDiffCond, XJicoorSideFaces, YJicoorSideFaces, Numi, Numj, NumI, NumJ, Kinvis);

      /* Calculates the diffusive fluxes for V momentum equations */
      DiffConducCalcVmom(VmomXDiffCond, VmomYDiffCond, XjIcoorFrontFaces, YjIcoorFrontFaces, Numi, Numj, NumI, NumJ, Kinvis);

      /* This subroutine is not being used in this version */
      /* Calculates the diffusive fluxes for Scalar Variable equations */
      DiffConducCalcScalar(ScalarXDiffCond, ScalarYDiffCond, XJIcoorCenter, YJIcoorCenter, Numi, Numj, NumI, NumJ, ThermalConduct);

      /* This function forms the coefficient matrix for the U momentum equation */
      UmomMatrixFormer(UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter, UmomaSource, Numi,
                       NumJ, UmomXConvFlux, UmomYConvFlux, YjIcoorFrontFaces, XjIcoorFrontFaces,
                       YjicoorCorners, YJicoorSideFaces, UmomXDiffCond, UmomYDiffCond, PJIpressure,
                       UJivelocity, Kinvis, DiffSchm, TimeURelaxPar, BoundaryLeft, BoundaryRight,
                       BoundaryTop, BoundaryBottom, Ro, XJIcoorCenter, TimeStep, isTransient,
                       nBlockCells, BlockCellCoor);

      /* This function forms the coefficient matrix for the V momentum equation */
      VmomMatrixFormer(VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource,
                       NumI, Numj, VmomXConvFlux, VmomYConvFlux, YJicoorSideFaces, XJicoorSideFaces,
                       XjicoorCorners, XjIcoorFrontFaces, VmomXDiffCond, VmomYDiffCond, PJIpressure,
                       VjIvelocity, Kinvis, DiffSchm, TimeVRelaxPar, BoundaryLeft, BoundaryRight,
                       BoundaryTop, BoundaryBottom, Ro, YJIcoorCenter, TimeStep, isTransient,
                       nBlockCells,BlockCellCoor);

      if (SolverApproach == 2) {  // If Time Marching Approach is selected
         /* This function calculates the relaxation parameters that will
         be used in the time marching procedure */
         CalTimeRelaxationForMomentumEqn(Ro, Numi, Numj, NumI, NumJ, TimeURelaxPar, TimeVRelaxPar,
                                         UmomaCenter, VmomaCenter, XJIcoorCenter, YJIcoorCenter,
                                         XjicoorCorners, YjicoorCorners, TimeStep);
      }

      if (SolverType == 2) {  // If the SIMPLER Algorithm is selected
         /* This function calculates the Pseudo-Velocities in X direction */
         PseudoUcalculator(UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter, UmomaSource,
                           Numi, NumJ, PJIpressure, YjicoorCorners, UJivelocity, UJi_pse_vel,
                           UmomaSource_Pseu);

         /* This function calculates the Pseudo-Velocities in Y direction */
         PseudoVcalculator(VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource,
                           NumI, Numj, PJIpressure, XjicoorCorners, VjIvelocity, VjI_pse_vel,
                           VmomaSource_Pseu);    // REFERANS // MEMORY LEACAG

         /* This part keeps the pressure values that are going to be
         corrected (changed) in the next step, for residual calculation */
         PressureForResidual(PJIpressure, PJIpressureOLD, NumI, NumJ);

         /* Although the name of the function is pressure corrector
         at this step this function forms the pressure equation not pressure
         correction equation */
         PressureCorrectionMatrix(PJIpressure, PresWest, PresEast, PresSouth, PresNorth, PresCenter,
                                  PresSource, Numi, Numj, NumI, NumJ, UmomaCenter, VmomaCenter,
                                  XjicoorCorners, YjicoorCorners, UJi_pse_vel, VjI_pse_vel, PresCrctdJi,
                                  PresCrctdjI, UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, VmomaWest,
                                  VmomaEast, VmomaSouth, VmomaNorth, SolverType, Ro, 0, BoundaryLeft,
                                  BoundaryRight, BoundaryTop, BoundaryBottom);

         /* This part solves the Pressure matrix */
         LineTDMASolver(PresNorth, PresSouth, PresWest, PresEast, PresCenter, PresSource, NumI, NumJ,
                        PJIpressure, 4, 1);

      	/* This function updates the source coefficients in U&V momentum
         equations since the pressure values are updated in the previous step. */

         SourceModifierSimpler(UmomaSource, VmomaSource, PJIpressure, YjicoorCorners, XjicoorCorners,
                               Numi, Numj, NumI, NumJ, UmomaSource_Pseu, VmomaSource_Pseu);
      }

      /* This part solves the U momentum matrix */
      LineTDMASolver(UmomaNorth, UmomaSouth, UmomaWest, UmomaEast, UmomaCenter, UmomaSource, Numi, NumJ,
                     UJivelocity, 4, 1);

      /* This part solves the V momentum matrix */
      LineTDMASolver(VmomaNorth, VmomaSouth, VmomaWest, VmomaEast, VmomaCenter, VmomaSource, NumI, Numj,
                     VjIvelocity, 4, 2);

      /* OUTLET BC (part 1/4)
      To impliment an outlet BC four modifications should be done and
      this part is one of them */
      OutletBC(BoundaryRight, BoundaryLeft, BoundaryTop, BoundaryBottom, UJivelocity, VjIvelocity,
               Numi, Numj, NumI, NumJ);

      /* This function forms the coefficient matrix for the pressure
      correction equation */
      PressureCorrectionMatrix(PJICorrect, PresCrctWest, PresCrctEast, PresCrctSouth, PresCrctNorth,
                               PresCrctCenter, PresCrctSource, Numi, Numj, NumI, NumJ, UmomaCenter,
                               VmomaCenter, XjicoorCorners, YjicoorCorners, UJivelocity, VjIvelocity,
                               PresCrctdJi, PresCrctdjI, UmomaWest, UmomaEast, UmomaSouth, UmomaNorth,
                               VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, SolverType, Ro, 1,
                               BoundaryLeft, BoundaryRight, BoundaryTop, BoundaryBottom);

      /* This part solves the pressure correction matrix */
      LineTDMASolver(PresCrctNorth, PresCrctSouth, PresCrctWest, PresCrctEast, PresCrctCenter,
                     PresCrctSource, NumI, NumJ, PJICorrect, 4, 1);

	  if (SolverType == 1 || SolverType == 3) {  // If the SIMPLE algorithm is selected
         /* This part keeps the pressure values that are going to be
         corrected (changed) in the next step, for residual calculation */
         PressureForResidual(PJIpressure, PJIpressureOLD, NumI, NumJ);

         /* Pressure is corrected at this step using the calculated pressure
         correctors. This is performed only in the SIMPLE & SIMPLEC Algorithms*/
         PressureCorrector(PJIpressure, PJICorrect, NumJ, NumI, PresRelax);
      }

      /* This part keeps the velocity values that are going to be
      corrected (changed) in the next step, for residual calculation */
      VelocityForResidual(UJivelocity, VjIvelocity, UJivelocityOLD, VjIvelocityOLD, Numi, Numj, NumI, NumJ);

      /* Velocity is corrected at this step. Also the velocity residuals
      are calculated at this step */
      VelocityCorrector(UJivelocity, VjIvelocity, PJICorrect, PresCrctdJi, PresCrctdjI, NumJ, NumI, Numj, Numi);

      /* This part calculates the residual changes of all primary
      variables and determines the convergence and divergence criteria */
      CalculateResidual(PJIpressure, UJivelocity, VjIvelocity, PJIpressureOLD, UJivelocityOLD, VjIvelocityOLD,
                        Numi, Numj, NumI, NumJ, OutIte, GlobIte, PresCrctSource, OuterTolerance, DivergenceLimit,
                        ConvergenceFlag, ResidualArray, PJIResidual, UJiResidual, VjIResidual);

      ScalarVarMatrixFormer(ScalaraWest, ScalaraEast,ScalaraSouth, ScalaraNorth, ScalaraCenter, ScalaraSource,
                            NumI, NumJ, ScalarXConvFlux, ScalarYConvFlux, YjicoorCorners, XjicoorCorners, YJIcoorCenter,
                            XJIcoorCenter, YjIcoorFrontFaces, XJicoorSideFaces, ScalarXDiffCond, ScalarYDiffCond,
                            ScalarVar, ThermalConduct, DiffSchm, TimeScaRelaxPar, BoundaryLeft, BoundaryRight,
                            BoundaryTop, BoundaryBottom);

      // OUTLET TEMPERATURE BOUNDARY CONDTION PART [2/2]
      for(int J=1;J<NumJ-1;J++) {
         ScalarVar[J][NumI-1] = ScalarVar[J][NumI-2];
      }

      // Write DAT and OUT files
      if (OutIte%SaveInterval == 0) {
         if (OutIte>0) {
            /* This function creates Tecplot files for the given interval */
            WriteDATfile(OutIte, Directory, UJivelocity, VjIvelocity, PJIpressure, XjicoorCorners, YjicoorCorners,
                         Numi, Numj, isTecplot, PJIResidual, UJiResidual, VjIResidual, XjIcoorFrontFaces,
                         YJicoorSideFaces, BoundaryTop, BoundaryBottom, BoundaryLeft, BoundaryRight, Ujiavrg,
                         Vjiavrg, Pjiavrg, UjiResidual_avrg, VjiResidual_avrg, PjiResidual_avrg, Vorticity_ji,
                         nBlockCells, BlockCellCoor);

            WriteOUTfile(PJIpressure, UJivelocity, VjIvelocity, NumI, NumJ, Numi, Numj, Directory, OutIte);
         }
      }

      if (ConvergenceFlag == 1) {
         WriteDATfile(OutIte, Directory, UJivelocity, VjIvelocity, PJIpressure, XjicoorCorners, YjicoorCorners,
                      Numi, Numj, isTecplot, PJIResidual, UJiResidual, VjIResidual, XjIcoorFrontFaces,
                      YJicoorSideFaces, BoundaryTop, BoundaryBottom, BoundaryLeft, BoundaryRight, Ujiavrg,
                      Vjiavrg, Pjiavrg, UjiResidual_avrg, VjiResidual_avrg, PjiResidual_avrg, Vorticity_ji,
                      nBlockCells, BlockCellCoor);

         message = QString(tr("The solution converged at iteration %1.")).arg(OutIte);
         emit appendOutput(message, Qt::black);
         emit sendRunStatus(QString(tr("CONVERGED")));

         emit sendIteration(OutIte);

         break;
      }

      if (ConvergenceFlag == 2) {
         message = QString(tr("The solution diverged at iteration %1.")).arg(OutIte);
         emit appendOutput(message, Qt::black);
         emit sendRunStatus(QString(tr("DIVERGED")));

         emit sendIteration(OutIte);

         break;
      }

	  mutex.lock();
	  if (pauseSolveThread) {

        message = QString(tr("Solution is paused at iteration %1.")).arg(OutIte);
        emit appendOutput(message, Qt::black);
        //emit sendRunStatus(QString(tr("PAUSED")));

        continueSolveThread.wait(&mutex);
	  }
	  mutex.unlock();

	  mutex.lock();
     if (terminateSolveThread) {
        mutex.unlock();
        message = QString(tr("Solution is terminated at iteration %1.")).arg(OutIte);
        emit appendOutput(message, Qt::black);
        //emit sendRunStatus(QString(tr("TERMINATED")));
        break;
	  }
	  mutex.unlock();

     emit sendIteration(OutIte);
	  // cuneyt: Asagidaki if biraz verimsiz mi oldu?
     if (OutIte == 1 || OutIte%problem->getControlPointUpdateInterval() == 0) {
          // Send residual data to plot.
          emit sendResidual(OutIte, ResidualArray[2], ResidualArray[3], ResidualArray[0], ResidualArray[1]);

          // Send control (track) point data to plot.
          //if (nTrackPoints > 0) {

              //int ID = 0; 
              //emit sendControlPointData(OutIte, UJivelocity[TrackPointYX[ID][0]][TrackPointYX[ID][1]],
              //	                                VjIvelocity[TrackPointYX[ID][0]][TrackPointYX[ID][1]],
              //                                  PJIpressure[TrackPointYX[ID][0]][TrackPointYX[ID][1]]);

              // Asagidaki control point datasi gonderme kisminin multi-blok destegi eksik.
              for(int ID=0; ID<nTrackPoints; ID++) {
                 problem->mesh->blocks[0].controlPointData[ID][0] = UJivelocity[TrackPointYX[ID][0]][TrackPointYX[ID][1]];
                 problem->mesh->blocks[0].controlPointData[ID][1] = VjIvelocity[TrackPointYX[ID][0]][TrackPointYX[ID][1]];
                 problem->mesh->blocks[0].controlPointData[ID][2] = PJIpressure[TrackPointYX[ID][0]][TrackPointYX[ID][1]];
              }

              emit sendControlPointData(OutIte);
		  //}
      }

      //   qApp->processEvents();
   
   }  // End of the global iterations (OutIte loop)

   if (OutIte == GlobIte + 1) {
      message = QString(tr("Iterations are done. Convergence is not achieved."));
      emit appendOutput(message, Qt::black);
      emit sendRunStatus(QString(tr("DONE")));
   }

   // Stop computation timer
   ComptSecStop = time (NULL);
   endTime = clock();
   cpu_time_used = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;

   TotalMemoryDeAllocate(XjicoorCorners, YjicoorCorners, XJicoorSideFaces, YJicoorSideFaces, XjIcoorFrontFaces, YjIcoorFrontFaces,
                         XJIcoorCenter, YJIcoorCenter, PJIpressure, UJivelocity, VjIvelocity, ScalarVar, PJIpressureOLD,
                         UJivelocityOLD, VjIvelocityOLD, UmomXConvFlux, UmomYConvFlux, UmomXDiffCond, UmomYDiffCond, VmomXConvFlux,
                         VmomYConvFlux, VmomXDiffCond, VmomYDiffCond, UmomaWest, UmomaEast, UmomaSouth, UmomaNorth, UmomaCenter,
                         UmomaSource, VmomaWest, VmomaEast, VmomaSouth, VmomaNorth, VmomaCenter, VmomaSource, PJICorrect, PresCrctWest,
                         PresCrctEast, PresCrctSouth, PresCrctNorth, PresCrctCenter, PresCrctSource, PresCrctdJi, PresCrctdjI,
                         UJi_pse_vel,VjI_pse_vel, UmomaSource_Pseu, VmomaSource_Pseu, PresWest, PresEast, PresSouth, PresNorth,
                         PresCenter, PresSource, TimeURelaxPar, TimeVRelaxPar, Ujiavrg, Vjiavrg, Pjiavrg, UjiResidual_avrg,
                         VjiResidual_avrg, PjiResidual_avrg, Vorticity_ji, Numj, NumJ, Numi, NumI, nTrackPoints, nBlockCells,
                         TrackPointYX, BlockCellCoor);

}  // End of mainSolver()

