#include "slvFunctions.h"
#include "../fparserCode/fparser.h"
#include "../guiCode/guiProblem.h"

extern Problem *problem;




int ReadInputFile(int** &controlPointIndex, int** &BlockCellIndex)
{
   // Cuneyt: No multi-block support because the flow solver can only work with a single block.

   // This function reads the .inp file created by Virtual Flow Lab GUI
   // Returning 0 means successful reading.
   // Returning -1 means unsuccessful reading due to non-existing mesh in the input file.
   // Returning -2 means there is a problem with the restart file.

   int b, i, j, jj, k, np, nb, dummyInt;
   char c21[21], c41[41];
   int southBCn, eastBCn, northBCn, westBCn;
   int *southBCtype, *eastBCtype, *northBCtype, *westBCtype;
   int **southBCcells, **eastBCcells, **northBCcells, **westBCcells;
   string **southBC, **eastBC, **northBC, **westBC;
   string IC[4];

   string funcString;       // String that holds 3 variable functions for BC's and IC's.
   FunctionParser fparser;  // Function parser object
   double funcResult;
   double funcVals[3];

   ifstream inpFile;
   string inpString = fullProblemName + ".inp";   // e.g. C:\VFL\Problems\MyProblem.inp
   inpFile.open(inpString.data());

   // Read and ignore the file header
   inpFile.ignore(256, '\n');


   /********************************************************
   // Read SECTION GEOMETRY
   ********************************************************/
   inpFile.ignore(256, '\n');    // Read and ignore the section title
   inpFile >> nb;                // Number of blocks. nb is a local variable.
   inpFile.ignore(256, '\n');    // Finish the remainings of the last line read
   inpFile >> np;                // Number of geometric primitives.  np is a local variable.


   /********************************************************
   // Read SECTION BLOCKFACE
   ********************************************************/
   inpFile.ignore(256, '\n');    // Finish the remainings of the last line read
   inpFile.ignore(256, '\n');    // Read and ignore the section title
   inpFile >> dummyInt;          // Read and ignore the block number
   inpFile >> numXcells;
   numXcells = numXcells - 1;
   inpFile >> numYcells;
   numYcells = numYcells - 1;


   /********************************************************
   // Read SECTION MESH
   ********************************************************/
   inpFile.ignore(256, '\n');    // Finish the remainings of the last line read
   inpFile.ignore(256, '\n');    // Read and ignore the section title
   inpFile >> dummyInt;          // Read the flag for mesh generation
   if (dummyInt == 0) {
      return -1;                 // No mesh is generated. Unsuccessfull return
   }


   /********************************************************
   // Read SECTION BC
   ********************************************************/
   inpFile.ignore(256, '\n');    // Finish the remainings of the last line read
   inpFile.ignore(256, '\n');    // Read and ignore the section title

   inpFile.ignore(256, '\n');    // Read and ignore the block number line
   
   inpFile >> southBCn;          // Number of BCs of the South face
   inpFile.ignore(256, '\n');    // Finish the remainings of the last line read
   southBCtype = new int[southBCn];

   southBC = new string*[southBCn];
   for (i = 0; i < southBCn; i++) {
      southBC[i] = new string[3];
   }

   southBCcells = new int*[southBCn];
   for (i = 0; i < southBCn; i++) {
      southBCcells[i] = new int[2];
   }

   for (i = 0; i < southBCn; i++) {
      inpFile >> southBCtype[i];
      inpFile >> southBCcells[i][0];
      inpFile >> southBCcells[i][1];
      
      inpFile.ignore(256, '\n');
      
      inpFile.get(c21,21);			// BC for u
      southBC[i][0] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for v
      southBC[i][1] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for p
      southBC[i][2] = c21;
      inpFile.ignore(256, '\n');
   }

   inpFile >> eastBCn;			   // Number of BCs of the East face
   inpFile.ignore(256, '\n');	   // Finish the remainings of the last line read
   eastBCtype = new int[eastBCn];

   eastBC = new string*[eastBCn];
   for (i = 0; i < eastBCn; i++) {
      eastBC[i] = new string[3];
   }

   eastBCcells = new int*[eastBCn];
   for (i = 0; i < eastBCn; i++) {
      eastBCcells[i] = new int[2];
   }

   for (i = 0; i < eastBCn; i++) {
      inpFile >> eastBCtype[i];
      inpFile >> eastBCcells[i][0];
      inpFile >> eastBCcells[i][1];

      inpFile.ignore(256, '\n');

      inpFile.get(c21,21);			// BC for u
      eastBC[i][0] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for v
      eastBC[i][1] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for p
      eastBC[i][2] = c21;
      inpFile.ignore(256, '\n');
   }

   inpFile >> northBCn;		      // Number of BCs of the North face
   inpFile.ignore(256, '\n');	   // Finish the remainings of the last line read
   northBCtype = new int[northBCn];

   northBC = new string*[northBCn];
   for (i = 0; i < northBCn; i++) {
      northBC[i] = new string[3];
   }

   northBCcells = new int*[northBCn];
   for (i = 0; i < northBCn; i++) {
      northBCcells[i] = new int[2];
   }

   for (i = 0; i < northBCn; i++) {
      inpFile >> northBCtype[i];
      inpFile >> northBCcells[i][0];
      inpFile >> northBCcells[i][1];

      inpFile.ignore(256, '\n');

      inpFile.get(c21,21);			// BC for u
      northBC[i][0] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for v
      northBC[i][1] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for p
      northBC[i][2] = c21;
      inpFile.ignore(256, '\n');
   }

   inpFile >> westBCn;			   // Number of BCs of the West face
   inpFile.ignore(256, '\n');	   // Finish the remainings of the last line read
   westBCtype = new int[westBCn];

   westBC = new string*[westBCn];
   for (i = 0; i < westBCn; i++) {
      westBC[i] = new string[3];
   }

   westBCcells = new int*[westBCn];
   for (i = 0; i < westBCn; i++) {
      westBCcells[i] = new int[2];
   }

   for (i = 0; i < westBCn; i++) {
      inpFile >> westBCtype[i];
      inpFile >> westBCcells[i][0];
      inpFile >> westBCcells[i][1];

      inpFile.ignore(256, '\n');

      inpFile.get(c21,21);			// BC for u
      westBC[i][0] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for v
      westBC[i][1] = c21;
      inpFile.ignore(256, '\n');
      inpFile.get(c21,21);			// BC for p
      westBC[i][2] = c21;
      inpFile.ignore(256, '\n');
   }


   /********************************************************
   // Read SECTION IC
   ********************************************************/
   inpFile.ignore(256, '\n');	   // Read and ignore the section title

   inpFile.get(c21,21);			   // IC for u
   IC[0] = c21;
   inpFile.ignore(256, '\n');
   inpFile.get(c21,21);			   // IC for v
   IC[1] = c21;
   inpFile.ignore(256, '\n');
   inpFile.get(c21,21);			   // IC for p
   IC[2] = c21;
   inpFile.ignore(256, '\n');
   inpFile.get(c21,21);			   // IC for the scalar variable
   IC[3] = c21;
   inpFile.ignore(256, '\n');


   /********************************************************
   // Read SECTION PARAMETERS
   ********************************************************/
   inpFile.ignore(256, '\n');	   // Read and ignore the section title

   inpFile >> dummyInt;		      // Number of parameters
   inpFile.ignore(256, '\n');	   // Ignore the remaining of the last line read

   inpFile.get(c41,41);          // Read 40 character parameter title
   inpFile >> solverAlgorithm;   // Read algorithm (0: SIMPLE, 1: SIMPLEC, 2: SIMPLER)  // Cuneyt: Incompatibility between GUI and Solver
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> discSchm;          // Read space discretization scheme (0: Central, 1: Upwind, 2: Hybrid, 3: Power law)  // Cuneyt: Incompatibility between GUI and Solver
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> solverApproach;    // Read solution strategy (0: Iterative, 1: Time marching)  // Cuneyt: Incompatibility between GUI and Solver
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> timeStep;          // (Pseudo) time step
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> kinvis;            // Read kinematic viscosity
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> density;           // Read density
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> uRelax;            // Read u relaxation
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> vRelax;            // Read v relaxation
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> pRelax;            // Read p relaxation
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> scalarRelax;       // Read scalar relaxation
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> outerIterMax;      // Read max. number of outer iterations
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> saveInterval;      // Read save interval
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> controlPointSaveInterval;    // Read track (control) point save interval
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> outerTolerance;    // Read convergence tolerance of outer iterations
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> isTransient;       // Read the flag for time dependency of the flow
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> isTecplot;         // Read the flag for DAT file generation.   Cuneyt: The variable name should not be isTecplot, but rather something like isDAtfile.
   inpFile.ignore(256, '\n');

   inpFile.get(c41,41);
   inpFile >> isRestart;         // Read the flag for restarting from a previous result
   inpFile.ignore(256, '\n');


   /********************************************************
   // Read SECTION MESHPOINTS
   ********************************************************/
   inpFile.ignore(256, '\n');    // Read and ignore the section title
   inpFile.ignore(256, '\n');    // Read and ignore the block number line
   
   Xji = MemoryAllocater2D(numYcells+1, numXcells+1);
   Yji = MemoryAllocater2D(numYcells+1, numXcells+1);

   for (j=0; j < numYcells+1; j++) {
      for (i=0; i < numXcells+1; i++) {
         inpFile >> Xji[j][i] >> Yji[j][i];
      }
   }

   /********************************************************
   // Read SECTION CONTROLPOINTS
   ********************************************************/
   inpFile.ignore(256, '\n');    // Finish the remainings of the last line read
   inpFile.ignore(256, '\n');    // Read and ignore the section title
   inpFile >> dummyInt;          // Block number is ignored
   inpFile >> nControlPoints;    // # of track points of this block
   inpFile.ignore(256, '\n');    // Read and ignore the section title
   if (nControlPoints > 0) {     // Allocate TrackPointCoor array
      controlPointIndex = new int*[nControlPoints];
      for (i = 0; i < nControlPoints; i++) {
         controlPointIndex[i] = new int[2];
      }
   }
   for(k = 0; k < nControlPoints; k++) {
      inpFile >> dummyInt;       // Read track point. This is a number between zero and the total number of nodes of the block minus 1.
                                 // So we have to find the corresponding i and j indices.
      i = dummyInt % (numXcells+1);   // Remainder function
      j = dummyInt / (numXcells+1);   // Integer division
      controlPointIndex[k][0] = j;
      controlPointIndex[k][1] = i;
      inpFile.ignore(256, '\n');
   }


   /********************************************************
   // Read SECTION BLOCKEDCELLS
   ********************************************************/
   inpFile.ignore(256, '\n');    // Read and ignore the section title
   inpFile >> dummyInt;          // Block number is ignored
   inpFile >> nBlockCells;       // # of blocked cells of this block
   if (nBlockCells > 0) {        // Allocate BlockedCellCoor array
      BlockCellIndex = new int*[nBlockCells];
      for (i = 0; i < nBlockCells; i++) {
         BlockCellIndex[i] = new int[4];
      }
   }
   for(k = 0; k < nBlockCells; k++) {
      inpFile >> dummyInt;       // Read blocked cell number. This is a number between zero and the total number of cells of the block minus 1.
                                 // So we have to find the corresponding i and j indices.
      i = dummyInt % (numXcells);   // Remainder function
      j = dummyInt / (numXcells);   // Integer division
      BlockCellIndex[k][0] = j;
      BlockCellIndex[k][1] = j+1;
      BlockCellIndex[k][2] = i;
      BlockCellIndex[k][3] = i+1;
      inpFile.ignore(256, '\n');
   }



   /********************************************************
   // Calculate the variables for the indexing of the staggered grid
   ********************************************************/
   NumI = numXcells + 2;   // Number of vertical lines that keep pressure & Vvelocity
   NumJ = numYcells + 2;   // Number of horizontal lines that keep pressure & Uvelocity
   Numi = numXcells + 1;   // Number of vertical lines that keep Uvelocity
   Numj = numYcells + 1;   // Number of horizontal lines that keep Vvelocity


   /********************************************************
   // Allocate and setup the BC arrays
   ********************************************************/
   BoundaryLeft = MemoryAllocater2D(NumJ, 2);
   BoundaryRight = MemoryAllocater2D(NumJ, 2);
   BoundaryTop = MemoryAllocater2D(NumI, 2);
   BoundaryBottom = MemoryAllocater2D(NumI, 2);

   funcVals[0] = 0.0;   // x
   funcVals[1] = 0.0;   // y
   funcVals[2] = 0.0;   // t time   // Cuneyt: This value should also change for time-dependent BCs.

   fparser.Parse("0.0", "x,y,t");   // This dummy call of the functions parser are necessary for proper operation.
   funcResult = fparser.Eval(funcVals);


   // Cuneyt: Pay attention to the following points.
   // 1. Only one velocity value is specified at all boundaries. Whether this velocity is for u or v is determined later. This is not flexible enough.
   // 2. Only velocity information can be specified at boundaries. What about specifiying pressure ?
   // 3. Thnik more about coordinate dependent BCs.

   b = 0;  // Cuneyt: No multi-block support.

   int n1, n2;


   // EAST (RIGHT) Boundary                         Cuneyt: Check the correstness of the following
   // Mid-nodes of the East (right) boundary.
   for (i = 0; i < eastBCn; i++) {
      for (j = eastBCcells[i][0]; j < eastBCcells[i][1]; j++) {
         n1 = numXcells * j;
         n2 = n1 + numXcells;

         funcVals[0] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1] + problem->mesh->blocks[b].coordinates[2*n2]);      // x coordinate
         funcVals[1] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1+1] + problem->mesh->blocks[b].coordinates[2*n2+1]);  // y coordinate

         fparser.Parse(eastBC[i][0], "x,y,t");
         funcResult = fparser.Eval(funcVals);
         BoundaryRight[j][0] = eastBCtype[i];
         BoundaryRight[j][1] = funcResult;
      }
   }

   // Lower corner of the East (right) boundary.
   BoundaryRight[0][0] = BoundaryRight[1][0];
   BoundaryRight[0][1] = BoundaryRight[1][1];

   // Upper corner of the East (right) boundary.
   BoundaryRight[NumJ-1][0] = BoundaryRight[NumJ-2][0];
   BoundaryRight[NumJ-1][1] = BoundaryRight[NumJ-2][1];



   // WEST (left) Boundary
   // Mid-nodes of the West (left) boundary.
   for (i = 0; i < westBCn; i++) {
      for (j = westBCcells[i][0]; j < westBCcells[i][1]; j++) {
         // Cuneyt: Following is suitable to specify u velocity at the left boundary. Changes may needed to specify v velocity too.
         jj = numYcells - j;   // BCs of the West boundary and its node coordinates are oriented differently.
         n1 = numXcells * jj;
         n2 = n1 - numXcells;

         funcVals[0] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1] + problem->mesh->blocks[b].coordinates[2*n2]);      // x coordinate
         funcVals[1] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1+1] + problem->mesh->blocks[b].coordinates[2*n2+1]);  // y coordinate

         fparser.Parse(westBC[i][0], "x,y,t");   // Cuneyt: Second index is always zero because for the time being only a single veocity is used at each boundary.
         funcResult = fparser.Eval(funcVals);
         BoundaryLeft[jj][0] = westBCtype[i];
         BoundaryLeft[jj][1] = funcResult;
      }
   }

   // Lower corner of the West (left) boundary.
   BoundaryLeft[0][0] = BoundaryLeft[1][0];
   BoundaryLeft[0][1] = BoundaryLeft[1][1];

   // Upper corner of the West (left) boundary.
   BoundaryLeft[NumJ-1][0] = BoundaryLeft[NumJ-2][0];
   BoundaryLeft[NumJ-1][1] = BoundaryLeft[NumJ-2][1];


   // EAST (right) Boundary                             Cuneyt: Check the correstness of the following
   // Mid-nodes of the East (right) boundary.
   for (i = 0; i < eastBCn; i++) {
      for (j = eastBCcells[i][0]; j < eastBCcells[i][1]; j++) {
         n1 = numXcells * j;
         n2 = n1 + numXcells;

         //funcVals[0] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1] + problem->mesh->blocks[b].coordinates[2*n2]);      // x coordinate
         //funcVals[1] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1+1] + problem->mesh->blocks[b].coordinates[2*n2+1]);  // y coordinate

         fparser.Parse(eastBC[i][0], "x,y,t");
         funcResult = fparser.Eval(funcVals);
         BoundaryRight[j][0] = eastBCtype[i];
         BoundaryRight[j][1] = funcResult;
      }
   }

   // Lower corner of the East (right) boundary.
   BoundaryRight[0][0] = BoundaryRight[1][0];
   BoundaryRight[0][1] = BoundaryRight[1][1];

   // Upper corner of the East (right) boundary.
   BoundaryRight[NumJ-1][0] = BoundaryRight[NumJ-2][0];
   BoundaryRight[NumJ-1][1] = BoundaryRight[NumJ-2][1];


/*
   // First node of the right (east) boundary.
   fparser.Parse(eastBC[0][0], "x,y,t");
   funcResult = fparser.Eval(funcVals);
   BoundaryRight[0][0] = eastBCtype[0];
   BoundaryRight[0][1] = funcResult;
   // Mid-nodes of the right boundary.
   for (i = 0; i < eastBCn; i++) {
      for (j = eastBCcells[i][0]; j <= eastBCcells[i][1]; j++) {
         fparser.Parse(eastBC[i][0], "x,y,t");
         funcResult = fparser.Eval(funcVals);
         BoundaryRight[j+1][0] = eastBCtype[i];
         BoundaryRight[j+1][1] = funcResult;
      }
   }
   // Last node of the right boundary.
   fparser.Parse(eastBC[eastBCn-1][0], "x,y,t");
   funcResult = fparser.Eval(funcVals);
   BoundaryLeft[NumJ-1][0] = eastBCtype[eastBCn-1];
   BoundaryLeft[NumJ-1][1] = funcResult;
*/

   // First node of the top (north) boundary.
   fparser.Parse(northBC[0][0], "x,y,t");
   funcResult = fparser.Eval(funcVals);
   BoundaryTop[0][0] = northBCtype[0];
   BoundaryTop[0][1] = funcResult;
   // Mid-nodes of the top boundary.
   for (i = 0; i < northBCn; i++) {
      for (j = northBCcells[i][0]; j <= northBCcells[i][1]; j++) {
         // Cuneyt: Following is suitable to specify v velocity at the top boundary. Changes may needed to specify u velocity too.
         fparser.Parse(northBC[i][0], "x,y,t");   // Cuneyt: Second index is always zero because for the time being only a single veocity is used at each boundary.
         funcResult = fparser.Eval(funcVals);
         BoundaryTop[j+1][0] = northBCtype[i];
         BoundaryTop[j+1][1] = funcResult;
      }
   }
   // Last node of the top boundary.
   fparser.Parse(northBC[northBCn-1][0], "x,y,t");
   funcResult = fparser.Eval(funcVals);
   BoundaryTop[NumI-1][0] = northBCtype[northBCn-1];
   BoundaryTop[NumI-1][1] = funcResult;


   // First node of the bottom (south) boundary.
   fparser.Parse(southBC[0][0], "x,y,t");
   funcResult = fparser.Eval(funcVals);
   BoundaryBottom[0][0] = southBCtype[0];
   BoundaryBottom[0][1] = funcResult;
   // Mid-nodes of the bottom boundary.
   for (i = 0; i < southBCn; i++) {
      for (j = southBCcells[i][0]; j <= southBCcells[i][1]; j++) {
         fparser.Parse(southBC[i][0], "x,y,t");
         funcResult = fparser.Eval(funcVals);
         BoundaryBottom[j+1][0] = southBCtype[i];
         BoundaryBottom[j+1][1] = funcResult;
      }
   }
   // Last node of the bottom boundary.
   fparser.Parse(southBC[southBCn-1][0], "x,y,t");
   funcResult = fparser.Eval(funcVals);
   BoundaryBottom[NumI-1][0] = southBCtype[southBCn-1];
   BoundaryBottom[NumI-1][1] = funcResult;



/* NEW BUT NOT WORKING

   // EAST (RIGHT) Boundary                         Cuneyt: Test the correctness of the folowing code
   // Mid-nodes of the East (right) boundary.
   for (i = 0; i < eastBCn; i++) {
      for (j = eastBCcells[i][0]; j < eastBCcells[i][1]; j++) {
         n1 = numXcells * j;
         n2 = n1 + numXcells;

         //funcVals[0] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1] + problem->mesh->blocks[b].coordinates[2*n2]);      // x coordinate
         //funcVals[1] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1+1] + problem->mesh->blocks[b].coordinates[2*n2+1]);  // y coordinate

         fparser.Parse(eastBC[i][0], "x,y,t");
         funcResult = fparser.Eval(funcVals);
         BoundaryRight[j][0] = eastBCtype[i];
         BoundaryRight[j][1] = funcResult;
      }
   }
   
   // Lower corner of the East (right) boundary.
   BoundaryRight[0][0] = BoundaryRight[1][0];
   BoundaryRight[0][1] = BoundaryRight[1][1];

   // Upper corner of the East (right) boundary.
   BoundaryRight[NumJ-1][0] = BoundaryRight[NumJ-2][0];
   BoundaryRight[NumJ-1][1] = BoundaryRight[NumJ-2][1];



   // WEST (LEFT) Boundary
   // Mid-nodes of the West (left) boundary.
   for (i = 0; i < westBCn; i++) {
      for (j = westBCcells[i][0]; j < westBCcells[i][1]; j++) {
         // cuneyt: Asagida yapilan sol sinirda u hizini vermeye uyun. v hizi da verilecekse farkliliklar gerekebilir.
         jj = numYcells - j;   // BCs of the West boundary and its node coordinates are oriented differently.
         n1 = numXcells * jj;
         n2 = n1 - numXcells;

         //funcVals[0] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1] + problem->mesh->blocks[b].coordinates[2*n2]);      // x coordinate
         //funcVals[1] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1+1] + problem->mesh->blocks[b].coordinates[2*n2+1]);  // y coordinate

         fparser.Parse(westBC[i][0], "x,y,t");		// Cuneyt: Second index is always zero because for the time being only a single veocity is used at each boundary
         funcResult = fparser.Eval(funcVals);
         BoundaryLeft[jj][0] = westBCtype[i];
         BoundaryLeft[jj][1] = funcResult;
      }
   }

   // Lower corner of the West (left) boundary.
   BoundaryLeft[0][0] = BoundaryLeft[1][0];
   BoundaryLeft[0][1] = BoundaryLeft[1][1];

   // Upper corner of the West (left) boundary.
   BoundaryLeft[NumJ-1][0] = BoundaryLeft[NumJ-2][0];
   BoundaryLeft[NumJ-1][1] = BoundaryLeft[NumJ-2][1];



   // SOUTH (BOTTOM) Boundary                         Cuneyt: Test the correctness of the folowing code
   // Mid-nodes of the bottom (South) boundary.
   for (i = 0; i < southBCn; i++) {
      for (j = southBCcells[i][0]; j < southBCcells[i][1]; j++) {
         n1 = j;
         n2 = n1 + 1;

         //funcVals[0] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1] + problem->mesh->blocks[b].coordinates[2*n2]);      // x coordinate
         //funcVals[1] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1+1] + problem->mesh->blocks[b].coordinates[2*n2+1]);  // y coordinate


         fparser.Parse(southBC[i][0], "x,y,t");
         funcResult = fparser.Eval(funcVals);
         BoundaryBottom[j][0] = southBCtype[i];
         BoundaryBottom[j][1] = funcResult;
      }
   }

   // Left corner of the South (bottom) boundary.
   BoundaryBottom[0][0] = BoundaryBottom[1][0];
   BoundaryBottom[0][1] = BoundaryBottom[1][1];

   // Right corner of the South (bottom) boundary.
   BoundaryBottom[NumI-1][0] = BoundaryBottom[NumI-2][0];
   BoundaryBottom[NumI-1][1] = BoundaryBottom[NumI-2][1];



   // NORTH (TOP) Boundary                         Cuneyt: Test the correctness of the folowing code
   // Mid-nodes of the North (Top) boundary.
   for (i = 0; i < northBCn; i++) {
      for (j = northBCcells[i][0]; j < northBCcells[i][1]; j++) {
         // cuneyt: Asagida yapilan ust sinirda v hizini vermeye uyun. u hizi icin farkliliklar gerekebilir.
         jj = numXcells - j;   // BCs of the North boundary and its node coordinates are oriented differently.
         n1 = numXcells * jj;
         n2 = n1 - 1;

         //funcVals[0] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1] + problem->mesh->blocks[b].coordinates[2*n2]);      // x coordinate
         //funcVals[1] = 0.5 * (problem->mesh->blocks[b].coordinates[2*n1+1] + problem->mesh->blocks[b].coordinates[2*n2+1]);  // y coordinate

         fparser.Parse(northBC[i][0], "x,y,t");		// Cuneyt: Second index is always zero because for the time being only a single veocity is used at each boundary
         funcResult = fparser.Eval(funcVals);
         BoundaryTop[jj][0] = northBCtype[i];
         BoundaryTop[jj][1] = funcResult;
      }
   }

   // Left corner of the North (top) boundary.
   BoundaryTop[0][0] = BoundaryTop[1][0];
   BoundaryTop[0][1] = BoundaryTop[1][1];

   // Right corner of the North (top) boundary.
   BoundaryTop[NumI-1][0] = BoundaryTop[NumI-2][0];
   BoundaryTop[NumI-1][1] = BoundaryTop[NumI-2][1];
*/

   /********************************************************
   // Allocate and setup the IC arrays
   ********************************************************/
   pJI = MemoryAllocater2D(NumJ, NumI);
   UJi = MemoryAllocater2D(NumJ, Numi);
   VjI = MemoryAllocater2D(Numj, NumI);
   ScalarVar   = MemoryAllocater2D(NumJ, NumI);

   if (isRestart == 1) {
      ifstream restartFile;
      string restartString = fullProblemName + ".restart";   // e.g. C:\VFL\Problems\MyProblem.restart
      restartFile.open(restartString.data(), ios::in);
      if (restartFile.good() == false)
         return -2;  // Means something is wrong with the restart file.

      for(i = 1; i <= 3; i++)
         restartFile.ignore(1024,'\n');

      for(j = 0; j < NumJ; j++)
         for(i = 0; i < NumI; i++)
            restartFile >> pJI[j][i];

      for(i = 1; i <= 4; i++)
         restartFile.ignore(1024,'\n');

      for(j = 0; j < NumJ; j++)
         for(i = 0; i < Numi; i++)
            restartFile >> UJi[j][i];

      for(i = 1; i <= 4; i++)
         restartFile.ignore(1024,'\n');
      
      for(j = 0; j < Numj; j++)
         for(i = 0; i < NumI; i++)
            restartFile >> VjI[j][i];

      for(i = 1; i <= 4; i++)
         restartFile.ignore(1024,'\n');
      
      for(j = 0; j < NumJ; j++)
         for(i = 0; i < NumI; i++)
            restartFile >> ScalarVar[j][i];

      restartFile.close();
   } else {
      fparser.Parse(IC[0], "x,y,t");
      funcResult = fparser.Eval(funcVals);
      for (i = 0; i < NumI; i++)
         for (j = 0; j < NumJ; j++)
            pJI[j][i] = funcResult;

      fparser.Parse(IC[1], "x,y,t");
      funcResult = fparser.Eval(funcVals);
      for (i = 0; i < Numi; i++)
         for (j = 0; j < NumJ; j++)
            UJi[j][i] = funcResult;

      fparser.Parse(IC[2], "x,y,t");
      funcResult = fparser.Eval(funcVals);
      for (i = 0; i < NumI; i++)
         for (j = 0; j < Numj; j++)
            VjI[j][i] = funcResult;

      fparser.Parse(IC[3], "x,y,t");
      funcResult = fparser.Eval(funcVals);
      for (i = 0; i < NumI; i++)
         for (j = 0; j < NumJ; j++)
            ScalarVar[j][i] = funcResult;
   }

   // Deallocate local variables
   delete[] southBCtype;
   delete[] eastBCtype;
   delete[] northBCtype;
   delete[] westBCtype;

   for(i = 0; i < southBCn; i++) { delete[] southBC[i]; delete[] southBCcells[i]; }
   for(i = 0; i < eastBCn; i++)  { delete[] eastBC[i];  delete[] eastBCcells[i];  }
   for(i = 0; i < northBCn; i++) { delete[] northBC[i]; delete[] northBCcells[i]; }
   for(i = 0; i < westBCn; i++)  { delete[] westBC[i];  delete[] westBCcells[i];  }

   delete[] southBC;  delete[] southBCcells;
   delete[] eastBC;   delete[] eastBCcells;
   delete[] northBC;  delete[] northBCcells;
   delete[] westBC;   delete[] westBCcells;

   southBC = NULL;  southBCcells = NULL;
   eastBC  = NULL;  eastBCcells  = NULL;
   northBC = NULL;  northBCcells = NULL;
   westBC  = NULL;  westBCcells  = NULL;

   inpFile.close();

   return 0;  // Successfull return

}  // End of function  ReadInputFile()
