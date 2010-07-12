// This is the implementation file problem.cpp.

#include "guiProblem.h"
#include "guiTypedefs.h"




Problem::Problem(void)
{
   // This is called when the program is first started or just before opening a new problem.
   mainState = GEOM;
   drawingWhat = NOPRIMITIVE;
   glMouseState = SELECTION;
   visualizeState = SELECT;
//   state.whatsDone = NOTHING;

   meshGenerationAlgorithm = 0;

   name = "";
   dir = "";
   cfdFileName = "";
   resultFileName = "";

   //corFileName = name  +  "_cor.txt" ;  // Bu 3'u kaldirilabilir.
   //bicFileName = name  +  "_bic.txt" ;
   //parFileName = name  +  "_par.txt" ;
   //outFileName = name  +  "_out.txt" ;
   //tecFileName = name  +  "_tec.txt" ;

   ICstring[0] = "";     // No IC is set yet for u-velocity
   ICstring[1] = "";     // No IC is set yet for v-velocity
   ICstring[2] = "";     // No IC is set yet for pressure

   scheme          = 0;      // Default scheme is SIMPLE
   discretization  = 0;      // Default discretization is Central
   strategy        = 0;      // Default solution strategy is Iterative
   isRestart       = FALSE;  // Default behavior is NOT to restart
   isTecplot       = TRUE;   // Default behavior is NOT to create Tecplot outputs.
   isTimeDependent = FALSE;  // Default behavior is steady flow.
   //nParameters = 17;       // cuneyt: Should this be here? Do not forget to update this number as new parameters are added.

   // meshType = STRUCTURED;		// cuneyt: Bunu da kullanicinin secebilmesi gerek.
   mesh = new StructuredMesh;

   needToSave = true;        // Set to true if a change is made to the problem. Currently it always stays as true.

   // The following are about the visualization page
   uResult        = NULL;
   vResult        = NULL;
   pResult        = NULL;
   showMesh       = FALSE;
   showBoundary   = FALSE;
   showContour    = FALSE;
   showStreamline = FALSE;
   contourVar     = 0;       // u velocity
   contourStyle   = 0;       // Continuous flood
   nContourLevels = 50;
   colorMap       = 1;       // Small rainbow
   probeType      = 2;       // Probe at index.
   probeResultX   = probeResultY = probeResultU = probeResultV = probeResultP = 0.0;
   nStreamlines   = 0;
} // End of function Problem::Problem()




Problem::~Problem(void)
{
   int i, j;

   delete mesh;

   // Delete streamline coordinates
   for(i=0; i<nStreamlines; i++) {
       for(j=0; j<streamlines[i].nPoints; j++) {
          delete[] streamlines[i].coor[j];
       }
       delete[] streamlines[i].coor;
   }
}




void Problem::setFileNames(string s1, string s2) {
   cfdFileName     = s1 + "/" + s2 + ".cfd";
   inputFileName   = s1 + "/" + s2 + ".inp";
   defaultFileName = s1 + "/" + "DEFAULT.txt";
}
