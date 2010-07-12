#include <QtGui>
#include <math.h>
#include <iomanip>
#include <string>
#include "mainWindow.h"
#include "glwidget.h"
#include "guiConvergencePlot.h"
#include "guiTypedefs.h"
#include "guiProblem.h"
#include "../solverCode/slvSolverThread.h"

extern Problem *problem;




void mainWindow::geometryButtonsClicked()
{
	/* Shows the corresponding page of the geometryStack when Line, Arc, Bezier, Delete, etc. buttons are pressed. */
	if (lineButton->isChecked()) {
		geometryStack->setCurrentIndex(1);
		problem->setDrawingWhat(LINE);
	}
	else if (arcButton->isChecked()) {
		geometryStack->setCurrentIndex(2);
		problem->setDrawingWhat(ARC);
	}
	else if (functionButton->isChecked()) {
		geometryStack->setCurrentIndex(4);
		problem->setDrawingWhat(FUNCTION);
	}
	else if (deleteButton->isChecked()) {
		geometryStack->setCurrentIndex(3);
		problem->setDrawingWhat(DEL);
	}
}  // End of geometryButtonsClicked()




void mainWindow::lineDrawButtonClicked()
{
	int i, isValid;
	float *dummyCoor;

	// First collect the data entered in the GUI.
	dummyCoor = new float[2*2];         // 2 is because a line has 2 entered points
	dummyCoor[0] = lineX1->text().toFloat();
	dummyCoor[1] = lineY1->text().toFloat();
	dummyCoor[2] = lineX2->text().toFloat();
	dummyCoor[3] = lineY2->text().toFloat();

	// Check whether this is a valid (acceptable) line.
	// It is not valid if it has zero length or another line with the same end points already exists.
	isValid = problem->mesh->isLineValid(dummyCoor);
	if (isValid == -1)
	{
		appendMessage(tr("ERROR: A line can not have zero length."), Qt::red);
		return;  // The line is not valid.
	} else if (isValid == -2)
	{
		appendMessage(tr("ERROR: A line the same as this one already exists."), Qt::red);
		return;  // The line is not valid.
	}

	// Check if there is space to add a new geometric primitive.
	// cuneyt: Actually the size should grow automatically.
	if (problem->mesh->getnPrimitives() >= MAX_PRIMITIVES) {
		// cuneyt: Increase the size of the primitives array. For the time being just print a warning message.
		appendMessage(tr("ERROR: Maximum number of geometric primitives is reached."), Qt::red);
		return;
	}

	// If the line is acceptable and if there is place to add a new geometric primitive than
	// increase the # of primitives and define the properties of this new primitive.
	problem->mesh->setnPrimitives(problem->mesh->getnPrimitives() + 1);
	i = problem->mesh->getFirstEmptyPrimitivePos();

	problem->mesh->addLine(i, dummyCoor);

    glWidget->updateGL();  // Draw this new primitive.
	
	// if (problem->mesh->getnPrimitives() == 1)  glWidget->showAll();  // showAll if there is only one primitive.
    glWidget->showAll();  // showAll after drawing a primitive.

	deleteButton->setEnabled(TRUE);   // Enable the delete and deleteAll buttons.
	deleteAllButton->setEnabled(TRUE);

}  // End of lineDrawButtonClicked




void mainWindow::arcDrawButtonClicked()
{
	int i, isValid;
	float *dummyCoor;
	bool center, cw;

	// First collect the data entered in the GUI.
	dummyCoor = new float[3*2];         // 3 is because an ARC has 2 entered points
	dummyCoor[0] = arcX1->text().toFloat();
	dummyCoor[1] = arcY1->text().toFloat();
	dummyCoor[2] = arcX2->text().toFloat();
	dummyCoor[3] = arcY2->text().toFloat();
	dummyCoor[4] = arcX3->text().toFloat();
	dummyCoor[5] = arcY3->text().toFloat();

	center = arcWithTwoPoints->isChecked();
	cw = arcCW->isChecked();

	// Check whether this is a valid (acceptable) ARC.
	isValid = problem->mesh->isArcValid(dummyCoor, center, cw);
	if (isValid == -1)
	{
		appendMessage(tr("ERROR: These three points do not define a circular arc."), Qt::red);
		return;  // The ARC is not valid.
	} else if (isValid == -2)
	{
		appendMessage(tr("ERROR: An arc the same as this one already exists."), Qt::red);
		return;  // The ARC is not valid.
	}

	// Check if there is space to add a new geometric primitive.
	// Cuneyt: Actually the size should grow automatically.
	if (problem->mesh->getnPrimitives() >= MAX_PRIMITIVES) {
		// cuneyt: Increase the size of the primitives array. For the time being just print a warning message.
		appendMessage(tr("ERROR: Maximum number of geometric primitives is reached."), Qt::red);
		return;
	}

	// If the ARC is acceptable and if there is place to add a new geometric primitive than
	// increase the # of primitives and define the properties of this new primitive.
	problem->mesh->setnPrimitives(problem->mesh->getnPrimitives() + 1);
	i = problem->mesh->getFirstEmptyPrimitivePos();

	problem->mesh->addArc(i, dummyCoor, center, cw);

   glWidget->updateGL();  // Draw this new primitive.
	
	// if (problem->mesh->getnPrimitives() == 1)  glWidget->showAll();  // showAll if there is only one primitive.
   glWidget->showAll();  // showAll after drawing a primitive.

	deleteButton->setEnabled(TRUE);   // Enable the delete and deleteAll buttons.
	deleteAllButton->setEnabled(TRUE);

}  // End of function arcDrawButtonClicked()




void mainWindow::functionDrawButtonClicked()
{
	int i, isValid;
	float *dummyCoor;

	// First collect the data entered in the GUI.
	string function = functionEdit->text().toStdString();
	dummyCoor = new float[2*2];         // Function has 2 end points
	dummyCoor[0] = functionX1->text().toFloat();
	dummyCoor[1] = functionY1->text().toFloat();
	dummyCoor[2] = functionX2->text().toFloat();
	dummyCoor[3] = functionY2->text().toFloat();


	// Check whether this is a valid (acceptable) FUNCTION.
	isValid = problem->mesh->isFunctionValid(function, dummyCoor);
	if (isValid == -1) {
		appendMessage(tr("ERROR: Both end points are entered to be the same."), Qt::red);
		return;  // The FUNCTION is not valid.
	} else if (isValid == -2) {
		appendMessage(tr("ERROR: The function does not pass through the first point."), Qt::red);
		return;  // The FUNCTION is not valid.
	} else if (isValid == -3) {
		appendMessage(tr("ERROR: The function does not pass through the second point."), Qt::red);
		return;  // The FUNCTION is not valid.
	} else if (isValid == -4) {
		appendMessage(tr("ERROR: A function the same as this one already exists."), Qt::red);
		return;  // The FUNCTION is not valid.
	}

	// Check if there is space to add a new geometric primitive.
	// Cuneyt: Actually the size should grow automatically.
	if (problem->mesh->getnPrimitives() >= MAX_PRIMITIVES) {
		// cuneyt: Increase the size of the primitives array. For the time being just print a warning message.
		appendMessage(tr("ERROR: Maximum number of geometric primitives is reached."), Qt::red);
		return;
	}

	// If the FUNCTION is acceptable and if there is place to add a new geometric primitive than
	// increase the # of primitives and define the properties of this new primitive.
	problem->mesh->setnPrimitives(problem->mesh->getnPrimitives() + 1);
	i = problem->mesh->getFirstEmptyPrimitivePos();

	problem->mesh->addFunction(i, function, dummyCoor);

    glWidget->updateGL();  // Draw this new primitive.
	
	// if (problem->mesh->getnPrimitives() == 1)  glWidget->showAll();  // showAll if there is only one primitive.
    glWidget->showAll();  // showAll after drawing a primitive.

	deleteButton->setEnabled(TRUE);   // Enable the delete and deleteAll buttons.
	deleteAllButton->setEnabled(TRUE);

}  // End of function DrawButtonClicked()




void mainWindow::deletePrimitive(int i)
{
	// This function actually does not delete anything from the memory, but rather assigns default values.
	problem->mesh->primitives[i].deletePrimitive();
	problem->mesh->setnPrimitives(problem->mesh->getnPrimitives() - 1);
	problem->mesh->updateMinMaxUsed();
	
	if (problem->mesh->getnPrimitives() == 0) {
		deleteButton->setEnabled(FALSE);     // Disable the deleteButton if there is no primitive to delete.
		deleteAllButton->setEnabled(FALSE);  // Similar...
	}
   problem->mesh->setIsMeshGenerated(false);
   problem->setResultFileName("");
}




void mainWindow::deleteAllPrimitives()
{
	// This function actually does not delete anything from the memory, but rather assigns default values.
	for (int i=0; i<MAX_PRIMITIVES; i++) {
	    if (problem->mesh->primitives[i].getIsDeleted())  continue;
		problem->mesh->primitives[i].deletePrimitive();
	}
	problem->mesh->setnPrimitives(0);
	problem->mesh->updateMinMaxUsed();

	deleteButton->setEnabled(FALSE);     // Disable the deleteButton.
	deleteAllButton->setEnabled(FALSE);  // Similar...
	glWidget->updateGL();
}

