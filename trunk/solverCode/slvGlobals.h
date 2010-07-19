// These are the global variables defined in slvMainsolver.cpp

#ifndef slvGlobalsH
#define slvGlobalsH

#include <string>

using namespace std;




extern string fullProblemName;

extern double density, kinvis;

extern int numXcells, numYcells;

extern int Numi, Numj, NumI, NumJ;

extern int discSchm, solverAlgorithm, solverApproach;

extern int outerIterMax;

extern double outerTolerance;

extern int saveInterval, controlPointSaveInterval;

extern double timeStep;

extern bool isTecplot, isRestart, isTransient;

extern double **pJI, **UJi, **VjI, **ScalarVar;

extern double pRelax, uRelax, vRelax, scalarRelax;

extern int nControlPoints, nBlockCells;

extern double **Xji, **Yji, **XJi, **YJi, **XjI, **YjI, **XJI, **YJI;

extern double **BoundaryLeft, **BoundaryRight, **BoundaryTop, **BoundaryBottom;

#endif
