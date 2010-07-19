#ifndef SOLVERTHREAD_H
#define SOLVERTHREAD_H

#include <QtGui>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>




class SolverThread : public QThread
{
Q_OBJECT

public:
   SolverThread(QObject *parent = 0);
   ~SolverThread();
   
   void run();
   void mainSolver();

signals:
   void appendOutput(QString, QColor);
   void sendRunStatus(QString);
   void sendIteration(int);
   void sendResidual(int, double, double, double, double);
   void sendControlPointData(int);

private:	
   QMutex mutex;
};

#endif
