#include "slvFunctions.h"

// These two functions are called at the beginning and at the end of a solution to
// allocate and deallocate flow solution related matrices.


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
                         double** &uRelaxTM,double** &vRelaxTM,double** &scalarRelaxTM,
                         double** &UJi_pseudo,double** &VjI_pseudo,
                         double** &UmomaSource_pseudo,double** &VmomaSource_pseudo,
                         double** &PresWest,double** &PresEast,double** &PresSouth,double** &PresNorth,
                         double** &PresCenter,double** &PresSource,
                         double** &pJIOLD,double** &UJiOLD,double** &VjIOLD,
                         double** &pJIResidual,double** &UJiResidual,double** &VjIResidual,
                         double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
                         double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
                         double** &Vorticity_ji)
{
   // This function allocates memory for the matrices defined in the mainSolver() function
   
   UmomXConvFlux = MemoryAllocater2D(NumJ, NumI);
   UmomYConvFlux = MemoryAllocater2D(Numj, Numi);
   UmomXDiffCond = MemoryAllocater2D(NumJ, NumI);
   UmomYDiffCond = MemoryAllocater2D(Numj, Numi);

   VmomXConvFlux = MemoryAllocater2D(Numj, Numi);
   VmomYConvFlux = MemoryAllocater2D(NumJ, NumI);
   VmomXDiffCond = MemoryAllocater2D(Numj, Numi);
   VmomYDiffCond = MemoryAllocater2D(NumJ, NumI);

   ScalarXConvFlux = MemoryAllocater2D(NumJ, Numi);
   ScalarYConvFlux = MemoryAllocater2D(Numj, NumI);
   ScalarXDiffCond = MemoryAllocater2D(NumJ, Numi);
   ScalarYDiffCond = MemoryAllocater2D(Numj, NumI);

   UmomaWest = MemoryAllocater2D(NumJ, Numi);
   UmomaEast = MemoryAllocater2D(NumJ, Numi);
   UmomaSouth = MemoryAllocater2D(NumJ, Numi);
   UmomaNorth = MemoryAllocater2D(NumJ, Numi);
   UmomaCenter = MemoryAllocater2D(NumJ, Numi);
   UmomaSource = MemoryAllocater2D(NumJ, Numi);

   VmomaWest = MemoryAllocater2D(Numj, NumI);
   VmomaEast = MemoryAllocater2D(Numj, NumI);
   VmomaSouth = MemoryAllocater2D(Numj, NumI);
   VmomaNorth = MemoryAllocater2D(Numj, NumI);
   VmomaCenter = MemoryAllocater2D(Numj, NumI);
   VmomaSource = MemoryAllocater2D(Numj, NumI);

   ScalaraWest = MemoryAllocater2D(NumJ, NumI);
   ScalaraEast = MemoryAllocater2D(NumJ, NumI);
   ScalaraSouth = MemoryAllocater2D(NumJ, NumI);
   ScalaraNorth = MemoryAllocater2D(NumJ, NumI);
   ScalaraCenter = MemoryAllocater2D(NumJ, NumI);
   ScalaraSource = MemoryAllocater2D(NumJ, NumI);

   PJICorrect = MemoryAllocater2D(NumJ, NumI);
   PresCrctWest = MemoryAllocater2D(NumJ, NumI);
   PresCrctEast = MemoryAllocater2D(NumJ, NumI);
   PresCrctSouth = MemoryAllocater2D(NumJ, NumI);
   PresCrctNorth = MemoryAllocater2D(NumJ, NumI);
   PresCrctCenter = MemoryAllocater2D(NumJ, NumI);
   PresCrctSource = MemoryAllocater2D(NumJ, NumI);
   PresCrctdJi = MemoryAllocater2D(NumJ, Numi);
   PresCrctdjI = MemoryAllocater2D(Numj, NumI);

   uRelaxTM = MemoryAllocater2D(NumJ, Numi);
   vRelaxTM = MemoryAllocater2D(Numj, NumI);
   scalarRelaxTM = MemoryAllocater2D(NumJ, NumI);

   UJi_pseudo = MemoryAllocater2D(NumJ, Numi);
   VjI_pseudo = MemoryAllocater2D(Numj, NumI);
   UmomaSource_pseudo = MemoryAllocater2D(NumJ, Numi);
   VmomaSource_pseudo = MemoryAllocater2D(Numj, NumI);

   PresWest = MemoryAllocater2D(NumJ, NumI);
   PresEast = MemoryAllocater2D(NumJ, NumI);
   PresSouth = MemoryAllocater2D(NumJ, NumI);
   PresNorth = MemoryAllocater2D(NumJ, NumI);
   PresCenter = MemoryAllocater2D(NumJ, NumI);
   PresSource = MemoryAllocater2D(NumJ, NumI);

   pJIOLD = MemoryAllocater2D(NumJ, NumI);
   UJiOLD = MemoryAllocater2D(NumJ, Numi);
   VjIOLD = MemoryAllocater2D(Numj, NumI);

   pJIResidual = MemoryAllocater2D(NumJ, NumI);
   UJiResidual = MemoryAllocater2D(NumJ, Numi);
   VjIResidual = MemoryAllocater2D(Numj, NumI);


   Ujiavrg = MemoryAllocater2D(Numj, Numi);
   Vjiavrg = MemoryAllocater2D(Numj, Numi);
   Pjiavrg = MemoryAllocater2D(Numj, Numi);

   UjiResidual_avrg = MemoryAllocater2D(Numj, Numi);
   VjiResidual_avrg = MemoryAllocater2D(Numj, Numi);
   PjiResidual_avrg = MemoryAllocater2D(Numj, Numi);

   Vorticity_ji = MemoryAllocater2D(Numj, Numi);  

}   // End of function TotalMemoryAllocate()




void TotalMemoryDeAllocate(double **&pJIOLD,double **&UJiOLD,double **&VjIOLD,
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
                           double **&UJi_pseudo,double **&VjI_pseudo,
                           double **&UmomaSource_pseudo,double **&VmomaSource_pseudo,
                           double **&PresWest,double **&PresEast,double **&PresSouth,double **&PresNorth,
                           double **&PresCenter,double **&PresSource,
                           double **&uRelaxTM,double **&vRelaxTM,
                           double** &Ujiavrg,double** &Vjiavrg,double** &Pjiavrg,
                           double** &UjiResidual_avrg,double** &VjiResidual_avrg,double** &PjiResidual_avrg,
                           double** &Vorticity_ji)      
{
   MemoryDeAllocater2D(Xji, Numj, Numi);
   MemoryDeAllocater2D(Yji, Numj, Numi);
   MemoryDeAllocater2D(XJi, NumJ, Numi);
   MemoryDeAllocater2D(YJi, NumJ, Numi);
   MemoryDeAllocater2D(XjI, Numj, NumI);
   MemoryDeAllocater2D(YjI, Numj, NumI);
   MemoryDeAllocater2D(XJI, NumJ, NumI);
   MemoryDeAllocater2D(YJI, NumJ, NumI);

   MemoryDeAllocater2D(pJI, NumJ, NumI);
   MemoryDeAllocater2D(UJi, NumJ, Numi);
   MemoryDeAllocater2D(VjI, Numj, NumI);
   MemoryDeAllocater2D(ScalarVar, NumJ, NumI);

   MemoryDeAllocater2D(pJIOLD, NumJ, NumI);
   MemoryDeAllocater2D(UJiOLD, NumJ, Numi);
   MemoryDeAllocater2D(VjIOLD, Numj, NumI);

   MemoryDeAllocater2D(UmomXConvFlux, NumJ, NumI);
   MemoryDeAllocater2D(UmomYConvFlux, Numj, Numi);
   MemoryDeAllocater2D(UmomXDiffCond, NumJ, NumI);
   MemoryDeAllocater2D(UmomYDiffCond, Numj, Numi);

   MemoryDeAllocater2D(VmomXConvFlux, Numj, Numi);
   MemoryDeAllocater2D(VmomYConvFlux, NumJ, NumI);
   MemoryDeAllocater2D(VmomXDiffCond, Numj, Numi);
   MemoryDeAllocater2D(VmomYDiffCond, NumJ, NumI);

   MemoryDeAllocater2D(UmomaWest, NumJ, Numi);
   MemoryDeAllocater2D(UmomaEast, NumJ, Numi);
   MemoryDeAllocater2D(UmomaSouth, NumJ, Numi);
   MemoryDeAllocater2D(UmomaNorth, NumJ, Numi);
   MemoryDeAllocater2D(UmomaCenter, NumJ, Numi);
   MemoryDeAllocater2D(UmomaSource, NumJ, Numi);

   MemoryDeAllocater2D(VmomaWest, Numj, NumI);
   MemoryDeAllocater2D(VmomaEast, Numj, NumI);
   MemoryDeAllocater2D(VmomaSouth, Numj, NumI);
   MemoryDeAllocater2D(VmomaNorth, Numj, NumI);
   MemoryDeAllocater2D(VmomaCenter, Numj, NumI);
   MemoryDeAllocater2D(VmomaSource, Numj, NumI);

   MemoryDeAllocater2D(PJICorrect, NumJ, NumI);
   MemoryDeAllocater2D(PresCrctWest, NumJ, NumI);
   MemoryDeAllocater2D(PresCrctEast, NumJ, NumI);
   MemoryDeAllocater2D(PresCrctSouth, NumJ, NumI);
   MemoryDeAllocater2D(PresCrctNorth, NumJ, NumI);
   MemoryDeAllocater2D(PresCrctCenter, NumJ, NumI);
   MemoryDeAllocater2D(PresCrctSource, NumJ, NumI);
   MemoryDeAllocater2D(PresCrctdJi, NumJ, Numi);
   MemoryDeAllocater2D(PresCrctdjI, Numj, NumI);

   MemoryDeAllocater2D(UJi_pseudo, NumJ, Numi);
   MemoryDeAllocater2D(VjI_pseudo, Numj, NumI);
   MemoryDeAllocater2D(UmomaSource_pseudo, NumJ, Numi);
   MemoryDeAllocater2D(VmomaSource_pseudo, Numj, NumI);

   MemoryDeAllocater2D(PresWest, NumJ, NumI);
   MemoryDeAllocater2D(PresEast, NumJ, NumI);
   MemoryDeAllocater2D(PresSouth, NumJ, NumI);
   MemoryDeAllocater2D(PresNorth, NumJ, NumI);
   MemoryDeAllocater2D(PresCenter, NumJ, NumI);
   MemoryDeAllocater2D(PresSource, NumJ, NumI);

   MemoryDeAllocater2D(uRelaxTM, NumJ, Numi);
   MemoryDeAllocater2D(vRelaxTM, Numj, NumI);

   MemoryDeAllocater2D(Ujiavrg, Numj, Numi);
   MemoryDeAllocater2D(Vjiavrg, Numj, Numi);
   MemoryDeAllocater2D(Pjiavrg, Numj, Numi);

   MemoryDeAllocater2D(UjiResidual_avrg, Numj, Numi);
   MemoryDeAllocater2D(VjiResidual_avrg, Numj, Numi);
   MemoryDeAllocater2D(PjiResidual_avrg, Numj, Numi);

   MemoryDeAllocater2D(Vorticity_ji, Numj, Numi);

//   IntMemoryDeAllocater2D(controlPointIndex, nControlPoints, 2);   // Cuneyt: 15 Oct 2008. These may create problems.
//   IntMemoryDeAllocater2D(BlockCellIndex, nBlockCells, 4);         // Cuneyt: 15 Oct 2008. These may create problems.

}  // End of function TotalMemoryDeAllocate()
