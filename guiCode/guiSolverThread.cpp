#include <QtGui>
#include <math.h>
#include "guiSolverThread.h"




SolverThread::SolverThread(QObject *parent): QThread(parent)
{
}




SolverThread::~SolverThread()
{
}




void SolverThread::run()
{
   mainSolver();  // This is the main() of Gunes' solver.
}
