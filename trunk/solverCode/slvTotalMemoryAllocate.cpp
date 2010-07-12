/***************************************************************/
/*           TOTAL MEMORY ALLOCATE and DEALLOCATE              */
/***************************************************************/

#include "slvFunctions.h"

/* This function allocates memory for each array that is defined in the main function */
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
int Numj,int NumJ,int Numi,int NumI)
{
	UmomXConvFlux = MemoryAllocater2D(NumJ,NumI);
	UmomYConvFlux = MemoryAllocater2D(Numj,Numi);
	UmomXDiffCond = MemoryAllocater2D(NumJ,NumI);
	UmomYDiffCond = MemoryAllocater2D(Numj,Numi);

	VmomXConvFlux = MemoryAllocater2D(Numj,Numi);
	VmomYConvFlux = MemoryAllocater2D(NumJ,NumI);
	VmomXDiffCond = MemoryAllocater2D(Numj,Numi);
	VmomYDiffCond = MemoryAllocater2D(NumJ,NumI);

   ScalarXConvFlux = MemoryAllocater2D(NumJ,Numi);
   ScalarYConvFlux = MemoryAllocater2D(Numj,NumI);
   ScalarXDiffCond = MemoryAllocater2D(NumJ,Numi);
   ScalarYDiffCond = MemoryAllocater2D(Numj,NumI);

	UmomaWest = MemoryAllocater2D(NumJ,Numi);
	UmomaEast = MemoryAllocater2D(NumJ,Numi);
	UmomaSouth = MemoryAllocater2D(NumJ,Numi);
	UmomaNorth = MemoryAllocater2D(NumJ,Numi);
	UmomaCenter = MemoryAllocater2D(NumJ,Numi);
	UmomaSource = MemoryAllocater2D(NumJ,Numi);

	VmomaWest = MemoryAllocater2D(Numj,NumI);
	VmomaEast = MemoryAllocater2D(Numj,NumI);
	VmomaSouth = MemoryAllocater2D(Numj,NumI);
	VmomaNorth = MemoryAllocater2D(Numj,NumI);
	VmomaCenter = MemoryAllocater2D(Numj,NumI);
	VmomaSource = MemoryAllocater2D(Numj,NumI);

   ScalaraWest = MemoryAllocater2D(NumJ,NumI);
   ScalaraEast = MemoryAllocater2D(NumJ,NumI);
   ScalaraSouth = MemoryAllocater2D(NumJ,NumI);
   ScalaraNorth = MemoryAllocater2D(NumJ,NumI);
   ScalaraCenter = MemoryAllocater2D(NumJ,NumI);
   ScalaraSource = MemoryAllocater2D(NumJ,NumI);

	PJICorrect = MemoryAllocater2D(NumJ,NumI);
	PresCrctWest = MemoryAllocater2D(NumJ,NumI);
	PresCrctEast = MemoryAllocater2D(NumJ,NumI);
	PresCrctSouth = MemoryAllocater2D(NumJ,NumI);
	PresCrctNorth = MemoryAllocater2D(NumJ,NumI);
	PresCrctCenter = MemoryAllocater2D(NumJ,NumI);
	PresCrctSource = MemoryAllocater2D(NumJ,NumI);
	PresCrctdJi = MemoryAllocater2D(NumJ,Numi);
	PresCrctdjI = MemoryAllocater2D(Numj,NumI);

	TimeURelaxPar = MemoryAllocater2D(NumJ,Numi);
	TimeVRelaxPar = MemoryAllocater2D(Numj,NumI);
   TimeScaRelaxPar = MemoryAllocater2D(NumJ,NumI);

	UJi_pse_vel = MemoryAllocater2D(NumJ,Numi);
	VjI_pse_vel = MemoryAllocater2D(Numj,NumI);
	UmomaSource_Pseu = MemoryAllocater2D(NumJ,Numi);
	VmomaSource_Pseu = MemoryAllocater2D(Numj,NumI);

	PresWest = MemoryAllocater2D(NumJ,NumI);
	PresEast = MemoryAllocater2D(NumJ,NumI);
	PresSouth = MemoryAllocater2D(NumJ,NumI);
	PresNorth = MemoryAllocater2D(NumJ,NumI);
	PresCenter = MemoryAllocater2D(NumJ,NumI);
	PresSource = MemoryAllocater2D(NumJ,NumI);

	PJIpressureOLD = MemoryAllocater2D( NumJ , NumI);
	UJivelocityOLD = MemoryAllocater2D( NumJ , Numi);
	VjIvelocityOLD = MemoryAllocater2D( Numj , NumI);

   PJIResidual = MemoryAllocater2D( NumJ , NumI);
	UJiResidual = MemoryAllocater2D( NumJ , Numi);
	VjIResidual = MemoryAllocater2D( Numj , NumI);


	Ujiavrg = MemoryAllocater2D(Numj,Numi);
	Vjiavrg = MemoryAllocater2D(Numj,Numi);
	Pjiavrg = MemoryAllocater2D(Numj,Numi);

   UjiResidual_avrg = MemoryAllocater2D(Numj,Numi);
	VjiResidual_avrg = MemoryAllocater2D(Numj,Numi);
	PjiResidual_avrg = MemoryAllocater2D(Numj,Numi);

   Vorticity_ji = MemoryAllocater2D(Numj,Numi);  

}   // End of function TotalMemoryAllocate()




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
int nBlockCells, int** &TrackPointYX, int** &BlockCellCoor)      
{
   MemoryDeAllocater2D(XjicoorCorners,Numj,Numi);
   MemoryDeAllocater2D(YjicoorCorners,Numj,Numi);
   MemoryDeAllocater2D(XJicoorSideFaces,NumJ,Numi);
   MemoryDeAllocater2D(YJicoorSideFaces,NumJ,Numi);
   MemoryDeAllocater2D(XjIcoorFrontFaces,Numj,NumI);
   MemoryDeAllocater2D(YjIcoorFrontFaces,Numj,NumI);
   MemoryDeAllocater2D(XJIcoorCenter,NumJ,NumI);
   MemoryDeAllocater2D(YJIcoorCenter,NumJ,NumI);

   MemoryDeAllocater2D(PJIpressure,NumJ,NumI);
   MemoryDeAllocater2D(UJivelocity,NumJ,Numi);
   MemoryDeAllocater2D(VjIvelocity,Numj,NumI);
   MemoryDeAllocater2D(ScalarVar,NumJ,NumI);

   MemoryDeAllocater2D(PJIpressureOLD,NumJ,NumI);
   MemoryDeAllocater2D(UJivelocityOLD,NumJ,Numi);
   MemoryDeAllocater2D(VjIvelocityOLD,Numj,NumI);

   MemoryDeAllocater2D(UmomXConvFlux,NumJ,NumI);
   MemoryDeAllocater2D(UmomYConvFlux,Numj,Numi);
   MemoryDeAllocater2D(UmomXDiffCond,NumJ,NumI);
   MemoryDeAllocater2D(UmomYDiffCond,Numj,Numi);

   MemoryDeAllocater2D(VmomXConvFlux,Numj,Numi);
   MemoryDeAllocater2D(VmomYConvFlux,NumJ,NumI);
   MemoryDeAllocater2D(VmomXDiffCond,Numj,Numi);
   MemoryDeAllocater2D(VmomYDiffCond,NumJ,NumI);

   MemoryDeAllocater2D(UmomaWest,NumJ,Numi);
   MemoryDeAllocater2D(UmomaEast,NumJ,Numi);
   MemoryDeAllocater2D(UmomaSouth,NumJ,Numi);
   MemoryDeAllocater2D(UmomaNorth,NumJ,Numi);
   MemoryDeAllocater2D(UmomaCenter,NumJ,Numi);
   MemoryDeAllocater2D(UmomaSource,NumJ,Numi);

   MemoryDeAllocater2D(VmomaWest,Numj,NumI);
   MemoryDeAllocater2D(VmomaEast,Numj,NumI);
   MemoryDeAllocater2D(VmomaSouth,Numj,NumI);
   MemoryDeAllocater2D(VmomaNorth,Numj,NumI);
   MemoryDeAllocater2D(VmomaCenter,Numj,NumI);
   MemoryDeAllocater2D(VmomaSource,Numj,NumI);

   MemoryDeAllocater2D(PJICorrect,NumJ,NumI);
   MemoryDeAllocater2D(PresCrctWest,NumJ,NumI);
   MemoryDeAllocater2D(PresCrctEast,NumJ,NumI);
   MemoryDeAllocater2D(PresCrctSouth,NumJ,NumI);
   MemoryDeAllocater2D(PresCrctNorth,NumJ,NumI);
   MemoryDeAllocater2D(PresCrctCenter,NumJ,NumI);
   MemoryDeAllocater2D(PresCrctSource,NumJ,NumI);
   MemoryDeAllocater2D(PresCrctdJi,NumJ,Numi);
   MemoryDeAllocater2D(PresCrctdjI,Numj,NumI);

   MemoryDeAllocater2D(UJi_pse_vel,NumJ,Numi);
   MemoryDeAllocater2D(VjI_pse_vel,Numj,NumI);
   MemoryDeAllocater2D(UmomaSource_Pseu,NumJ,Numi);
   MemoryDeAllocater2D(VmomaSource_Pseu,Numj,NumI);

   MemoryDeAllocater2D(PresWest,NumJ,NumI);
   MemoryDeAllocater2D(PresEast,NumJ,NumI);
   MemoryDeAllocater2D(PresSouth,NumJ,NumI);
   MemoryDeAllocater2D(PresNorth,NumJ,NumI);
   MemoryDeAllocater2D(PresCenter,NumJ,NumI);
   MemoryDeAllocater2D(PresSource,NumJ,NumI);

   MemoryDeAllocater2D(TimeURelaxPar,NumJ,Numi);
   MemoryDeAllocater2D(TimeVRelaxPar,Numj,NumI);

   MemoryDeAllocater2D(Ujiavrg,Numj,Numi);
   MemoryDeAllocater2D(Vjiavrg,Numj,Numi);
   MemoryDeAllocater2D(Pjiavrg,Numj,Numi);

   MemoryDeAllocater2D(UjiResidual_avrg,Numj,Numi);
   MemoryDeAllocater2D(VjiResidual_avrg,Numj,Numi);
   MemoryDeAllocater2D(PjiResidual_avrg,Numj,Numi);

   MemoryDeAllocater2D(Vorticity_ji,Numj,Numi);

//   IntMemoryDeAllocater2D(TrackPointYX,nTrackPoints,2);   // cuneyt: 15-10-2008 bunlar da sorun olabiliyor.
//   IntMemoryDeAllocater2D(BlockCellCoor,nBlockCells,4);   // cuneyt: 15-10-2008 bunlar da sorun olabiliyor.

}  // End of function TotalMemoryDeAllocate()
