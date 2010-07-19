#include <QtGui>
#include <math.h>
#include <iomanip>
#include "mainWindow.h"
#include "glwidget.h"
#include "guiTypedefs.h"
#include "guiProblem.h"
#include "../guiCode/guiSolverThread.h"

extern Problem *problem;




void mainWindow::showBC(int p)
{
   int BCtype;
   string *strPointer;

   if (p == -1) return;

   BCtype = problem->mesh->primitives[p].getBCtype();

   if (BCtype == 1)			// Wall
      boundaryTypeComboBox->setCurrentIndex(0);
   else if (BCtype == 2)	// Inflow
      boundaryTypeComboBox->setCurrentIndex(1);
   else if (BCtype == 3)	// Outflow
      boundaryTypeComboBox->setCurrentIndex(2);

   strPointer = problem->mesh->primitives[p].getBCstring();

   uBCedit->setText(QString(strPointer[0].c_str()));     // c_str converts string to char, than it is casted into QString
   vBCedit->setText(QString(strPointer[1].c_str()));
   pBCedit->setText(QString(strPointer[2].c_str()));
}  // End of function showBC()




void mainWindow::boundaryTypeChanged(int bcType)
{
   if (bcType == 0 || bcType == 1) {	// Wall or Inflow
      uBCedit->setEnabled(true);
      vBCedit->setEnabled(true);
      pBCedit->setText("");
      pBCedit->setEnabled(false);
   } else if (bcType == 2) {	         // Outflow
      uBCedit->setText("");
      vBCedit->setText("");
      pBCedit->setText("");
      uBCedit->setEnabled(false);
      vBCedit->setEnabled(false);
      pBCedit->setEnabled(false);
   }
}




void mainWindow::setBC()
{
   int p = problem->mesh->getPrimitiveUnderMouse();

   if (p == -1) return;

   problem->mesh->primitives[p].setBCtype( boundaryTypeComboBox->currentIndex() + 1 );	  // 1: Wall, 2: Inflow, 3: Outflow
   
   problem->mesh->primitives[p].setBCstring(0, uBCedit->text().toStdString() );
   problem->mesh->primitives[p].setBCstring(1, vBCedit->text().toStdString() );
   problem->mesh->primitives[p].setBCstring(2, pBCedit->text().toStdString() );

   glWidget->updateGL();    // Cuneyt: In this updateGL, primitives for which BCs are set might be drawn with a different color.
}




void mainWindow::setIC()
{
   problem->setICstring(0, uICedit->text().toStdString() );
   problem->setICstring(1, vICedit->text().toStdString() );
   problem->setICstring(2, pICedit->text().toStdString() );
}




bool mainWindow::checkBC()
{
   // Called before starting a solution. If one of the items in the mesh does not have an assigned BC the solution will not start.

   int k, b, BCtype, counter, p;
   int *dummyList;
   string *BCstr;

   b = 0;  // Cuneyt: No multi-block support.

   for (int i = 0; i < 4; i++) {  // Face counter
      counter = 0;  // Counter for the nodes on the face
      k = problem->mesh->blocks[b].faces[i].getnPrimitives();
      dummyList  = problem->mesh->blocks[b].faces[i].getOrderedList();
      for (int j = 0; j < k; j++) {
         p = dummyList[j];
         BCtype = problem->mesh->primitives[p].getBCtype();
         BCstr = problem->mesh->primitives[p].getBCstring();
         if (BCtype == 1 && BCstr[0] == "" && BCstr[1] == "" && BCstr[2] == "") {
            return false;
         }
      }
   }

   return true;
}  // End of checkBC()




bool mainWindow::checkIC()
{
   // Called before starting a solution. If one of the text boxes for ICs is empty than the solution will not start.
   if (!restartCheck->isChecked()) {
      if (uICedit->text().toStdString() == "" || vICedit->text().toStdString() == "" || pICedit->text().toStdString() == "") {
         return false;
      }
   }
   return true;
}




bool mainWindow::checkParameters()
{
   // Called before starting a solution. If one of the text boxes for parameters is empty than the solution will not start.

   bool flag = true;

   if (kinvisEdit->text().toStdString() == "" || densityEdit->text().toStdString() == "" || maxOuterIterEdit->text().toStdString() == "" ||
       outputIntervalEdit->text().toStdString() == "" || controlPointUpdateIntervalEdit->text().toStdString() == "" ||
       outerToleranceEdit->text().toStdString() == "") {
       flag = false;
   }
   if (strategyCombo->currentIndex() == 1) {  // Time marching solution
      if (timeStepEdit->text().toStdString() == "") {
         flag = false;
      }
      if (schemeCombo->currentIndex() == 0 || schemeCombo->currentIndex() == 1) {  // SIMPLE of SIMPLEC
         if (pRelaxationEdit->text().toStdString() == "") {
            flag = false;
         }
      }
   } else {  // Iterative solution
      if (uRelaxationEdit->text().toStdString() == "" || vRelaxationEdit->text().toStdString() == "") {
            flag = false;
      }
      if (schemeCombo->currentIndex() == 0) {  // SIMPLE
         if (pRelaxationEdit->text().toStdString() == "") {
            flag = false;
         }
      }
   }

   return flag;
}  // End of function checkParameters()
