#include <QtGui>

#include <iomanip>
#include <fstream>
#include "mainWindow.h"
#include "guiTypedefs.h"
#include "guiProblem.h"

extern Problem *problem;




void mainWindow::readCfdFile()
{
   // Cuneyt: Multi-block support of this function is not checked.

   int i, j, k, b, m, p, dummyInt, dummyInt2, dummyInt3;
   ifstream cfdFile;
   char dummyChar, c21[21], c41[41], c256[256];
   char *dummyCharPointer;
   string str;
   bool dummyBool;
   float dummyFloat;
   float *dummyCoor;

   cfdFile.open(problem->getCfdFileName().c_str(), ios::in);

   // Read one line file header
   cfdFile.ignore(256, '\n');



   // Read SECTION GEOMETRY
   cfdFile.ignore(256, '\n'); // Read and ignore the section title
   cfdFile >> dummyInt;       // Read total number of primitives drawn
   
   problem->mesh->setnPrimitives(dummyInt);

   for (i = 0; i < problem->mesh->getnPrimitives(); i++) {
      cfdFile >> p;           // Primitive number
      cfdFile >> dummyInt;    // Primitive type
   
      if (dummyInt == 1) {    // This primitive is a LINE
         
         cfdFile.ignore(256, '\n');   // Finish reading the current line
         
         problem->mesh->primitives[p].setType(LINE);
         problem->mesh->primitives[p].setNumberOfEnteredPoints(2);
         problem->mesh->primitives[p].setNumberOfDefPoints(2);

         dummyCoor = new float[2*2];

         cfdFile >> dummyCoor[0];     // Coordinates of the points of this LINE
         cfdFile >> dummyCoor[1];
         cfdFile >> dummyCoor[2];
         cfdFile >> dummyCoor[3];

         problem->mesh->primitives[p].allocateEnteredPointCoor();
         problem->mesh->primitives[p].setEnteredPointCoor(dummyCoor);
         
         delete[] dummyCoor;
         dummyCoor = NULL;

         problem->mesh->primitives[p].allocateDefPointCoor();
         problem->mesh->primitives[p].setLineDefPointCoor();
      
      } else if (dummyInt == 2) {   // This primitive is an ARC
         
         cfdFile >> dummyInt2;      // 1st control parameter determines whether the arc is created using "3 points" or "two points and center"
         if (dummyInt2 == 1)
            problem->mesh->primitives[p].setUsesCenter(TRUE);
         else
            problem->mesh->primitives[p].setUsesCenter(FALSE);

         cfdFile >> dummyInt3;      // 2nd control parameter determines whether the arcis drawn CW or CCW
         if (dummyInt3 == 1)	
            problem->mesh->primitives[p].setIsCW(TRUE);
         else
            problem->mesh->primitives[p].setIsCW(FALSE);
         
         cfdFile.ignore(256, '\n'); // Finish reading the current line

         problem->mesh->primitives[p].setType(ARC);
         problem->mesh->primitives[p].setNumberOfEnteredPoints(3);
         problem->mesh->primitives[p].setNumberOfDefPoints(CIRCULAR_ARC_DEF_POINTS);

         dummyCoor = new float[3*2];

         cfdFile >> dummyCoor[0];  // Coordinates of the points of this ARC
         cfdFile >> dummyCoor[1];
         cfdFile >> dummyCoor[2];
         cfdFile >> dummyCoor[3];
         cfdFile >> dummyCoor[4];
         cfdFile >> dummyCoor[5];

         problem->mesh->primitives[p].allocateEnteredPointCoor();
         problem->mesh->primitives[p].setEnteredPointCoor(dummyCoor);
         
         delete[] dummyCoor;
         dummyCoor = NULL;

         problem->mesh->primitives[p].allocateDefPointCoor();
         problem->mesh->primitives[p].calculateArcDefPointCoor();

      } else if (dummyInt == 3) {		// This primitive is a curve defined by a FUNCTION
         
         cfdFile.ignore(256, '\n');		// Finish reading the current line

         problem->mesh->primitives[p].setType(FUNCTION);
         problem->mesh->primitives[p].setNumberOfEnteredPoints(2);
         problem->mesh->primitives[p].setNumberOfDefPoints(FUNCTION_DEF_POINTS);

         cfdFile.get(c41,41);			// Function that defines the curve
         problem->mesh->primitives[p].setFunction(QString(c41).trimmed().toStdString());

         dummyCoor = new float[2*2];

         cfdFile >> dummyCoor[0];  // Coordinates of the points of this curve
         cfdFile >> dummyCoor[1];
         cfdFile >> dummyCoor[2];
         cfdFile >> dummyCoor[3];

         problem->mesh->primitives[p].allocateEnteredPointCoor();
         problem->mesh->primitives[p].setEnteredPointCoor(dummyCoor);
         
         delete[] dummyCoor;
         dummyCoor = NULL;

         problem->mesh->primitives[p].allocateDefPointCoor();
         problem->mesh->primitives[p].calculateFunctionDefPointCoor();
      }
      problem->mesh->primitives[p].setIsDeleted(FALSE);
   }



   // Read SECTION BLOCKFACE
   cfdFile.ignore(256, '\n');  // Finish the remainings of the last line read
   cfdFile.ignore(256, '\n');  // Read and ignore the section title
   cfdFile >> dummyInt;        // Number of blocks
   problem->mesh->setnBlocks(dummyInt);

   if (dummyInt > 0) problem->mesh->allocateBlocks();

   for (i = 0; i < problem->mesh->getnPrimitives(); i++) {
        cfdFile >> p;			// Primitive number
      cfdFile >> dummyBool;	// Is the primitive attached to one block or two blocks?
      problem->mesh->primitives[p].setIsThereSecondBlock(dummyBool);
      cfdFile >> dummyInt;	   // 1st block that this primitive is connected to. Default is 0
      problem->mesh->primitives[p].setAttachedBlock(0, dummyInt);
      cfdFile >> dummyInt;	   // 2nd block that this primitive is connected to. Default is 0
      problem->mesh->primitives[p].setAttachedBlock(1, dummyInt);
      cfdFile.ignore(2);		// Read 2 dummy characters
      cfdFile >> dummyChar;	// Face of the 1st block that this primitive is connected to. Default is A
      problem->mesh->primitives[p].setAttachedFace(0, dummyChar);
      cfdFile.ignore(2);		// Read 2 dummy characters
      cfdFile >> dummyChar;	// Face of the 2nd block that this primitive is connected to. Default is A
      problem->mesh->primitives[p].setAttachedFace(1, dummyChar);
      cfdFile >> dummyInt;	   // Number of boundary points of the primitive.
      problem->mesh->primitives[p].setNumberOfPoints(dummyInt);
      cfdFile >> dummyInt;	   // Cluster type
      if (dummyInt == 0)
         problem->mesh->primitives[p].setClusterType(NONE);
      else if (dummyInt == 1)
         problem->mesh->primitives[p].setClusterType(EXP);
      else if (dummyInt == 2)
         problem->mesh->primitives[p].setClusterType(SINH);
      else if (dummyInt == 3)
         problem->mesh->primitives[p].setClusterType(TANH);
      
      cfdFile >> dummyFloat;	// Cluster coefficient
      problem->mesh->primitives[p].setClusterCoeff(dummyFloat);
      
      dummyCharPointer = problem->mesh->primitives[p].getAttachedFace();

      if (dummyCharPointer[0] != 'A' && problem->mesh->primitives[p].getNumberOfPoints() > 0)
         problem->mesh->primitives[p].calculateBoundaryNodes();
   }



   // Read SECTION MESH
   cfdFile.ignore(256, '\n');  // Finish the remainings of the last line read
   cfdFile.ignore(256, '\n');  // Read and ignore the section title
   cfdFile >> dummyBool;       // 0: No mesh is generated (or things are changed after generating a mesh and that mesh can not be used), 1: Mesh is generated
   problem->mesh->setIsMeshGenerated(dummyBool);
   cfdFile >> dummyInt;        // Mesh generation algorithm (0: TFI Linear, 1: Laplace)
   problem->setMeshGenerationAlgorithm(dummyInt);



   // Read SECTION BCIC
   cfdFile.ignore(256, '\n');     // Finish the remainings of the last read line
   cfdFile.ignore(256, '\n');     // Read and ignore the section title

   for (i = 0; i < problem->mesh->getnPrimitives(); i++) {
      cfdFile >> p;               // Primitive number
      
      cfdFile >> dummyInt;        // Boundary type (1: wall, 2: inflow, 3: outflow)
      problem->mesh->primitives[p].setBCtype(dummyInt);
      
      cfdFile.ignore(3);          // Read two spaces and one | character
      
      cfdFile.get(c21,21);        // BC for u
      problem->mesh->primitives[p].setBCstring(0, QString(c21).trimmed().toStdString());
      
      cfdFile.ignore(1);          // Read one | character
      
      cfdFile.get(c21,21);        // BC for v
      problem->mesh->primitives[p].setBCstring(1, QString(c21).trimmed().toStdString());
      
      cfdFile.ignore(1);          // Read one | character
      
      cfdFile.get(c21,21);        // BC for p
      problem->mesh->primitives[p].setBCstring(2, QString(c21).trimmed().toStdString());
      
      cfdFile.ignore(256, '\n');  // Ignore the rest of the line
   }

   cfdFile.ignore(7);                                                // Read 7 spaces
   cfdFile.ignore(1);                                                // Read one | character
   cfdFile.get(c21,21);  uICedit->setText(QString(c21).trimmed());   // IC for u
   cfdFile.ignore(1);                                                // Read one | character
   cfdFile.get(c21,21);  vICedit->setText(QString(c21).trimmed());   // IC for v
   cfdFile.ignore(1);                                                // Read one | character
   cfdFile.get(c21,21);  pICedit->setText(QString(c21).trimmed());   // IC for p



   // Read SECTION PARAMETERS
   cfdFile.ignore(256, '\n');  // Finish the remainings of the last line read
   cfdFile.ignore(256, '\n');  // Read and ignore the section title
   
   cfdFile >> dummyInt;        // Number of parameters
   
   problem->setNparameters(dummyInt);
   cfdFile.ignore(256, '\n');  // Ignore the remaining of the last line read

   cfdFile.get(c41,41);                                   // Read 40 character parameter title
   cfdFile >> dummyInt;                                   // Read scheme (0: SIMPLE, 1: SIMPLEC, 2: SIMPLER)
   schemeCombo->setCurrentIndex(dummyInt);
   cfdFile.ignore(256, '\n');

   cfdFile.get(c41,41);
   cfdFile >> dummyInt;                                   // Read discretization (0: Central, 1: Upwind, 2: Hybrid, 3: Power law)
   discretizationCombo->setCurrentIndex(dummyInt);
   cfdFile.ignore(256, '\n');

   cfdFile.get(c41,41);
   cfdFile >> dummyInt;                                   // Read solution strategy (0: Iterative, 1: time marching)
   strategyCombo->setCurrentIndex(dummyInt);
   cfdFile.ignore(256, '\n');

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   timeStepEdit->setText(QString(c256).trimmed());        // Time step

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   kinvisEdit->setText(QString(c256).trimmed());          // Read kinematic viscosity

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   densityEdit->setText(QString(c256).trimmed());         // Read density

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   uRelaxationEdit->setText(QString(c256).trimmed());     // Read u relaxation

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   vRelaxationEdit->setText(QString(c256).trimmed());     // Read v relaxation

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   pRelaxationEdit->setText(QString(c256).trimmed());     // Read p relaxation

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   //scalarRelaxationEdit->setText(QString(c256).trimmed());   // Read scalar relaxation

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   maxOuterIterEdit->setText(QString(c256).trimmed());    // Read max. number of outer iterations

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   outputIntervalEdit->setText(QString(c256).trimmed());  // Read output interval

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   plotUpdateIntervalEdit->setText(QString(c256).trimmed());  // Read plot update interval

   cfdFile.get(c41,41);
   cfdFile.getline(c256, 256, '\n');
   outerToleranceEdit->setText(QString(c256).trimmed());  // Read convergence tolerance of outer iterations

   cfdFile.get(c41,41);
   cfdFile >> dummyBool;                                  // Read the flag for time dependency
   timeDependentCheck->setChecked(dummyBool);
   cfdFile.ignore(256, '\n');

   cfdFile.get(c41,41);
   cfdFile >> dummyBool;                                  // Read the flag for DAT file generation
   DATfileCheck->setChecked(dummyBool);
   cfdFile.ignore(256, '\n');

   cfdFile.get(c41,41);
   cfdFile >> dummyBool;                                  // Read the flag for restarting from a previous result
   restartCheck->setChecked(dummyBool);



   // Read SECTION MESHPOINTS
   cfdFile.ignore(256, '\n');  // Finish the remainings of the last line read
   cfdFile.ignore(256, '\n');  // Read and ignore the section title
   
   if (problem->mesh->getIsMeshGenerated()) {
      
      for (k=0; k<problem->mesh->getnBlocks(); k++) {
         cfdFile >> b;         // Block number
         cfdFile >> dummyInt;  // Number of points in the x direction
         problem->mesh->blocks[b].setnXpoints(dummyInt);
         cfdFile >> dummyInt;  // Number of points in the y direction
         problem->mesh->blocks[b].setnYpoints(dummyInt);

         problem->mesh->blocks[b].coordinates = NULL;
         delete[] problem->mesh->blocks[b].coordinates;   // Cuneyt: Make sure that this really deletes everything.
         
         problem->mesh->blocks[b].coordinates = new float[2 * problem->mesh->blocks[b].getnXpoints() * problem->mesh->blocks[b].getnYpoints()];

         // Cuneyt: This is NOT the correct place for the following 4 lines.
         problem->mesh->blocks[b].isCellBlocked = new int[(problem->mesh->blocks[b].getnXpoints()-1)*(problem->mesh->blocks[b].getnYpoints()-1)];
         for(i=0; i<(problem->mesh->blocks[b].getnXpoints() - 1)*(problem->mesh->blocks[b].getnYpoints() - 1); i++) {
            problem->mesh->blocks[b].isCellBlocked[i] = 0;   // Mark all the cells as NOT blocked.
         }

         for (j=0; j<problem->mesh->blocks[b].getnYpoints(); j++)
         {
            for (i=0; i<problem->mesh->blocks[b].getnXpoints(); i++)
            {
               m = i + j * problem->mesh->blocks[b].getnXpoints();
               cfdFile >> problem->mesh->blocks[b].coordinates[2*m];
               cfdFile >> problem->mesh->blocks[b].coordinates[2*m+1];
            }
         }
      }
   }



   // Read SECTION CONTROLPOINTS
   cfdFile.ignore(256, '\n');  // Finish the remainings of the last line read
   cfdFile.ignore(256, '\n');  // Read and ignore the section title
   
   if (problem->mesh->getIsMeshGenerated()) {

      for (k=0; k<problem->mesh->getnBlocks(); k++) {
         cfdFile >> b;         // Block number
         cfdFile >> dummyInt;  // Number of control points of this block
         problem->mesh->blocks[b].setnControlPoints(dummyInt);
         cfdFile.ignore(256, '\n');  // Finish the remainings of the last read line

         for(i=0; i<dummyInt; i++) {
            cfdFile >> dummyInt2;
            problem->mesh->blocks[b].controlPoints[i] = dummyInt2;
            cfdFile.ignore(256, '\n');  // Finish the remainings of the last read line
         }
      }
   }



   // Read SECTION BLOCKEDCELLS
   cfdFile.ignore(256, '\n');    // Read and ignore the section title
   
   if (problem->mesh->getIsMeshGenerated()) {
      for (k=0; k<problem->mesh->getnBlocks(); k++) {
         cfdFile >> b;           // Block number
         cfdFile >> dummyInt;    // Number of control points of this block
         problem->mesh->blocks[b].setnBlockedCells(dummyInt);
         cfdFile.ignore(256, '\n');  // Finish the remainings of the last read line
         for(i=0; i<dummyInt; i++) {
            cfdFile >> dummyInt2;
            problem->mesh->blocks[b].isCellBlocked[dummyInt2] = 1;
            cfdFile.ignore(256, '\n');  // Finish the remainings of the last read line
         }
      }
   }


   if (problem->mesh->getnPrimitives() == 0) {  // If there are no primitives
      deleteButton->setEnabled(FALSE);          // Enable the delete button
      deleteAllButton->setEnabled(FALSE);       // Enable the deleteAll button
   } else {
      problem->mesh->updateMinMaxUsed();        // Update the min and max coordinates that will be shown on the screen.
      deleteButton->setEnabled(TRUE);           // Enable the delete button
      deleteAllButton->setEnabled(TRUE);        // Enable the deleteAll button
   }

   if (problem->mesh->getIsMeshGenerated()) {
      problem->mesh->calculateNumberOfPrimitivesOnFaces();
      problem->mesh->allocateFaceData();
      problem->mesh->setFacePrimitives();
      problem->mesh->calculateNumberOfBoundaryPointsOnFaces();
      problem->mesh->isFacePrimitiveOrderingCorrect();   // Cuneyt: If a CFD file is written before creating a mesh, this line will give unnecessary warnings.
   }

   cfdFile.close();

}  // End of function readCfdFile()




void mainWindow::writeCfdFile()
{
   // Cuneyt: Multi-blcok support of this function is not checked.

   int p, k;
   int *dummyIntPointer;
   char *dummyCharPointer;
   string *dummyStringPointer;
   float *dummyCoor;
   ofstream cfdFile;

   cfdFile.open(problem->getCfdFileName().c_str(), ios::out|ios::trunc);  // Cuneyt: If not opened, give a warning.
   
   // Write the file header
   cfdFile << "This file is created by Virtual Flow Lab. It may get corrupt if you modify it outside Virtual Flow Lab." << "\n";

   // Write SECTION GEOMETRY
   cfdFile << "SECTION GEOMETRY**********************************" << "\n";
   
   cfdFile << problem->mesh->getnPrimitives() << "\n";   // Total number of primitives drawn
   
   for (p = 0; p < MAX_PRIMITIVES; p++) {
   
      if (problem->mesh->primitives[p].getIsDeleted()) {
         continue;   // Go to the next primitive if this one is marked as deleted.
      }
      
      cfdFile << left << setw(3) << p;                 // Primitive number

      if (problem->mesh->primitives[p].getType() == LINE) {
         cfdFile << left << setw(3) << "1" << "\n";    // Primitive type (1 means LINE)
         
         dummyCoor = problem->mesh->primitives[p].getEnteredPointCoor();   // Cuneyt: Is there a problem here ?
         
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[0];   // Coordinates of line's 2 points
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[1] << "\n";
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[2];
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[3] << "\n";

      } else if (problem->mesh->primitives[p].getType() == ARC) {
      
         cfdFile << left << setw(3) << "2";            // Primitive type (2 means ARC)

         if (problem->mesh->primitives[p].getUsesCenter()) {  // 1st control parameter determines whether "3 points" or "two points and center" is used
            cfdFile << left << setw(3) << "1";
         } else {
            cfdFile << left << setw(3) << "2";
         }

         if (problem->mesh->primitives[p].getIsCW()) {        // 2nd control parameter determines whether the arc is drawn CW or CCW
            cfdFile << left << setw(3) << "1";
         } else {
            cfdFile << setw(3) << "2";
         }

         cfdFile << "\n";

         dummyCoor = problem->mesh->primitives[p].getEnteredPointCoor();   // Cuneyt: Is there a problem here ?
         
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[0];    // Coordinates of arc's 3 points
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[1] << "\n";
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[2];
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[3] << "\n";
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[4];
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[5] << "\n";

      } else if (problem->mesh->primitives[p].getType() == FUNCTION) {
         cfdFile << left << setw(3) << "3" << "\n";;    // Primitive type (3 means FUNCTION)
         
         cfdFile << problem->mesh->primitives[p].getFunction() << "\n";    // Write the function as a string.

         dummyCoor = problem->mesh->primitives[p].getEnteredPointCoor();   // Cuneyt: Is there a problem here ?

         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[0];   // Coordinates of function-defined-curve's 2 points.
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[1] << "\n";
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[2];
         cfdFile << left << fixed << setw(13) << setprecision(7) << dummyCoor[3] << "\n";
      }
   }

   // Write SECTION BLOCKFACE
   cfdFile << "SECTION BLOCKFACE*********************************" << "\n";
   
   cfdFile << problem->mesh->getnBlocks() << "\n";   // Number of blocks
   
   for (p = 0; p < MAX_PRIMITIVES; p++) {
      
      if (problem->mesh->primitives[p].getIsDeleted()) {
         continue;   // Go to the next primitive if this one is marked as deleted.
      }

      cfdFile << left << setw(4) << p;               // Primitive number
      
      cfdFile << left << setw(3) << problem->mesh->primitives[p].getIsThereSecondBlock();  // 1: Primitive is attached to two blocks, 0: Primitive is attached to one block.
      
      dummyIntPointer = problem->mesh->primitives[p].getAttachedBlock();
      dummyCharPointer = problem->mesh->primitives[p].getAttachedFace();

      cfdFile << left << setw(3) << dummyIntPointer[0];   // 1st block that this primitive is connected to. Default is 0
      cfdFile << left << setw(3) << dummyIntPointer[1];   // 2nd block that this primitive is connected to. Default is 0
      cfdFile << left << setw(3) << dummyCharPointer[0];  // Face of the 1st block that this primitive is connected to. Default is A
      cfdFile << left << setw(3) << dummyCharPointer[1];  // Face of the 2nd block that this primitive is connected to. Default is A
      cfdFile << left << setw(6) << problem->mesh->primitives[p].getNumberOfPoints();   // Number of boundary points of the primitive.
      cfdFile << left << setw(3) << problem->mesh->primitives[p].getClusterType();      // Clustering type of the boundary nodes (0:None, 1:Exp, 2:Sinh, 3:Tanh).
      cfdFile << left << setw(7) << setprecision(4) << problem->mesh->primitives[p].getClusterCoeff() << "\n";   // Cluster coefficient
   }



   // Write SECTION MESH
   cfdFile << "SECTION MESH**************************************" << "\n";
   
   cfdFile << left << setw(3) << problem->mesh->getIsMeshGenerated();            // 0: No mesh is generated (or things are changed after generating a mesh and that mesh can not be used), 1: Mesh is generated
   cfdFile << left << setw(3) << problem->getMeshGenerationAlgorithm() << "\n";  // 0: TFI Linear, 1: Laplace

   // Cuneyt: When an algorithm other than TFI is implemented values of "min iter, max iter, tolerance, weight factor" parameters should be added here.



   // Write SECTION BCIC
   cfdFile << "SECTION BCIC**************************************" << "\n";

   for (p = 0; p < MAX_PRIMITIVES; p++) {

      if (problem->mesh->primitives[p].getIsDeleted()) {
         continue;   // Go to the next primitive if this one is marked as deleted.
      }

      cfdFile << left << setw(4) << p;                                           // Primitive number
      cfdFile << left << setw(3) << problem->mesh->primitives[p].getBCtype();    // Boundary type (1: wall, 2: inflow, 3: outflow)
      
      dummyStringPointer = problem->mesh->primitives[p].getBCstring();
      
      cfdFile << "|" << left << setw(20) << dummyStringPointer[0];               // BC for u
      cfdFile << "|" << left << setw(20) << dummyStringPointer[1];               // BC for v
      cfdFile << "|" << left << setw(20) << dummyStringPointer[2] << "|\n";      // BC for p
   }

   cfdFile << "       ";                                                         // Space for alignment
   cfdFile << "|" << left << setw(20) << uICedit->text().toStdString();          // IC for u
   cfdFile << "|" << left << setw(20) << vICedit->text().toStdString();          // IC for v
   cfdFile << "|" << left << setw(20) << pICedit->text().toStdString() << "|\n"; // IC for p



   // Write SECTION PARAMETERS
   cfdFile << "SECTION PARAMETERS********************************" << "\n";
   
   cfdFile << "17 \n";   // Cuneyt: problem->getNparameters() << "\n";
   
   cfdFile << "Scheme                                  " << schemeCombo->currentIndex() << "\n";
   cfdFile << "Discretization                          " << discretizationCombo->currentIndex() << "\n";
   cfdFile << "Solution strategy                       " << strategyCombo->currentIndex() << "\n";
   cfdFile << "Time step                               " << timeStepEdit->text().toStdString() << "\n";
   cfdFile << "Kinematic viscosity (m2/s)              " << kinvisEdit->text().toStdString() << "\n";
   cfdFile << "Density (kg/m3)                         " << densityEdit->text().toStdString() << "\n";
   cfdFile << "u velocity relaxation                   " << uRelaxationEdit->text().toStdString() << "\n";
   cfdFile << "v velocity relaxation                   " << vRelaxationEdit->text().toStdString() << "\n";
   cfdFile << "Pressure relaxation                     " << pRelaxationEdit->text().toStdString() << "\n";
   cfdFile << "Scalar relaxation                       " << "" << "\n";
   //cfdFile << "Scalar relaxation                       " << scalarRelaxationEdit->text().toStdString() << "\n";
   //cfdFile << "Scalar relaxation                       " << scalarRelaxationEdit->text().toStdString() << "\n";
   cfdFile << "Max. number of outer iterations         " << maxOuterIterEdit->text().toStdString() << "\n";
   cfdFile << "Save interval                           " << outputIntervalEdit->text().toStdString() << "\n";
   cfdFile << "Plot update interval                    " << plotUpdateIntervalEdit->text().toStdString() << "\n";
   cfdFile << "Convergence tolerance of outer iters.   " << outerToleranceEdit->text().toStdString() << "\n";
   cfdFile << "Time dependent flow?                    " << timeDependentCheck->isChecked() << "\n";
   cfdFile << "Create DAT files?                       " << DATfileCheck->isChecked() << "\n";
   cfdFile << "Restart from a previous solution?       " << restartCheck->isChecked() << "\n";



   // Write SECTION MESHPOINTS
   cfdFile << "SECTION MESHPOINTS********************************" << "\n";
   
   if (problem->mesh->getIsMeshGenerated()) {
      for (int b=0; b<problem->mesh->getnBlocks(); b++) {
         
         cfdFile << setw(3) << b << setw(6) << problem->mesh->blocks[b].getnXpoints() << setw(6) << problem->mesh->blocks[b].getnYpoints() << "\n";  // Block number, number of points in x direction, number of points in y direction
         
         for (int j=0; j<problem->mesh->blocks[b].getnYpoints(); j++) {
            for (int i=0; i<problem->mesh->blocks[b].getnXpoints(); i++) {
               k = i + j * problem->mesh->blocks[b].getnXpoints();
               cfdFile << left << setw(13) << setprecision(7)
                                           << problem->mesh->blocks[b].coordinates[2*k] << "\t"
                                           << left << setw(13) << setprecision(7)
                                           << problem->mesh->blocks[b].coordinates[2*k+1] << "\n";
            }
         }
      }
   }



   // Write SECTION CONTROLPOINTS
   cfdFile << "SECTION CONTROLPOINTS*****************************" << "\n";
   
   for (int b=0; b<problem->mesh->getnBlocks(); b++) {
      cfdFile << setw(4) << b << setw(4)
              << problem->mesh->blocks[b].getnControlPoints() << "\n";   // Block number and # of control points of this block
      
      for(int i = 0; i < problem->mesh->blocks[b].getnControlPoints(); i++) {
         cfdFile << problem->mesh->blocks[b].controlPoints[i] << "\n";
      }
   }



   // Write SECTION BLOCKEDCELLS
   cfdFile << "SECTION BLOCKEDCELLS*****************************" << "\n";

   for (int b=0; b<problem->mesh->getnBlocks(); b++) {
      cfdFile << setw(4) << b << setw(4)
              << problem->mesh->blocks[b].getnBlockedCells() << "\n";   // Block number and # of blocked cells of this block
      
      int nX = problem->mesh->blocks[b].getnXpoints();   // Shortcut (# of points in x direction)
      int nY = problem->mesh->blocks[b].getnYpoints();   // Shortcut (# of points in y direction)
      int nCell = (nX-1)*(nY-1);                         // Shortcut (# of cells)
      
      for(int i = 0; i < nCell; i++) {
         if (problem->mesh->blocks[b].isCellBlocked[i] == 1) {
            cfdFile << i << "\n";
         }
      }
   }

   // Cuneyt: A message such as "CFD file is writen." should be provided.
   cfdFile.close();
}  // End of function writeCfdFile()




void mainWindow::writeInputFile()
{
   // Creates the .inp file which is read by the flow solver.
   
   // Cuneyt: Multi-block support of this function is not checked.

   int i, j, k, b, p, counter;
   int *dummyList;
   string *strPointer;
   ofstream inputFile;

   // Cuneyt: Following seems to be unnecessary, right ?

   //if (! problem->mesh->getIsMeshGenerated()) {  // Exit if there is no mesh
   //	appendMessage(tr("WARNING: .INP file can not be written because no mesh is generated yet."), Qt::red);
   //	return;
   //}

   inputFile.open(problem->getInputFileName().c_str(), ios::out|ios::trunc);  // Cuneyt: If the file does NOT exist, it is created.
   
   // Write the file header
   inputFile << "This file is created by Virtual Flow Lab. It may get corrupt if you modify it outside Virtual Flow Lab." << "\n";
   
   // Write SECTION GEOMETRY
   inputFile << "SECTION GEOMETRY**********************************" << "\n";
   inputFile << problem->mesh->getnBlocks() << "\n";
   inputFile << problem->mesh->getnPrimitives() << "\n";

   // Write SECTION BLOCKFACE
   inputFile << "SECTION BLOCKFACE*********************************" << "\n";
   for (b = 0; b < problem->mesh->getnBlocks(); b++) {
      inputFile << left << setw(3) << b;
      inputFile << left << setw(6) << problem->mesh->blocks->getnXpoints()
                << left << setw(6) << problem->mesh->blocks->getnYpoints()
              << "\n";
   }

   // Write SECTION MESH
   inputFile << "SECTION MESH**************************************" << "\n";
   inputFile << left << setw(3) << problem->mesh->getIsMeshGenerated() << "\n";  // 0: No mesh is generated (or things are changed after generating a mesh and that mesh can not be used), 1: Mesh is generated

   // Write SECTION BC
   // Cuneyt: This part should be controlled carefully.
   inputFile << "SECTION BC****************************************" << "\n";
   for (b = 0; b < problem->mesh->getnBlocks(); b++) {
      inputFile << setw(3) << b << "\n";    // Block number
   
      for (i = 0; i < 4; i++) {  // Face counter
         
         counter = 0;  // Counter for the nodes on the face

         k = problem->mesh->blocks[b].faces[i].getnPrimitives();
         inputFile << left << k << "\n";
         dummyList  = problem->mesh->blocks[b].faces[i].getOrderedList();

         for (j = 0; j < k; j++) {
            p = dummyList[j];
            inputFile << left << setw(4) << problem->mesh->primitives[p].getBCtype();
            
            inputFile << left << setw(6) << counter;	// First node number of this primitive (Starts from zero)
            
            counter = counter + problem->mesh->primitives[p].getNumberOfPoints() - 1;
            
            inputFile << left << setw(6) << counter << "\n";	// First node number of this primitive.
            
            strPointer = problem->mesh->primitives[p].getBCstring();
            
            inputFile << strPointer[0] << " \n";  // u BC of this primitive
            inputFile << strPointer[1] << " \n";  // v BC of this primitive
            inputFile << strPointer[2] << " \n";  // p BC of this primitive
         }
      }
   }

   // Write SECTION IC
   inputFile << "SECTION IC****************************************" << "\n";
   for (b = 0; b < problem->mesh->getnBlocks(); b++) {
      inputFile << left << setw(20) << uICedit->text().toStdString() << " \n";
      inputFile << left << setw(20) << vICedit->text().toStdString() << " \n";
      inputFile << left << setw(20) << pICedit->text().toStdString() << " \n";
      inputFile << "0.0" << "\n";   // Cuneyt: This 0.0 is for the IC of the scalar variable. It is not being used right now.
   }

   // Write SECTION PARAMETERS
   // Cuneyt: Some of the following parameters are directly taken from the GUI. But if the corresponding edit boxes are empty then arbitrary values are written.
   //         What is a better solution for this ?
   inputFile << "SECTION PARAMETERS********************************" << "\n";
   inputFile << "17 \n";  // cuneyt:   problem->getNparameters() << "\n";
   inputFile << "Scheme                                  " << schemeCombo->currentIndex() + 1 << "\n";   // Cuneyt: These "+1" s should be avoided. Actually the ones in the solver should start from zero.
   inputFile << "Discretization                          " << discretizationCombo->currentIndex() + 1 << "\n";
   inputFile << "Solution strategy                       " << strategyCombo->currentIndex() + 1 << "\n";
   inputFile << "Time step                               " << timeStepEdit->text().toStdString();
   if (timeStepEdit->text().toStdString() == "") inputFile << "-1.0";  inputFile << "\n";    // Cuneyt: Read the above comment.
   inputFile << "Kinematic viscosity (m2/s)              " << kinvisEdit->text().toStdString();
   if (kinvisEdit->text().toStdString() == "") inputFile << "-1.0";  inputFile << "\n";      // Cuneyt: Read the above comment.
   inputFile << "Density (kg/m3)                         " << densityEdit->text().toStdString();
   if (densityEdit->text().toStdString() == "") inputFile << "-1.0";  inputFile << "\n";     // Cuneyt: Read the above comment.
   inputFile << "u velocity relaxation                   " << uRelaxationEdit->text().toStdString();
   if (uRelaxationEdit->text().toStdString() == "") inputFile << "-1.0";  inputFile << "\n"; // Cuneyt: Read the above comment.
   inputFile << "v velocity relaxation                   " << vRelaxationEdit->text().toStdString();
   if (vRelaxationEdit->text().toStdString() == "") inputFile << "-1.0";  inputFile << "\n"; // Cuneyt: Read the above comment.
   inputFile << "Pressure relaxation                     " << pRelaxationEdit->text().toStdString();
   if (pRelaxationEdit->text().toStdString() == "") inputFile << "-1.0";  inputFile << "\n"; // Cuneyt: Read the above comment.
   inputFile << "Scalar relaxation                       " << -1.0;  inputFile << "\n";
// inputFile << "Scalar relaxation                       " << scalarRelaxationEdit->text().toStdString();
// if (scalarRelaxationEdit->text().toStdString() == "") inputFile << "-1.0";  inputFile << "\n";         // Cuneyt: Read the above comment.
   inputFile << "Max. number of outer iterations         " << maxOuterIterEdit->text().toStdString();
   if (maxOuterIterEdit->text().toStdString() == "") inputFile << "-1";  inputFile << "\n";               // Cuneyt: Read the above comment.
   inputFile << "Save interval                           " << outputIntervalEdit->text().toStdString();
   if (outputIntervalEdit->text().toStdString() == "") inputFile << "-1";  inputFile << "\n";             // Cuneyt: Read the above comment.
   inputFile << "Plot update interval                    " << plotUpdateIntervalEdit->text().toStdString();
   if (plotUpdateIntervalEdit->text().toStdString() == "") inputFile << "-1";  inputFile << "\n";         // Cuneyt: Read the above comment.
   inputFile << "Convergence tolerance of outer iters.   " << outerToleranceEdit->text().toStdString();
   if (outerToleranceEdit->text().toStdString() == "") inputFile << "-0.1";  inputFile << "\n";           // Cuneyt: Read the above comment.
   inputFile << "Time dependent flow?                    " << timeDependentCheck->isChecked() << "\n";
   inputFile << "Create DAT files?                       " << DATfileCheck->isChecked() << "\n";
   inputFile << "Restart from a previous solution?       " << restartCheck->isChecked() << "\n";

   // Write SECTION MESHPOINTS
   inputFile << "SECTION MESHPOINTS********************************" << "\n";
   if (problem->mesh->getIsMeshGenerated()) {
      for (int b=0; b<problem->mesh->getnBlocks(); b++) {
         inputFile << setw(3) << b << "\n";   // Block number
         for (int j=0; j<problem->mesh->blocks[b].getnYpoints(); j++)
         {
            for (int i=0; i<problem->mesh->blocks[b].getnXpoints(); i++)
            {
               k = i + j * problem->mesh->blocks[b].getnXpoints();
               inputFile << left << setw(13) << setprecision(7)
                              << problem->mesh->blocks[b].coordinates[2*k] << "\t"
                              << left << setw(13) << setprecision(7)
                              << problem->mesh->blocks[b].coordinates[2*k+1] << "\n";
            }
         }
      }
   }

   // Write SECTION CONTROLPOINTS
   inputFile << "SECTION CONTROLPOINTS*****************************" << "\n";
   if (problem->mesh->getIsMeshGenerated()) {
      for (int b=0; b<problem->mesh->getnBlocks(); b++) {
         inputFile << setw(4) << b << setw(4)
                   << problem->mesh->blocks[b].getnControlPoints() << "\n";   // Block number and # of control points of this block
         
         for(int i = 0; i < problem->mesh->blocks[b].getnControlPoints(); i++) {
            inputFile << problem->mesh->blocks[b].controlPoints[i] << "\n";
         }
      }
   }

   // Write SECTION BLOCKEDCELLS
   inputFile << "SECTION BLOCKEDCELLS*****************************" << "\n";
   if (problem->mesh->getIsMeshGenerated()) {
      for (int b=0; b<problem->mesh->getnBlocks(); b++) {
         inputFile << setw(4) << b << setw(4)
                   << problem->mesh->blocks[b].getnBlockedCells() << "\n";   // Block number and # of blocked cells of this block
         
         int nX = problem->mesh->blocks[b].getnXpoints();  // Shortcut (# of points in x direction)
         int nY = problem->mesh->blocks[b].getnYpoints();  // Shortcut (# of points in y direction)
         int nCell = (nX-1)*(nY-1);  //   Shortcut (# of cells)
         
         for(int i = 0; i < nCell; i++) {
            if (problem->mesh->blocks[b].isCellBlocked[i] == 1) {
               inputFile << i << "\n";
            }
         }
      }
   }

   inputFile.close();

}  // End of function writeInputFile()
