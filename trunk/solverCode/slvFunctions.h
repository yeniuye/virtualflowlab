// These are the functions used by the flow solver.

#ifndef slvFunctionsH
#define slvFunctionsH

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <math.h>

using namespace std;

//////
void DiffConducCalcUmom(double** &UmomXDiffCond,double** &UmomYDiffCond,
double** XJicoorSideFaces,double** YJicoorSideFaces,int Numi,int Numj,int NumI,
int NumJ,double Kinvis);

void DiffConducCalcVmom(double** &VmomXDiffCond,double** &VmomYDiffCond,
double** XjIcoorFrontFaces,double** YjIcoorFrontFaces,int Numi,int Numj,int NumI,
int NumJ,double Kinvis);

void DiffConducCalcScalar(double** &ScalarXDiffCond,double** &ScalarYDiffCond,
double** XJIcoorCenter,double** YJIcoorCenter,int Numi,int Numj,int NumI,
int NumJ,double ThermalConduct);
//////
void CoordinateCreater(double** XijcoorCorners,
double** YijcoorCorners,int NumXcells, int NumYcells,
int Numi,int Numj,int NumI,int NumJ,
double** &XiJcoorSideFaces,double** &YiJcoorSideFaces,
double** &XIjcoorFrontFaces,double** &YIjcoorFrontFaces,
double** &XIJcoorCenter,double** &YIJcoorCenter);
//////
void ConvFluxCalcUmom(double** &UmomXConvFlux,double** &UmomYConvFlux,
double** UJivelocity,double** VjIvelocity,int Numi, int Numj, int NumI,
int NumJ,double Ro);

void ConvFluxCalcVmom(double** &VmomXConvFlux,double** &VmomYConvFlux,
double** UJivelocity,double** VjIvelocity,int Numi, int Numj, int NumI,
int NumJ,double Ro);

void ConvFluxCalcScalar(double** &ScalarXConvFlux,double** &ScalarYConvFlux,
double** UJivelocity,double** VjIvelocity,int Numi, int Numj, int NumI,
int NumJ,double Ro);
//////
void TransformColocate (double** UJivelocity, double** VjIvelocity,
double** PJIpressure,double** XjicoorCorners, double** YjicoorCorners,int Numi,
int Numj,double** PJIResidual,double** UJiResidual,double** VjIResidual,
double** XjIcoorFrontFaces, double** YJicoorSideFaces,
double** BoundaryTop,double** BoundaryBottom,double** BoundaryLeft,double** BoundaryRight,
double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
double** &Vorticity_ji,int nBlockCells, int** BlockCellCoor);
//////
void SpecVelBoundCond (double** BoundaryLeft,double** BoundaryRight,
double** BoundaryTop,double** BoundaryBottom,int Numj,int NumJ,int Numi,
int NumI,double** &UJivelocity,double** &VjIvelocity);

void OutletBC(double** BoundaryRight,double** BoundaryLeft,double** BoundaryTop,
double** BoundaryBottom,double** &UJivelocity,double** &VjIvelocity,int Numi,
int Numj,int NumI,int NumJ);
//////
void VmomMatrixFormer(double** &VmomaWest,double** &VmomaEast,
double** &VmomaSouth,double** &VmomaNorth, double** &VmomaCenter,
double** &VmomaSource,int NumI,int Numj,double** VmomXConvFlux,
double** VmomYConvFlux,double** YJicoorSideFaces,double** XJicoorSideFaces,
double** XjicoorCorners,double** XjIcoorFrontFaces,
double** VmomXDiffCond,double** VmomYDiffCond,double** PJIpressure,
double** VjIvelocity,double Kinvis,int DiffSchm,double** TimeVRelaxPar,
double** BoundaryLeft,double** BoundaryRight,double** BoundaryTop,
double** BoundaryBottom,double Ro,double** YJIcoorCenter,double TimeStep,
bool isTransient,int nBlockCells, int** BlockCellCoor);
//////
void UmomMatrixFormer(double** &UmomaWest,double** &UmomaEast,
double** &UmomaSouth,double** &UmomaNorth, double** &UmomaCenter,
double** &UmomaSource,int Numi,int NumJ,double** UmomXConvFlux,
double** UmomYConvFlux,double** YjIcoorFrontFaces,double** XjIcoorFrontFaces,
double** YjicoorCorners,double** YJicoorSideFaces,
double** UmomXDiffCond,double** UmomYDiffCond,double** PJIpressure,
double** UJivelocity,double Kinvis,int DiffSchm,
double** TimeURelaxPar,double** BoundaryLeft,double** BoundaryRight,double** BoundaryTop,
double** BoundaryBottom,double Ro,double** XJIcoorCenter,double TimeStep,
bool isTransient,int nBlockCells, int** BlockCellCoor);
//////
void TotalMemoryDeAllocate(double **&XjicoorCorners,double **&YjicoorCorners,
double **&XJicoorSideFaces,double **&YJicoorSideFaces,
double **&XjIcoorFrontFaces,double **&YjIcoorFrontFaces,
double **&XJIcoorCenter,double **&YJIcoorCenter,
double **&PJIpressure,double **&UJivelocity,double **&VjIvelocity,double **&ScalarVar,
double **&PJIpressureOLD,double **&UJivelocityOLD,double **&VjIvelocityOLD,
double **&UmomXConvFlux,double **&UmomYConvFlux,double **&UmomXDiffCond,
double **&UmomYDiffCond,
double **&VmomXConvFlux,double **&VmomYConvFlux,double **&VmomXDiffCond,
double **&VmomYDiffCond,
double **&UmomaWest,double **&UmomaEast,double **&UmomaSouth,
double **&UmomaNorth,double **&UmomaCenter,double **&UmomaSource,
double **&VmomaWest,double **&VmomaEast,double **&VmomaSouth,
double **&VmomaNorth,double **&VmomaCenter,double **&VmomaSource,
double **&PJICorrect,double **&PresCrctWest,double **&PresCrctEast,
double **&PresCrctSouth,double **&PresCrctNorth,double **&PresCrctCenter,
double **&PresCrctSource,double **&PresCrctdJi,double **&PresCrctdjI,
double **&UJi_pse_vel,double **&VjI_pse_vel,
double **&UmomaSource_Pseu,double **&VmomaSource_Pseu,
double **&PresWest,double **&PresEast,double **&PresSouth,double **&PresNorth,
double **&PresCenter,double **&PresSource,
double **&TimeURelaxPar,double **&TimeVRelaxPar,
double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
double** &Vorticity_ji,
int Numj,int NumJ,int Numi,int NumI,int nTrackPoints,
int nBlockCells, int** &TrackPointYX, int** &BlockCellCoor);
//////
void TotalMemoryAllocate(double** &UmomXConvFlux,double** &UmomYConvFlux,
double** &UmomXDiffCond,double** &UmomYDiffCond,double** &VmomXConvFlux,
double** &VmomYConvFlux,double** &VmomXDiffCond,double** &VmomYDiffCond,
double** &ScalarXConvFlux,double** &ScalarYConvFlux,
double** &ScalarXDiffCond,double** &ScalarYDiffCond,
double** &UmomaWest,double** &UmomaEast,double** &UmomaSouth,
double** &UmomaNorth,double** &UmomaCenter,double** &UmomaSource,
double** &VmomaWest,double** &VmomaEast,double** &VmomaSouth,
double** &VmomaNorth,double** &VmomaCenter,double** &VmomaSource,
double** &ScalaraWest,double** &ScalaraEast,double** &ScalaraSouth,
double** &ScalaraNorth,double** &ScalaraCenter,double** &ScalaraSource,
double** &PJICorrect,double** &PresCrctWest,double** &PresCrctEast,
double** &PresCrctSouth,double** &PresCrctNorth,double** &PresCrctCenter,
double** &PresCrctSource,double** &PresCrctdJi,double** &PresCrctdjI,
double** &TimeURelaxPar,double** &TimeVRelaxPar,double** &TimeScaRelaxPar,
double** &UJi_pse_vel,double** &VjI_pse_vel,
double** &UmomaSource_Pseu,double** &VmomaSource_Pseu,
double** &PresWest,double** &PresEast,double** &PresSouth,double** &PresNorth,
double** &PresCenter,double** &PresSource,
double** &PJIpressureOLD,double** &UJivelocityOLD,double** &VjIvelocityOLD,
double** &PJIResidual,double** &UJiResidual,double** &VjIResidual,
double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
double** &Vorticity_ji,
int Numj,int NumJ,int Numi,int NumI);
//////
void CalTimeRelaxationForMomentumEqn (double Ro,int Numi,int Numj,int NumI,
int NumJ,double** &TimeURelaxPar,double** &TimeVRelaxPar,double** UmomaCenter,
double** VmomaCenter,double** XJIcoorCenter,double** YJIcoorCenter,
double** XjicoorCorners,double** YjicoorCorners,double TimeStep);

void InitilizeRelaxationParameters(int Numi,int Numj,int NumI,int NumJ,
double** &TimeURelaxPar,double** &TimeVRelaxPar,double** &TimeScaRelaxPar,int SolverApproach,
double UvelRelax,double VvelRelax,double ScalarRelax);
//////
void WriteDATfile(int OutIte,string Directory,double** UJivelocity,
double** VjIvelocity,double** PJIpressure,double** XjicoorCorners,
double** YjicoorCorners,int Numi,int Numj,bool isTecplot,
double** PJIResidual,double** UJiResidual,double** VjIResidual,
double** XjIcoorFrontFaces, double** YJicoorSideFaces,
double** BoundaryTop,double** BoundaryBottom,double** BoundaryLeft,
double** BoundaryRight,
double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
double** &Vorticity_ji,int nBlockCells, int** BlockCellCoor);
//////
void TDMASolver(double* Carray, double* Aarray, double* Barray,double* Darray,double* Tarray,int size);
//////
void ScalarVarMatrixFormer(double** &ScalaraWest,double** &ScalaraEast,
double** &ScalaraSouth,double** &ScalaraNorth, double** &ScalaraCenter,
double** &ScalaraSource,int NumI,int NumJ,double** ScalarXConvFlux,
double** ScalarYConvFlux,
double** YjicoorCorners,double** XjicoorCorners,
double** YJIcoorCenter,double** XJIcoorCenter,
double** YjIcoorFrontFaces,double** XJicoorSideFaces,

double** ScalarXDiffCond,double** ScalarYDiffCond,
double** ScalarVar,double ThermalConduct,int DiffSchm,
double** TimeScaRelaxPar,double** BoundaryLeft,double** BoundaryRight,double** BoundaryTop,
double** BoundaryBottom);
//////
void CalculateResidual (double** PJIpressure,double** UJivelocity,
double** VjIvelocity,double** PJIpressureOLD,double** UJivelocityOLD,
double** VjIvelocityOLD,int Numi,int Numj,int NumI,int NumJ,int &OutIte,
int GlobIte,double** PresCrctSource,
double OuterTolerance,double DivergenceLimit,int &ConvergenceFlag,
double* ResidualArray,double** &PJIResidual,double** &UJiResidual,
double** &VjIResidual);

void VelocityForResidual (double** UJivelocity,double** VjIvelocity,
double** UJivelocityOLD,double** VjIvelocityOLD,int Numi,int Numj,int NumI,
int NumJ);

void PressureForResidual (double** PJIpressure,double** PJIpressureOLD,
int NumI,int NumJ);
//////
int ReadInputFile(
double** &XjicoorCorners, double** &YjicoorCorners,
int &NumXcells, int &NumYcells, int &nTrackPoints,
int &nBlockCells,

double** &BoundaryLeft, double** &BoundaryRight,
double** &BoundaryTop, double** &BoundaryBottom,

double** &PJIpressure, double** &UJivelocity,
double** &VjIvelocity, double** &ScalarVar,

int** &TrackPointYX, int** &BlockCellCoor,

double &Ro, double &Kinvis, int &GlobIte,double &PresRelax,
double &UvelRelax, double &VvelRelax,double &ScalarRelax,
int &Numi, int &Numj, int &NumI,int &NumJ, int &DiffSchm,
int &SaveInterval, int &SolverType, int &SolverApproach,
double &TimeStep, double &OuterTolerance,
int &trackPointSaveInterval, bool &isTecplot,
bool &isTransient, bool &isRestart, string Directory);
//////
void PseudoUcalculator(double** UmomaWest,double** UmomaEast,
double** UmomaSouth,double** UmomaNorth, double** UmomaCenter,
double** UmomaSource,int Numi,int NumJ,double** PJIpressure,
double** YjicoorCorners,double** UJivelocity,double** &UJi_pse_vel,
double** &UmomaSource_Pseu);


void PseudoVcalculator(double** VmomaWest,double** VmomaEast,
double** VmomaSouth,double** VmomaNorth, double** VmomaCenter,
double** VmomaSource,int NumI,int Numj,double** PJIpressure,
double** XjicoorCorners,double** VjIvelocity,double** &VjI_pse_vel,
double** &VmomaSource_Pseu);

void SourceModifierSimpler(double** UmomaSource,double** VmomaSource,
double** PJIpressure,double**YjicoorCorners,double** XjicoorCorners,
int Numi,int Numj,int NumI,int NumJ,double** UmomaSource_Pseu,
double** VmomaSource_Pseu);
//////
void PressureCorrector(double** &PJIpressure,double** &PJICorrect,int NumJ,
int NumI,double PresRelax);

void VelocityCorrector(double** &UJivelocity,double** &VjIvelocity,
double** PJICorrect,double** PresCrctdJi,double** PresCrctdjI,int NumJ,
int NumI,int Numj,int Numi);
//////
void PressureCorrectionMatrix(double** &PJICorrect, double** &PresCrctWest,
double** &PresCrctEast,double** &PresCrctSouth,double** &PresCrctNorth,
double** &PresCrctCenter,double** &PresCrctSource,
int Numi,int Numj,int NumI,int NumJ,
double** UmomaCenter,double** VmomaCenter,
double** XjicoorCorners,double** YjicoorCorners,
double** UJivelocity,double** VjIvelocity,
double** &PresCrctdJi, double** &PresCrctdjI,double** UmomaWest,double** UmomaEast,
double** UmomaSouth,double** UmomaNorth,double** VmomaWest,double** VmomaEast,
double** VmomaSouth,double** VmomaNorth,int SolverType,double Ro,bool PressureCorrectFlag,
double** BoundaryLeft,double** BoundaryRight,double** BoundaryTop,
double** BoundaryBottom);
//////
void WriteOUTfile(double** &PJIpressure,double** &UJivelocity,
double** &VjIvelocity,int NumI,int NumJ,int Numi,int Numj,string Directory,int OutIte);
//////
void MemoryDeAllocater2D(double** DeAllocatedArray,int RowSize, int ColumnSize);

void IntMemoryDeAllocater2D(int** DeAllocatedArray,int RowSize, int ColumnSize);
//////
double** MemoryAllocater2D(int RowSize, int ColumnSize);

int** IntMemoryAllocater2D(int RowSize, int ColumnSize);
//////
void LineTDMASolver(double** NorthArray,double** SouthArray,double** WestArray,
double** EastArray,double** CenterArray,double** SourceArray,int NumXcell,
int NumYcell,double** TemperatureArray,int IterationNum,int TypeSweep);

#endif
