#include <QtGui>
#include <math.h>
#include <iomanip>
#include "mainWindow.h"
#include "glwidget.h"
#include "guiConvergencePlot.h"
#include "guiControlPointPlot.h"
#include "guiTypedefs.h"
#include "guiProblem.h"
#include "../guiCode/guiSolverThread.h"

extern Problem *problem;
extern bool pauseSolveThread;
extern bool terminateSolveThread;
extern QWaitCondition continueSolveThread;




void mainWindow::setParameters()
{
   problem->setScheme(schemeCombo->currentIndex());                  // 0: SIMPLE, 1: SIMPLEC, 2: SIMPLER
   problem->setDiscretization(discretizationCombo->currentIndex());  // 0: Central, 1: Upwind, 2: Hybrid, 3: Power law
   
   problem->setKinvis(kinvisEdit->text().toFloat());
   problem->setDensity(densityEdit->text().toFloat());
   problem->setRelaxation(0, uRelaxationEdit->text().toFloat());
   problem->setRelaxation(1, vRelaxationEdit->text().toFloat());
   problem->setRelaxation(2, pRelaxationEdit->text().toFloat());
   //problem->setRelaxation(3, scalarRelaxationEdit->text().toFloat());
   problem->setMaxOuterIter(maxOuterIterEdit->text().toInt());
   problem->setOuterTolerance(outerToleranceEdit->text().toFloat());
   problem->setOutputInterval(outputIntervalEdit->text().toInt());
   problem->setPlotUpdateInterval(plotUpdateIntervalEdit->text().toInt());

   if (timeDependentCheck->isChecked())
      problem->setIsTimeDependent(TRUE);
   else
      problem->setIsTimeDependent(FALSE);

   if (DATfileCheck->isChecked())
      problem->setIsTecplot(TRUE);
   else
      problem->setIsTecplot(FALSE);

   if (restartCheck->isChecked())
      problem->setIsRestart(TRUE);
   else
      problem->setIsRestart(FALSE);
} // End of function setParameters()




void mainWindow::restartStateChanged(int state)
{
   if (restartCheck->isChecked()) {
      uICedit->setEnabled(FALSE);
      vICedit->setEnabled(FALSE);
      pICedit->setEnabled(FALSE);
   } else{
      uICedit->setEnabled(TRUE);
      vICedit->setEnabled(TRUE);
      pICedit->setEnabled(TRUE);
   }
}




void mainWindow::schemeOrStrategyChanged(int state)
{
   if (strategyCombo->currentIndex() == 0) {   // Iterative
      timeDependentCheck->setChecked(FALSE);
      timeDependentCheck->setEnabled(FALSE);
      timeStepEdit->setEnabled(FALSE);
      uRelaxationEdit->setEnabled(TRUE);
      vRelaxationEdit->setEnabled(TRUE);
      pRelaxationEdit->setEnabled(TRUE);
      //scalarRelaxationEdit->setEnabled(FALSE);
      if (schemeCombo->currentIndex() == 1 || schemeCombo->currentIndex() == 2) {   // SIMPLEC or SIMPLER
         pRelaxationEdit->setEnabled(FALSE);
      }
   } else {   // Time marching
      timeDependentCheck->setEnabled(TRUE);
      timeStepEdit->setEnabled(TRUE);
      uRelaxationEdit->setEnabled(FALSE);
      vRelaxationEdit->setEnabled(FALSE);
      pRelaxationEdit->setEnabled(TRUE);
      //scalarRelaxationEdit->setEnabled(FALSE);
      if (schemeCombo->currentIndex() == 2) {   // SIMPLER
         pRelaxationEdit->setEnabled(FALSE);
      }
   }
} // End of function schemeOrStrategyChanged()




void mainWindow::startSolution(void)
{
   if (!problem->mesh->getIsMeshGenerated()) {
      appendMessage(tr("ERROR: First generate the mesh. Solution will not start."), Qt::red);
      return;
   }

   setIC();             // IC values are trasferred from the GUI into the variables.
   setParameters();     // Parameters are trasferred from the GUI into the variables.

   if (!checkBC()) {    // All items of the mesh should have a BC assigned.
      appendMessage(tr("ERROR: Provide boundary conditions for all geometrical entities. Solution will not start."), Qt::red);
      return;
   }

   if (!checkIC()) {    // IC related check boxes need to be filled.
      appendMessage(tr("ERROR: Provide values for all initial conditions. Solution will not start."), Qt::red);
      return;
   }

   if (!checkParameters()) {    // IC related check boxes need to be filled.
      appendMessage(tr("ERROR: Provide values for all solution parameters. Solution will not start."), Qt::red);
      return;
   }

   if (!saveProblem())    // We need to save the problem before starting the solution.
      return;

   rightTab->setCurrentIndex(1);  // Show the Convergence page.
   convergencePlot->initialize(problem->getMaxOuterIter());
   controlPointPlot->initialize(problem->getMaxOuterIter());
   solverThread.start();
   pauseButton->setEnabled(TRUE);
   terminateButton->setEnabled(TRUE);
   startButton->setEnabled(FALSE);

} // End of function startSolution()




void mainWindow::pauseSolution (void)
{
   pauseSolveThread = true;
   pauseButton->setEnabled(FALSE);
   continueButton->setEnabled(TRUE);
   statusLabel->setText(tr("Status: PAUSED"));
}




void mainWindow::continueSolution (void)
{
   pauseSolveThread = false;
   pauseButton->setEnabled(TRUE);
   continueButton->setEnabled(FALSE);
   continueSolveThread.wakeOne();
   statusLabel->setText(tr("Status: RUNNING"));
}




void mainWindow::terminateSolution (void)
{
   terminateSolveThread = true;
   if (pauseSolveThread) {   // If the Terminate button is pressed after pressing the Pause button.
      continueSolution();
   }
   startButton->setEnabled(TRUE);
   pauseButton->setEnabled(FALSE);
   continueButton->setEnabled(FALSE);
   terminateButton->setEnabled(FALSE);
   statusLabel->setText(tr("Status: TERMINATED"));
}




void mainWindow::solverThreadDone(void)
{
   startButton->setEnabled(TRUE);
   pauseButton->setEnabled(FALSE);
   continueButton->setEnabled(FALSE);
   terminateButton->setEnabled(FALSE);
   terminateSolveThread = false;
}




void mainWindow::setConvergenceAxesLabels(double minX, double maxX, double minY, double maxY)
{
   convergenceMinXlabel->setText(QString("%1").arg(minX));
   convergenceMaxXlabel->setText(QString("%1").arg(maxX));
   convergenceMinYlabel->setText("1e" + QString("%1").arg(minY) + "  ");
   convergenceMaxYlabel->setText("1e" + QString("%1").arg(maxY) + "  ");
}




void mainWindow::setControlPointAxesLabels(double minX, double maxX, double minY, double maxY)
{
   controlPointMinXlabel->setText(QString("%1").arg(minX));
   controlPointMaxXlabel->setText(QString("%1").arg(maxX));
   controlPointMinYlabel->setText(QString("%1").arg(minY,0,'E',3) + " ");
   controlPointMaxYlabel->setText(QString("%1").arg(maxY,0,'E',3) + " ");
}




void mainWindow::clearAllControlPoints()
{
   if(!problem->mesh->getIsMeshGenerated())
      return;

   int b = 0;  // Cuneyt: No multi-block support.

   problem->mesh->blocks[b].setnControlPoints(0);
   glWidget->update();
}




void mainWindow::clearAllBlockedCells()
{
   int b, i, nX, nY;

   if(!problem->mesh->getIsMeshGenerated())
      return;

   b = 0;  // Cuneyt: No multi-block support.


   nX = problem->mesh->blocks[b].getnXpoints();
   nY = problem->mesh->blocks[b].getnYpoints();

   for (i = 0; i < (nY-1)*(nX-1); i++)
      problem->mesh->blocks[b].isCellBlocked[i] = 0;

   problem->mesh->blocks[b].setnBlockedCells(0);
   glWidget->update();
}




void mainWindow::autoFillBlockCellStatusChanged() {
   if(this->autoFillBlockedCellsButton->isChecked()) {
      problem->setMainState(BLOCKEDCELLSAUTOFILL);
   } else {
      problem->setMainState(BLOCKEDCELLS);
   }
}
