#include <QtGui>
#include <math.h>
#include <iomanip>
#include <fstream>
#include "mainWindow.h"
#include "glwidget.h"
#include "guiTypedefs.h"
#include "guiPrimitive.h"
#include "guiProblem.h"

using namespace std;

extern Problem *problem;




void mainWindow::generateMeshClicked(void)
{
   // Show all primitives in black.
   if(problem->mesh->getPrimitiveUnderMouse() != -1) {
      problem->mesh->primitives[problem->mesh->getPrimitiveUnderMouse()].setColor(Qt::black);
      problem->mesh->setPrimitiveUnderMouse(-1);
      glWidget->update();
   }

   // Check if the geometry is closed or not.
   if ( ! problem->mesh->isGeometryClosed() ) {
      // Cuneyt: Primitives that have an open end can be painted with a different color.
      appendMessage(tr("ERROR: Mesh can not be generated because the geometry is not closed. Please return to the Geometry page."), Qt::red);
      return;
   }

   // Deallocate previously created blocks.
   // Cuneyt: Memory leaks should be controlled carefully.
   delete[] problem->mesh->blocks;

   // Calculate the number of blocks used.
   problem->mesh->calculateNumberOfBlocks();

   // Allocate blocks.
   problem->mesh->allocateBlocks();

    // Calculate how many primitives are used on each face of each block;
   problem->mesh->calculateNumberOfPrimitivesOnFaces();

   // Allocate the dynamic data for the faces
   problem->mesh->allocateFaceData();

   // Set the primitive numbers for all faces.
   problem->mesh->setFacePrimitives();

   // Calculate the number of boundary points for all faces.
   problem->mesh->calculateNumberOfBoundaryPointsOnFaces();

    // Create ordered list of face primitives and check for errors
   if (! problem->mesh->isFacePrimitiveOrderingCorrect() ) {
      appendMessage(tr("ERROR: Mesh can not be generated due to a problem in face orientations or face node numbers."), Qt::red);
      return;
      // outputText->append("Face ordering is not correct.");
      // cuneyt: Hatali face'ler farkli bir renge boyanabilir.
   }
   
   // Check if the total points of the corresponding faces are matching
   if (! problem->mesh->doSW_NEpointsMatch() ) {
      appendMessage(tr("ERROR: Mesh can not be generated due to a problem in face orientations or face node numbers."), Qt::red);
      return;
      // outputText->append("Number of points on the North-South and/or East-West faces are not matching.");
      // Cuneyt: Detailed warnings such as "S-N numbers do not match in block 2" can be provided.
      //         Faulty faces can be painted in a different color.
   }


   // If the above controls are passed, now we can generate the mesh
   if (meshMethodCombo->currentIndex() == 0 ) {
      problem->mesh->generateLinearTFIMesh();
   } else if (meshMethodCombo->currentIndex() == 1 ) {
      problem->mesh->generateLaplaceMesh();
   }

   problem->mesh->setIsMeshGenerated(TRUE);

   // Show the mesh.
   glWidget->generateMeshList();		   // Generate the glList for the mesh.
   glWidget->generateMeshPointList();  // Generate the glList for the mesh points (used for the selection of control points).
   glWidget->update();

   appendMessage(tr("Mesh is generated."), Qt::black);
   // Cuneyt: Number of blocks, mesh size of each block, total node and cell numbers can be povided.

}  // End of function generateMeshClicked()
