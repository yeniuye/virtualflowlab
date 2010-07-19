#include <QtGui>
#include <math.h>
#include <iomanip>
#include "mainWindow.h"
#include "glwidget.h"
#include "guiConvergencePlot.h"
#include "guiTypedefs.h"
#include "guiProblem.h"
#include "../guiCode/guiSolverThread.h"

extern Problem *problem;




void mainWindow::showPrimitivesBlockProperties(int p)
{
   if (p < 0) return;

   int* attachedBlock;
   char* attachedFace;
   int nPoints;
   float clusterCoeff;
   clusterTypes clusterType;
   bool isThereSecondBlock;

   attachedBlock = problem->mesh->primitives[p].getAttachedBlock();
   attachedFace = problem->mesh->primitives[p].getAttachedFace();
   nPoints = problem->mesh->primitives[p].getNumberOfPoints();
   clusterCoeff = problem->mesh->primitives[p].getClusterCoeff();
   clusterType = problem->mesh->primitives[p].getClusterType();
   isThereSecondBlock = problem->mesh->primitives[p].getIsThereSecondBlock();

   // First block
   // block1spinBox->setValue(attachedBlock[0] + 1);
   if (attachedFace[0] == 'S')
      block1SouthRadio->setChecked( TRUE );
   else if (attachedFace[0] == 'E')
      block1EastRadio->setChecked( TRUE );
   else if (attachedFace[0] == 'N')
      block1NorthRadio->setChecked( TRUE );
   else if (attachedFace[0] == 'W')
      block1WestRadio->setChecked( TRUE );
   else if (attachedFace[0] == 'A')  { // Nothing assigned yet
      block1SouthRadio->setAutoExclusive(FALSE);   block1SouthRadio->setChecked(FALSE);
      block1EastRadio->setAutoExclusive(FALSE);    block1EastRadio->setChecked(FALSE);
      block1NorthRadio->setAutoExclusive(FALSE);   block1NorthRadio->setChecked(FALSE);
      block1WestRadio->setAutoExclusive(FALSE);    block1WestRadio->setChecked(FALSE);
      block1SouthRadio->setAutoExclusive(TRUE);
      block1EastRadio->setAutoExclusive(TRUE);
      block1NorthRadio->setAutoExclusive(TRUE);
      block1WestRadio->setAutoExclusive(TRUE);
   }

   // Second block
   /*
   if (isThereSecondBlock) {
      twoBlocksCheckBox->setChecked(TRUE);
      block2spinBox->setEnabled(TRUE);
      block2groupBox->setEnabled(TRUE);
      block2spinBox->setValue(attachedBlock[1] + 1);

      if (attachedFace[1] == 'S')
         block2SouthRadio->setChecked( TRUE );
      else if (attachedFace[1] == 'E')
         block2EastRadio->setChecked( TRUE );
      else if (attachedFace[1] == 'N')
         block2NorthRadio->setChecked( TRUE );
      else if (attachedFace[1] == 'W')
         block2WestRadio->setChecked( TRUE );
      else if (attachedFace[1] == 'A')  { // Nothing assigned yet
         block2SouthRadio->setAutoExclusive(FALSE);   block2SouthRadio->setChecked(FALSE);
         block2EastRadio->setAutoExclusive(FALSE);    block2EastRadio->setChecked(FALSE);
         block2NorthRadio->setAutoExclusive(FALSE);   block2NorthRadio->setChecked(FALSE);
         block2WestRadio->setAutoExclusive(FALSE);    block2WestRadio->setChecked(FALSE);
         block2SouthRadio->setAutoExclusive(TRUE);
         block2EastRadio->setAutoExclusive(TRUE);
         block2NorthRadio->setAutoExclusive(TRUE);
         block2WestRadio->setAutoExclusive(TRUE);
      }
   } else {
      twoBlocksCheckBox->setChecked(FALSE);
      block2spinBox->setValue(1);
      block2spinBox->setEnabled(FALSE);
      block2groupBox->setEnabled(FALSE);

      block2SouthRadio->setAutoExclusive(FALSE);   block2SouthRadio->setChecked(FALSE);
      block2EastRadio->setAutoExclusive(FALSE);    block2EastRadio->setChecked(FALSE);
      block2NorthRadio->setAutoExclusive(FALSE);   block2NorthRadio->setChecked(FALSE);
      block2WestRadio->setAutoExclusive(FALSE);    block2WestRadio->setChecked(FALSE);
      block2SouthRadio->setAutoExclusive(TRUE);
      block2EastRadio->setAutoExclusive(TRUE);
      block2NorthRadio->setAutoExclusive(TRUE);
      block2WestRadio->setAutoExclusive(TRUE);
   }
   */

   NoNodesEdit->setText(QString("%1").arg(nPoints) );
   clusterCoeffEdit->setText(QString("%1").arg(clusterCoeff, 0, 'f', 2 ) );

   if (clusterType == NONE)
      clusterTypeComboBox->setCurrentIndex(0);
   else if (clusterType == EXP)
      clusterTypeComboBox->setCurrentIndex(1);
   else if (clusterType == SINH)
      clusterTypeComboBox->setCurrentIndex(2);
   else if (clusterType == TANH)
      clusterTypeComboBox->setCurrentIndex(3);

}  // End of function showPrimitivesBlockProperties()





void mainWindow::blockFaceSetButtonClicked()
{
   int p;

   p = problem->mesh->getPrimitiveUnderMouse();
   
   if (p < 0) return;  // There is no primitive under the cursor.

   if (!(block1SouthRadio->isChecked() || block1EastRadio->isChecked() ||
      block1NorthRadio->isChecked() || block1WestRadio->isChecked()))
      return;    // No radio button is selected

   if (NoNodesEdit->text().toInt() <= 0)
      return;    // Node number can not be negative

   // Deallocate the dynamic arrays that hold boundary point coordinates.
   problem->mesh->primitives[p].deallocatePointCoor();

   // First block
   //problem->mesh->primitives[p].setAttachedBlock(0, block1spinBox->value() - 1);
   problem->mesh->primitives[p].setAttachedBlock(0, 0);

   if (block1SouthRadio->isChecked())
      problem->mesh->primitives[p].setAttachedFace(0, 'S');
   else if (block1EastRadio->isChecked())
      problem->mesh->primitives[p].setAttachedFace(0, 'E');
   else if (block1NorthRadio->isChecked())
      problem->mesh->primitives[p].setAttachedFace(0, 'N');
   else if (block1WestRadio->isChecked())
      problem->mesh->primitives[p].setAttachedFace(0, 'W');

   // Second Block
   /*
   if (twoBlocksCheckBox->isChecked() ) {
      problem->mesh->primitives[p].setIsThereSecondBlock(TRUE);
      problem->mesh->primitives[p].setAttachedBlock(1, block2spinBox->value() - 1);

      if (block2SouthRadio->isChecked())
         problem->mesh->primitives[p].setAttachedFace(1, 'S');
      else if (block2EastRadio->isChecked())
         problem->mesh->primitives[p].setAttachedFace(1, 'E');
      else if (block2NorthRadio->isChecked())
         problem->mesh->primitives[p].setAttachedFace(1, 'N');
      else if (block2WestRadio->isChecked())
         problem->mesh->primitives[p].setAttachedFace(1, 'W');
   } else {
      problem->mesh->primitives[p].setIsThereSecondBlock(FALSE);
      problem->mesh->primitives[p].setAttachedBlock(1, 0);     // Cuneyt: Is this zero correct, or should it be -1 ?
      problem->mesh->primitives[p].setAttachedFace(1, 'S');    // Cuneyt: Should this be 'A' ?
   }
   */

   problem->mesh->primitives[p].setNumberOfPoints(NoNodesEdit->text().toInt());
   // Cuneyt: An integer larger than 1 should be enetered to this box.

   problem->mesh->primitives[p].setClusterCoeff(clusterCoeffEdit->text().toFloat());
   // Cuneyt: Zero should NOT be enetered to this box.

   if (clusterTypeComboBox->currentIndex() == 0)
      problem->mesh->primitives[p].setClusterType(NONE);
   else if (clusterTypeComboBox->currentIndex() == 1)
      problem->mesh->primitives[p].setClusterType(EXP);
   else if (clusterTypeComboBox->currentIndex() == 2)
      problem->mesh->primitives[p].setClusterType(SINH);
   else if (clusterTypeComboBox->currentIndex() == 3)
      problem->mesh->primitives[p].setClusterType(TANH);
   
   problem->mesh->primitives[p].calculateBoundaryNodes();
   problem->mesh->setIsMeshGenerated(false);
   problem->setResultFileName("");
   glWidget->updateGL();
}  // End of function blockFaceSetButtonClicked()




void mainWindow::clusterTypeChanged(int i)
{
   if (i == 0)  // Cluster type is set to NONE
      clusterCoeffEdit->setEnabled(FALSE);
   else
      clusterCoeffEdit->setEnabled(TRUE);
}
