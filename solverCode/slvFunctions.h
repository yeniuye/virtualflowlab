// These are the functions used by the flow solver.

#ifndef slvFunctionsH
#define slvFunctionsH

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include "slvGlobals.h"

using namespace std;




void CoordinateCreater();

void DiffConducCalcUmom(double** &UmomXDiffCond, double** &UmomYDiffCond);

void DiffConducCalcVmom(double** &VmomXDiffCond, double** &VmomYDiffCond);

void DiffConducCalcScalar(double** &ScalarXDiffCond, double** &ScalarYDiffCond, double ThermalConduct);

void ConvFluxCalcUmom(double** &UmomXConvFlux, double** &UmomYConvFlux);

void ConvFluxCalcVmom(double** &VmomXConvFlux, double** &VmomYConvFlux);

void ConvFluxCalcScalar(double** &ScalarXConvFlux, double** &ScalarYConvFlux);

void VmomMatrixFormer(double** &VmomaWest, double** &VmomaEast, double** &VmomaSouth, double** &VmomaNorth,
                      double** &VmomaCenter, double** &VmomaSource, double** VmomXConvFlux,
                      double** VmomYConvFlux, double** VmomXDiffCond,
                      double** VmomYDiffCond,
                      double** vRelaxTM, int** BlockCellIndex);

void UmomMatrixFormer(double** &UmomaWest, double** &UmomaEast, double** &UmomaSouth, double** &UmomaNorth,
                      double** &UmomaCenter, double** &UmomaSource, double** UmomXConvFlux,
                      double** UmomYConvFlux, double** UmomXDiffCond,
                      double** UmomYDiffCond, double** uRelaxTM, int** BlockCellIndex);

void CalTimeRelaxationForMomentumEqn (double** &uRelaxTM, double** &vRelaxTM, double** UmomaCenter, double** VmomaCenter);

void InitilizeRelaxationParameters(double** &uRelaxTM, double** &vRelaxTM, double** &scalarRelaxTM);

void SpecifiedVelBC ();

void OutletBC();

void TDMASolver(double* Carray, double* Aarray, double* Barray, double* Darray, double* Tarray, int size);

void LineTDMASolver(double** NorthArray, double** SouthArray, double** WestArray, double** EastArray, double** CenterArray,
                    double** SourceArray, int Num1, int Num2, double** UnknownArray, int nSweep, int sweepDir); 

void ScalarVarMatrixFormer(double** &ScalaraWest, double** &ScalaraEast, double** &ScalaraSouth, double** &ScalaraNorth,
                           double** &ScalaraCenter, double** &ScalaraSource, double** ScalarXConvFlux,
                           double** ScalarYConvFlux, double** ScalarXDiffCond,
                           double** ScalarYDiffCond, double** ScalarVar, double ThermalConduct,
                           double** scalarRelaxTM);

void pseudoUcalculator(double** UmomaWest, double** UmomaEast, double** UmomaSouth, double** UmomaNorth, double** UmomaCenter,
                       double** UmomaSource, double** &UJi_pseudo, double** &UmomaSource_pseudo);

void pseudoVcalculator(double** VmomaWest, double** VmomaEast, double** VmomaSouth, double** VmomaNorth, double** VmomaCenter,
                       double** VmomaSource, double** &VjI_pseudo, double** &VmomaSource_pseudo);

void SourceModifierSimpler(double** UmomaSource, double** VmomaSource, double** UmomaSource_pseudo, double** VmomaSource_pseudo);

void PressureCorrector(double** &PJICorrect);

void VelocityCorrector(double** PJICorrect, double** PresCrctdJi, double** PresCrctdjI);

void PressureCorrectionMatrix(double** &PresCrctWest, double** &PresCrctEast, double** &PresCrctSouth,
                              double** &PresCrctNorth, double** &PresCrctCenter, double** &PresCrctSource,
                              double** UmomaCenter, double** VmomaCenter,
                              double** Uvel, double** Vvel,
                              double** &PresCrctdJi, double** &PresCrctdjI,
                              double** UmomaWest, double** UmomaEast, double** UmomaSouth,
                              double** UmomaNorth, double** VmomaWest, double** VmomaEast, double** VmomaSouth,
                              double** VmomaNorth, bool PressureCorrectFlag);

void CalculateResidual (double** pJIOLD, double** UJiOLD, double** VjIOLD,
                        double** PresCrctSource, double DivergenceLimit, int &ConvergenceFlag,
                        double* ResidualArray, double** &pJIResidual, double** &UJiResidual, double** &VjIResidual);

void copyVelocity (double** UJiOLD, double** VjIOLD);

void copyPressure (double** pJIOLD);

int ReadInputFile(int** &controlPointIndex, int** &BlockCellIndex);

void writeDATfile(double** pJIResidual, double** UJiResidual, double** VjIResidual,
                  double** &Ujiavrg, double** &Vjiavrg, double** &Pjiavrg,
                  double** &UjiResidual_avrg, double** &VjiResidual_avrg, double** &PjiResidual_avrg, double** &Vorticity_ji,
                  int** BlockCellIndex, int outerIter);

void writeOUTfile(int outerIter);

void TransformColocate (double** pJIResidual, double** UJiResidual, double** VjIResidual,
                        double** &Ujiavrg, double** &Vjiavrg, double** &Pjiavrg,
                        double** &UjiResidual_avrg, double** &VjiResidual_avrg, double** &PjiResidual_avrg,
                        double** &Vorticity_ji, int** BlockCellIndex);

double** MemoryAllocater2D(int RowSize, int ColumnSize);

int** IntMemoryAllocater2D(int RowSize, int ColumnSize);

void MemoryDeAllocater2D(double** DeAllocatedArray, int RowSize, int ColumnSize);

void IntMemoryDeAllocater2D(int** DeAllocatedArray, int RowSize, int ColumnSize);

void TotalMemoryAllocate(double** &UmomXConvFlux, double** &UmomYConvFlux, double** &UmomXDiffCond,
                         double** &UmomYDiffCond, double** &VmomXConvFlux, double** &VmomYConvFlux,
                         double** &VmomXDiffCond, double** &VmomYDiffCond, double** &ScalarXConvFlux,
                         double** &ScalarYConvFlux, double** &ScalarXDiffCond, double** &ScalarYDiffCond,
                         double** &UmomaWest, double** &UmomaEast, double** &UmomaSouth, double** &UmomaNorth,
                         double** &UmomaCenter, double** &UmomaSource, double** &VmomaWest, double** &VmomaEast,
                         double** &VmomaSouth, double** &VmomaNorth, double** &VmomaCenter, double** &VmomaSource,
                         double** &ScalaraWest, double** &ScalaraEast, double** &ScalaraSouth, double** &ScalaraNorth,
                         double** &ScalaraCenter, double** &ScalaraSource, double** &PJICorrect, double** &PresCrctWest,
                         double** &PresCrctEast, double** &PresCrctSouth, double** &PresCrctNorth, double** &PresCrctCenter,
                         double** &PresCrctSource, double** &PresCrctdJi, double** &PresCrctdjI, double** &uRelaxTM,
                         double** &vRelaxTM, double** &scalarRelaxTM, double** &UJi_pseudo, double** &VjI_pseudo,
                         double** &UmomaSource_pseudo, double** &VmomaSource_pseudo, double** &PresWest, double** &PresEast,
                         double** &PresSouth, double** &PresNorth, double** &PresCenter, double** &PresSource,
                         double** &pJIOLD, double** &UJiOLD, double** &VjIOLD, double** &pJIResidual,
                         double** &UJiResidual, double** &VjIResidual, double** &Ujiavrg, double** &Vjiavrg, double** &Pjiavrg,
                         double** &UjiResidual_avrg, double** &VjiResidual_avrg, double** &PjiResidual_avrg, double** &Vorticity_ji);

void TotalMemoryDeAllocate(double **&pJIOLD, double **&UJiOLD, double **&VjIOLD, double **&UmomXConvFlux,
                           double **&UmomYConvFlux, double **&UmomXDiffCond, double **&UmomYDiffCond,
                           double **&VmomXConvFlux, double **&VmomYConvFlux, double **&VmomXDiffCond,
                           double **&VmomYDiffCond, double **&UmomaWest, double **&UmomaEast, double **&UmomaSouth,
                           double **&UmomaNorth, double **&UmomaCenter, double **&UmomaSource, double **&VmomaWest,
                           double **&VmomaEast, double **&VmomaSouth, double **&VmomaNorth, double **&VmomaCenter,
                           double **&VmomaSource, double **&PJICorrect, double **&PresCrctWest, double **&PresCrctEast,
                           double **&PresCrctSouth, double **&PresCrctNorth, double **&PresCrctCenter,
                           double **&PresCrctSource, double **&PresCrctdJi, double **&PresCrctdjI, double **&UJi_pseudo,
                           double **&VjI_pseudo, double **&UmomaSource_pseudo, double **&VmomaSource_pseudo, double **&PresWest,
                           double **&PresEast, double **&PresSouth, double **&PresNorth, double **&PresCenter,
                           double **&PresSource, double **&uRelaxTM, double **&vRelaxTM, double** &Ujiavrg,
                           double** &Vjiavrg, double** &Pjiavrg, double** &UjiResidual_avrg, double** &VjiResidual_avrg,
                           double** &PjiResidual_avrg, double** &Vorticity_ji);

#endif
