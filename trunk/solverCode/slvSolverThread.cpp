#include <QtGui>
#include <math.h>
#include "slvSolverThread.h"

SolverThread::SolverThread(QObject *parent): QThread(parent)
{
	//restart = false;
	//abort = false;
}

SolverThread::~SolverThread()
{
	//mutex.lock();
	//abort = true;
	//condition.wakeOne();
	//mutex.unlock();

	//wait();
}



void SolverThread::run()
{
	//QString str;
	//emit sendMessage(str, Qt::red);
	mainSolver();  // This is the main() of Gunes' solver.
	//emit sendMessage(str, Qt::red);
}

