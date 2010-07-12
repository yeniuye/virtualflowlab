#include <QtGui>
#include <QtOpenGL>
#include <math.h>

#include "guiTypedefs.h"
#include "glwidget.h"
#include "guiProblem.h"

extern Problem *problem;




GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
   this->initializeGL();
   setMouseTracking(TRUE);  // Necessary to call MouseMoveEvent even tough mouse buttons are not pressed.
}




GLWidget::~GLWidget()
{
}




void GLWidget::initializeGL()
{
	XtoYratio = 1.0;
   minX = -10.0;   minXprev = -10.0;
   maxX =  10.0;   maxXprev =  10.0;
   minY = -10.0;   minYprev = -10.0;
   maxY =  10.0;   maxYprev =  10.0;
	backgroundColor = Qt::white;
	object = 0;
	qglClearColor( backgroundColor );
	glClear (GL_COLOR_BUFFER_BIT);
	glOrtho(minX, maxX, minY, maxY, -1.0, 1.0);   // Specifies the coordinate system, LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR

   isRubberBandZooming = FALSE;
   isContinuousZooming = FALSE;
   isPanning = FALSE;

   wPrev = this->width();
   hPrev = this->height();
}




void GLWidget::paintGL(void)
{
   QColor blockedCellColor;

	// The following is to make the points and lines antialiased
	/*
	glEnable (GL_LINE_SMOOTH);
	// glEnable (GL_POINT_SMOOTH);   // Cuneyt: This slows down the code a lot.
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	*/

	// Clear the OpenGL screen
	qglClearColor(backgroundColor);
	glClear (GL_COLOR_BUFFER_BIT);

   if (problem->getMainState() == GEOM) {
      drawPrimitives();
      drawBlockedCells(Qt::red);
	} else if (problem->getMainState() == MESH) {
      drawPrimitives();
		drawStructuredMesh();
      drawBoundaryPoints();
      drawBlockedCells(Qt::red);
	} else if (problem->getMainState() == BCIC) {
      drawPrimitives();
      drawBlockedCells(Qt::red);
   } else if (problem->getMainState() == PARAMETERS) {
      drawPrimitives();
      drawBlockedCells(Qt::red);
	} else if (problem->getMainState() == CONTROLPOINTS) {
		drawMeshPoints();
      drawStructuredMesh();
      drawBlockedCells(Qt::red);
		drawControlPoints();
	} else if (problem->getMainState() == BLOCKEDCELLS) {
		drawStructuredMesh();
      drawBlockedCells(Qt::red);
   } else if (problem->getMainState() == VISUALIZE) {
      if (problem->getResultFileName() != "") {

         // Determine the blockedCellColor
         blockedCellColor = Qt::red;
         if (problem->showContour && problem->colorMap == 1 && !problem->showMesh)
            blockedCellColor = Qt::black;

         if (problem->showContour)
            drawContour();
         if (problem->showMesh)
            drawStructuredMesh();
         if (problem->showBoundary || problem->showMesh || problem->showContour)
            drawBlockedCells(blockedCellColor);
         if (problem->showBoundary)
            drawPrimitives();
         if (problem->showStreamline)
            drawStreamline();
      }
   }

   if (isRubberBandZooming)  drawZoomRectangle();

	//renderText(40, 40, "x: 0.00000");  //, const QFont & fnt = QFont(), int listBase = 2000 )

}  // End of paintGL




void GLWidget::drawPrimitives(void)
{
   float *dummyCoor;
   int i;

   for (i = 0; i < MAX_PRIMITIVES; i++) {
      if (problem->mesh->primitives[i].getIsDeleted())  continue;

      dummyCoor = problem->mesh->primitives[i].getDefPointCoor();

      glLoadName(i);
      qglColor (problem->mesh->primitives[i].getColor());
      glLineWidth (problem->mesh->primitives[i].getThickness());

      // if (problem->mesh->primitives[i].getType() == LINE ) {
         // Draw Lines
         glBegin(GL_LINES);
            glVertex2f (dummyCoor[0], dummyCoor[1]);
            glVertex2f (dummyCoor[2], dummyCoor[3]);
         glEnd();
/*
      } else if (problem->mesh->primitives[i].getType() == ARC ) {
         // Draw Circular Arcs
         glBegin(GL_LINE_STRIP);
            for (j = 0; j < problem->mesh->primitives[i].getNumberOfDefPoints(); j++)
               glVertex2f (dummyCoor[2*j], dummyCoor[2*j+1]);
         glEnd();
      } else if (problem->mesh->primitives[i].getType() == FUNCTION ) {
         // Draw curves defined by Functions
         glBegin(GL_LINE_STRIP);
            for (j = 0; j < problem->mesh->primitives[i].getNumberOfDefPoints(); j++)
               glVertex2f (dummyCoor[2*j], dummyCoor[2*j+1]);
          glEnd();
      }
*/
   }

   glFlush ();  // Executes the drawing commands immediately

}  // End of function drawPrimitives()



void GLWidget::drawBoundaryPoints(void)
{
	int i, j;
	float *dummyCoor;
	
	for (i = 0; i < MAX_PRIMITIVES; i++) {
      if (problem->mesh->primitives[i].getIsDeleted())  continue;
		
		dummyCoor = problem->mesh->primitives[i].getPointCoor();

		qglColor (problem->mesh->primitives[i].getColor());
		glPointSize (4.0);
			
		// Draw points
		glBegin(GL_POINTS);
		for (j = 0; j < problem->mesh->primitives[i].getNumberOfPoints(); j++)
			glVertex2f (dummyCoor[2*j], dummyCoor[2*j+1]);
		glEnd();
			
      glFlush ();  // Executes the drawing commands immediately
	}
}




void GLWidget::generateMeshList(void)
{
   int b, i, j, nX, nY, c1, c2, c3, c4, c1_2, c2_2, c3_2, c4_2, cellCounter;
   if (! problem->mesh->getIsMeshGenerated()) return;

	glDeleteLists(1,1);  // Delete the list with number 1.
	glNewList(1, GL_COMPILE);

   for (b = 0; b < problem->mesh->getnBlocks(); b++ ) {
		nX = problem->mesh->blocks[b].getnXpoints();
		nY = problem->mesh->blocks[b].getnYpoints();
		cellCounter = 0;
		for (j = 0; j < nY-1; j++ ) {
			for (i = 0; i < nX-1; i++ ) {
				c1 = j * nX + i;
				c2 = c1 + 1;
				c3 = c2 + nX;
				c4 = c1 + nX;

            c1_2 = 2*c1;
            c2_2 = 2*c2;
            c3_2 = 2*c3;
            c4_2 = 2*c4;

				glLoadName(cellCounter);  // First cell is named 0, last cell is named (nX-1)*(nY-1)-1
				cellCounter = cellCounter + 1;
				glBegin(GL_POLYGON);
               glVertex2f(problem->mesh->blocks[b].coordinates[c1_2], problem->mesh->blocks[b].coordinates[c1_2 + 1]);
               glVertex2f(problem->mesh->blocks[b].coordinates[c2_2], problem->mesh->blocks[b].coordinates[c2_2 + 1]);
               glVertex2f(problem->mesh->blocks[b].coordinates[c3_2], problem->mesh->blocks[b].coordinates[c3_2 + 1]);
               glVertex2f(problem->mesh->blocks[b].coordinates[c4_2], problem->mesh->blocks[b].coordinates[c4_2 + 1]);
				glEnd();
			}
		}
	}
	glEndList();
}  // End of function generateMeshList()




void GLWidget::drawStructuredMesh(void)
{
   if (! problem->mesh->getIsMeshGenerated()) return;

   qglColor(Qt::black);
   glLineWidth(1.0);
   glPolygonMode(GL_FRONT, GL_LINE);

   glCallList(1);	// List 1 is for the mesh
   glFlush ();
}




void GLWidget::generateMeshPointList(void)
{
   int b, i, j, k;
   if (! problem->mesh->getIsMeshGenerated()) return;

   glDeleteLists(2,1);  // Delete the list with number 2.
   glNewList(2, GL_COMPILE);
	
   for (b = 0; b < problem->mesh->getnBlocks(); b++ ) {
      for (i = 0; i < problem->mesh->blocks[b].getnXpoints(); i++ ) {
         for (j = 0; j < problem->mesh->blocks[b].getnYpoints(); j++ ) {
            k = i + j * problem->mesh->blocks[b].getnXpoints();
            glLoadName(k);  // First point is named as 0, last point is named as nX*nY-1
            glBegin(GL_POINTS);
            glVertex2f (problem->mesh->blocks[b].coordinates[2*k], problem->mesh->blocks[b].coordinates[2*k+1]);
            glEnd();
         }
		}
	}
   glEndList();
   glFlush ();
}




void GLWidget::drawMeshPoints(void)
{
   if (! problem->mesh->getIsMeshGenerated()) return;

   qglColor(Qt::black);
   glPointSize(1.0);

   glCallList(2);	// List 2 is for the mesh points
   glFlush ();
}




void GLWidget::drawControlPoints(void)
{
   int c, k;

   if (! problem->mesh->getIsMeshGenerated()) return;

   qglColor(Qt::blue);
   glPointSize(6.0);
	
   // Cuneyt: No multi-block support. Only works for the 0th block.
   for (c = 0; c < problem->mesh->blocks[0].getnControlPoints(); c++) {
      k = problem->mesh->blocks[0].controlPoints[c];
      glBegin(GL_POINTS);
      glVertex2f (problem->mesh->blocks[0].coordinates[2*k], problem->mesh->blocks[0].coordinates[2*k+1]);
      glEnd();
   }
   glFlush();
}



void GLWidget::drawBlockedCells(QColor cellColor)
{
   int b, c1, c2, c3, c4, i, j, nX, nY, cellCounter;
   
   if (! problem->mesh->getIsMeshGenerated()) return;

   qglColor(cellColor);
   glPolygonMode(GL_FRONT, GL_FILL);

   for (b = 0; b < problem->mesh->getnBlocks(); b++ ) {
      nX = problem->mesh->blocks[b].getnXpoints();
      nY = problem->mesh->blocks[b].getnYpoints();
      cellCounter = 0; 
      for (j = 0; j < nY-1; j++ ) {
         for (i = 0; i < nX-1; i++ ) {
            if (problem->mesh->blocks[b].isCellBlocked[cellCounter] == 1) {  // This is blocked cell. Draw it as red.
               c1 = j * nX + i;
               c2 = c1 + 1;
               c3 = c2 + nX;
               c4 = c1 + nX;
               glBegin(GL_POLYGON);
                  glVertex2f(problem->mesh->blocks[b].coordinates[2*c1], problem->mesh->blocks[b].coordinates[2*c1+1]);
                  glVertex2f(problem->mesh->blocks[b].coordinates[2*c2], problem->mesh->blocks[b].coordinates[2*c2+1]);
                  glVertex2f(problem->mesh->blocks[b].coordinates[2*c3], problem->mesh->blocks[b].coordinates[2*c3+1]);
                  glVertex2f(problem->mesh->blocks[b].coordinates[2*c4], problem->mesh->blocks[b].coordinates[2*c4+1]);
               glEnd();
            }
            cellCounter = cellCounter + 1;
         }
      }
   }
   glFlush();
}  // End of function drawBlockedCells()




void GLWidget::drawContour(void)
{
   if (! problem->mesh->getIsMeshGenerated()) return;

	if (problem->contourStyle == 0 && problem->colorMap == 0)
		drawBWCells();
	else if (problem->contourStyle == 1 && problem->colorMap == 0)
		drawAverageBWCells();
	else if (problem->contourStyle == 0 && problem->colorMap == 1)
		drawColoredCells();
	else if (problem->contourStyle == 1 && problem->colorMap == 1)
		drawAverageColoredCells();
}



void GLWidget::drawBWCells(void)
{
	// Cuneyt: No multi-block support.

   int b, i, j, k, n, c1, c2, c3, c4, nX, nY, red, green, blue, nLevels, *colorIndices;
   double contourVarMin, contourVarMax, value, *levels;
   double **result;

   if (! problem->mesh->getIsMeshGenerated()) return;

   nLevels = problem->nContourLevels;
   levels = new double[nLevels+1];
   colorIndices = new int[nLevels];

   if (problem->contourVar == 0)      result = problem->uResult;
	else if (problem->contourVar == 1) result = problem->vResult;
   else                               result = problem->pResult;

   glPolygonMode(GL_FRONT, GL_FILL);

   // According to the Grayscale coloring scheme (White-Black)
   int colorMap[256][3];
   // Red = Green = Blue
   for(i=0; i<=255; i++) {
      colorMap[i][0] = i;
      colorMap[i][1] = i;
      colorMap[i][2] = i;
   }

   for (b = 0; b < problem->mesh->getnBlocks(); b++ ) {
		nX = problem->mesh->blocks[b].getnXpoints();
		nY = problem->mesh->blocks[b].getnYpoints();

		// Calculate min and max values of the the contour variable.
		contourVarMin = result[0][0];
		contourVarMax = result[0][0];
		for(i=0; i<nX; i++) {
			for(j=0; j<nY; j++) {
            if (result[i][j] < contourVarMin) contourVarMin = result[i][j];
            if (result[i][j] > contourVarMax) contourVarMax = result[i][j];
			}
		}

      for(i=0; i<=nLevels; i++) {
         levels[i] = contourVarMin + i * (contourVarMax - contourVarMin) / nLevels;
      }

      for(i=0; i<nLevels; i++) {
         value = 0.5 * (levels[i] + levels[i+1]);
                        colorIndices[i] = int (255 * (value - levels[0]) / (levels[nLevels] - levels[0]));  // colorMap index for the value that corresponds to  0.5*(levels[i] + levels[i+1])
      }

		for (j = 0; j < nY-1; j++ ) {
			for (i = 0; i < nX-1; i++ ) {
				c1 = j * nX + i;
				c2 = c1 + 1;
				c3 = c2 + nX;
				c4 = c1 + nX;

				glBegin(GL_POLYGON);
					value = result[i][j];		// Value of the contour variable at the bottom left corner of the cell.
               n = findContourInterval(value, nLevels, levels, -1);	// n=5 means value is between levels[5] and levels[6]
               if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?
               if (n > nLevels-1) n= nLevels-1;
               k = colorIndices[n];
               red = colorMap[k][0];
               green = colorMap[k][1];
               blue = colorMap[k][2];
               glColor3f(red/255.0, green/255.0, blue/255.0);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c1], problem->mesh->blocks[b].coordinates[2*c1+1]);

               value = result[i+1][j];		// Value of the contour variable at the bottom right corner of the cell.
               n = findContourInterval(value, nLevels, levels, n);
               if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?
               if (n > nLevels-1) n= nLevels-1;
               k = colorIndices[n];
               red = colorMap[k][0];
               green = colorMap[k][1];
               blue = colorMap[k][2];
               glColor3f(red/255.0, green/255.0, blue/255.0);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c2], problem->mesh->blocks[b].coordinates[2*c2+1]);

               value = result[i+1][j+1];	// Value of the contour variable at the top right corner of the cell.
               n = findContourInterval(value, nLevels, levels, n);
               if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?   
               if (n > nLevels-1) n= nLevels-1;
               k = colorIndices[n];
               red = colorMap[k][0];
               green = colorMap[k][1];
               blue = colorMap[k][2];
               glColor3f(red/255.0, green/255.0, blue/255.0);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c3], problem->mesh->blocks[b].coordinates[2*c3+1]);

               value = result[i][j+1];		// Value of the contour variable at the top left corner of the cell.
               n = findContourInterval(value, nLevels, levels, n);
               if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?
               if (n > nLevels-1) n= nLevels-1;
               k = colorIndices[n];
               red = colorMap[k][0];
               green = colorMap[k][1];
               blue = colorMap[k][2];
               glColor3f(red/255.0, green/255.0, blue/255.0);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c4], problem->mesh->blocks[b].coordinates[2*c4+1]);
            glEnd();
			}
		}
	}
   //drawBlockedCells(Qt::red);
	glFlush ();
}  // End of function drawBWCells()





void GLWidget::drawAverageBWCells(void)
{
	// Cuneyt: No multi-block support.

   int b, i, j, k, n, c1, c2, c3, c4, nX, nY, red, green, blue, nLevels, *colorIndices;;
   double contourVarMin, contourVarMax, value, value1, value2, value3, value4, valueAve, *levels;
   double ** result;

   if (! problem->mesh->getIsMeshGenerated()) return;

   nLevels = problem->nContourLevels;
   levels = new double[nLevels+1];
   colorIndices = new int[nLevels];

	if (problem->contourVar == 0)      result = problem->uResult;
	else if (problem->contourVar == 1) result = problem->vResult;
   else                               result = problem->pResult;

   glPolygonMode(GL_FRONT, GL_FILL);

   // According to the Grayscale coloring scheme (White-Black)
   int colorMap[255][3];
   // Red = Green = Blue
   for(i=0; i<=255; i++) {
      colorMap[i][0] = i;
      colorMap[i][1] = i;
      colorMap[i][2] = i;
   }

   for (b = 0; b < problem->mesh->getnBlocks(); b++ ) {
		nX = problem->mesh->blocks[b].getnXpoints();
		nY = problem->mesh->blocks[b].getnYpoints();

		// Calculate min and max values of the the contour variable.
		contourVarMin = result[0][0];
		contourVarMax = result[0][0];
		for(i=0; i<nX; i++) {
			for(j=0; j<nY; j++) {
            if (result[i][j] < contourVarMin) contourVarMin = result[i][j];
            if (result[i][j] > contourVarMax) contourVarMax = result[i][j];
			}
		}

      for(i=0; i<=nLevels; i++) {
         levels[i] = contourVarMin + i * (contourVarMax - contourVarMin) / nLevels;
      }

      for(i=0; i<nLevels; i++) {
         value = 0.5 * (levels[i] + levels[i+1]);
                        colorIndices[i] = int (255 * (value - levels[0]) / (levels[nLevels] - levels[0]));  // colorMap index for the value that corresponds to  0.5*(levels[i] + levels[i+1])
      }

      for (j = 0; j < nY-1; j++ ) {
         for (i = 0; i < nX-1; i++ ) {
            c1 = j * nX + i;
            c2 = c1 + 1;
            c3 = c2 + nX;
            c4 = c1 + nX;
            glBegin(GL_POLYGON);
               value1 = result[i][j];		// Value of the contour variable at the bottom left corner of the cell.
               value2 = result[i+1][j];	// Value of the contour variable at the bottom right corner of the cell.
               value3 = result[i+1][j+1];	// Value of the contour variable at the top right corner of the cell.
               value4 = result[i][j+1];	// Value of the contour variable at the top left corner of the cell.
               valueAve = 0.25*(value1 + value2 + value3 + value4);

               n = findContourInterval(valueAve, nLevels, levels, -1);	// n=5 means value is between levels[5] and levels[6]
               if (n <	0) n = 0;		  // Cuneyt: Why is this and the following line are necessary ?
               if (n > nLevels-1) n= nLevels-1;
               k = colorIndices[n];

               red = colorMap[k][0];
               green = colorMap[k][1];
               blue = colorMap[k][2];
               glColor3f(red/255.0, green/255.0, blue/255.0);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c1], problem->mesh->blocks[b].coordinates[2*c1+1]);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c2], problem->mesh->blocks[b].coordinates[2*c2+1]);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c3], problem->mesh->blocks[b].coordinates[2*c3+1]);
               glVertex2f(problem->mesh->blocks[b].coordinates[2*c4], problem->mesh->blocks[b].coordinates[2*c4+1]);
            glEnd();
         }
      }
   }
   //drawBlockedCells(Qt::red);
	glFlush ();
}  // End of function drawAverageBWCells()




void GLWidget::drawColoredCells(void)
{
	// Cuneyt: No multi-block support.

	int b, i, j, k, n, c1, c2, c3, c4, nX, nY, red, green, blue, nLevels, *colorIndices;
	double contourVarMin, contourVarMax, value, *levels;
	double ** result;

   if (! problem->mesh->getIsMeshGenerated()) return;

	nLevels = problem->nContourLevels;
	levels = new double[nLevels+1];
	colorIndices = new int[nLevels];

	if (problem->contourVar == 0)      result = problem->uResult;
	else if (problem->contourVar == 1) result = problem->vResult;
   else                               result = problem->pResult;

	glPolygonMode(GL_FRONT, GL_FILL);

	// According to the Rainbow coloring scheme (Blue-Cyan-Green-Yellow-Red)
	int colorMap[1021][3];
	// Red
	for(i=0; i<=510; i++)    colorMap[i][0] = 0;
	for(i=510; i<=765; i++)  colorMap[i][0] = i-510;
	for(i=765; i<=1020; i++) colorMap[i][0] = 255;
	// Green
	for(i=0; i<=255; i++)    colorMap[i][1] = i;
	for(i=255; i<=765; i++)  colorMap[i][1] = 255;
	for(i=765; i<=1020; i++) colorMap[i][1] = 1020-i;
	// Blue
	for(i=0; i<=255; i++)    colorMap[i][2] = 255;
	for(i=255; i<=510; i++)  colorMap[i][2] = 510-i;
	for(i=510; i<=1020; i++) colorMap[i][2] = 0;

	for (b = 0; b < problem->mesh->getnBlocks(); b++ ) {
		nX = problem->mesh->blocks[b].getnXpoints();
		nY = problem->mesh->blocks[b].getnYpoints();

		// Calculate min and max values of the the contour variable.
		contourVarMin = result[0][0];
		contourVarMax = result[0][0];
		for(i=0; i<nX; i++) {
			for(j=0; j<nY; j++) {
            if (result[i][j] < contourVarMin) contourVarMin = result[i][j];
            if (result[i][j] > contourVarMax) contourVarMax = result[i][j];
			}
		}

		for(i=0; i<=nLevels; i++) {
			levels[i] = contourVarMin + i * (contourVarMax - contourVarMin) / nLevels;
		}

		for(i=0; i<nLevels; i++) {
			value = 0.5 * (levels[i] + levels[i+1]);
                        colorIndices[i] = int (1020 * (value - levels[0]) / (levels[nLevels] - levels[0]));  // colorMap index for the value that corresponds to  0.5*(levels[i] + levels[i+1])
		}

		for (j = 0; j < nY-1; j++ ) {
			for (i = 0; i < nX-1; i++ ) {
				c1 = j * nX + i;
				c2 = c1 + 1;
				c3 = c2 + nX;
				c4 = c1 + nX;

				glBegin(GL_POLYGON);
					value = result[i][j];		// Value of the contour variable at the bottom left corner of the cell.
					n = findContourInterval(value, nLevels, levels, -1);	// n=5 means value is between levels[5] and levels[6]
					if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?
					if (n > nLevels-1) n= nLevels-1;
					k = colorIndices[n];
					red = colorMap[k][0];
					green = colorMap[k][1];
					blue = colorMap[k][2];
					glColor3f(red/255.0, green/255.0, blue/255.0);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c1], problem->mesh->blocks[b].coordinates[2*c1+1]);

					value = result[i+1][j];		// Value of the contour variable at the bottom right corner of the cell.
					n = findContourInterval(value, nLevels, levels, n);
					if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?
					if (n > nLevels-1) n= nLevels-1;
					k = colorIndices[n];
					red = colorMap[k][0];
					green = colorMap[k][1];
					blue = colorMap[k][2];
					glColor3f(red/255.0, green/255.0, blue/255.0);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c2], problem->mesh->blocks[b].coordinates[2*c2+1]);

					value = result[i+1][j+1];  // Value of the contour variable at the top right corner of the cell.
					n = findContourInterval(value, nLevels, levels, n);
					if (n <	0) n = 0;         // Cuneyt: Why is this and the following line are necessary ?
					if (n > nLevels-1) n= nLevels-1;
					k = colorIndices[n];
					red = colorMap[k][0];
					green = colorMap[k][1];
					blue = colorMap[k][2];
					glColor3f(red/255.0, green/255.0, blue/255.0);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c3], problem->mesh->blocks[b].coordinates[2*c3+1]);

					value = result[i][j+1];		// Value of the contour variable at the top left corner of the cell.
					n = findContourInterval(value, nLevels, levels, n);
					if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?
					if (n > nLevels-1) n= nLevels-1;
					k = colorIndices[n];
					red = colorMap[k][0];
					green = colorMap[k][1];
					blue = colorMap[k][2];
					glColor3f(red/255.0, green/255.0, blue/255.0);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c4], problem->mesh->blocks[b].coordinates[2*c4+1]);
				glEnd();
			}
		}
	}
   //drawBlockedCells(Qt::black);
	glFlush ();
}  // End of function drawColoredCells()





void GLWidget::drawAverageColoredCells(void)
{
	// cuneyt: Bu fonksiyonun multi-block destegi yok.

	int b, i, j, k, n, c1, c2, c3, c4, nX, nY, red, green, blue, nLevels, *colorIndices;;
	double contourVarMin, contourVarMax, value, value1, value2, value3, value4, valueAve, *levels;
	double ** result;

   if (! problem->mesh->getIsMeshGenerated()) return;

	nLevels = problem->nContourLevels;
	levels = new double[nLevels+1];
	colorIndices = new int[nLevels];

	if (problem->contourVar == 0)      result = problem->uResult;
	else if (problem->contourVar == 1) result = problem->vResult;
   else                               result = problem->pResult;

	glPolygonMode(GL_FRONT, GL_FILL);

	// According to the Rainbow coloring scheme (Blue-Cyan-Green-Yellow-Red)
	int colorMap[1021][3];
	// Red
	for(i=0; i<=510; i++)    colorMap[i][0] = 0;
	for(i=510; i<=765; i++)  colorMap[i][0] = i-510;
	for(i=765; i<=1020; i++) colorMap[i][0] = 255;
	// Green
	for(i=0; i<=255; i++)    colorMap[i][1] = i;
	for(i=255; i<=765; i++)  colorMap[i][1] = 255;
	for(i=765; i<=1020; i++) colorMap[i][1] = 1020-i;
	// Blue
	for(i=0; i<=255; i++)    colorMap[i][2] = 255;
	for(i=255; i<=510; i++)  colorMap[i][2] = 510-i;
	for(i=510; i<=1020; i++) colorMap[i][2] = 0;

	for (b = 0; b < problem->mesh->getnBlocks(); b++ ) {
		nX = problem->mesh->blocks[b].getnXpoints();
		nY = problem->mesh->blocks[b].getnYpoints();

		// Calculate min and max values of the the contour variable.
		contourVarMin = result[0][0];
		contourVarMax = result[0][0];
		for(i=0; i<nX; i++) {
			for(j=0; j<nY; j++) {
            if (result[i][j] < contourVarMin) contourVarMin = result[i][j];
            if (result[i][j] > contourVarMax) contourVarMax = result[i][j];
			}
		}

		for(i=0; i<=nLevels; i++) {
			levels[i] = contourVarMin + i * (contourVarMax - contourVarMin) / nLevels;
		}

		for(i=0; i<nLevels; i++) {
			value = 0.5 * (levels[i] + levels[i+1]);
                        colorIndices[i] = int (1020 * (value - levels[0]) / (levels[nLevels] - levels[0]));  // colorMap index for the value that corresponds to  0.5*(levels[i] + levels[i+1])
		}

		for (j = 0; j < nY-1; j++ ) {
			for (i = 0; i < nX-1; i++ ) {
				c1 = j * nX + i;
				c2 = c1 + 1;
				c3 = c2 + nX;
				c4 = c1 + nX;
				glBegin(GL_POLYGON);
					value1 = result[i][j];		// Value of the contour variable at the bottom left corner of the cell.
					value2 = result[i+1][j];	// Value of the contour variable at the bottom right corner of the cell.
					value3 = result[i+1][j+1];	// Value of the contour variable at the top right corner of the cell.
					value4 = result[i][j+1];	// Value of the contour variable at the top left corner of the cell.
					valueAve = 0.25*(value1 + value2 + value3 + value4);

					n = findContourInterval(valueAve, nLevels, levels, -1);	// n=5 means value is between levels[5] and levels[6]
					if (n <	0) n = 0;		   // Cuneyt: Why is this and the following line are necessary ?
					if (n > nLevels-1) n= nLevels-1;
					k = colorIndices[n];

					red = colorMap[k][0];
					green = colorMap[k][1];
					blue = colorMap[k][2];
					glColor3f(red/255.0, green/255.0, blue/255.0);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c1], problem->mesh->blocks[b].coordinates[2*c1+1]);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c2], problem->mesh->blocks[b].coordinates[2*c2+1]);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c3], problem->mesh->blocks[b].coordinates[2*c3+1]);
					glVertex2f(problem->mesh->blocks[b].coordinates[2*c4], problem->mesh->blocks[b].coordinates[2*c4+1]);
				glEnd();
			}
		}
	}
   //drawBlockedCells(Qt::black);
	glFlush ();
}  // End of function drawAverageColoredCells()




int GLWidget::findContourInterval(double value, int nLevels, double* &levels, int possible)
{
	int i, low, high, mid;
	// Returns the contour interval that encloses the value "value"
	// n=5 means value is between levels[5] and levels[6]

	// Check if "possible" is the required interval or not
	// This is just to speed up the search.
	if (possible != -1)
      if (value >= levels[possible] && value <= levels[possible+1])
			return possible;


	// Find the interval by dividing the whole range into 2 (Bisection method)
	low = 0;
	high = nLevels;
	for(i=0; ; i++) {
		mid = (low + high)/2;

      if (levels[mid] > value)		// value is between levels[low] and levels[mid]
			high = mid;
		else						// value is between levels[mid] and levels[high]
			low = mid;
		
		if (high-low == 1)	// Interval is found
			break;
	}
	return low;

}  // End of function findContourInterval()




void GLWidget::drawStreamline()
{
	int i, j;
   if (problem->nStreamlines == 0) return;

   if (! problem->mesh->getIsMeshGenerated()) return;

	qglColor(Qt::black);
	glLineWidth(1.0);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	for(i=0; i<problem->nStreamlines; i++) {
		glBegin(GL_LINE_STRIP);
			for (j=0; j<problem->streamlines[i].nPoints; j++)
				glVertex2f (problem->streamlines[i].coor[j][0], problem->streamlines[i].coor[j][1]);
		glEnd();
	}

	//glDisable (GL_LINE_SMOOTH);
}




void GLWidget::resizeEvent(QResizeEvent *event)
{
   wPrev = event->oldSize().width();
   hPrev = event->oldSize().height();
}




void GLWidget::resizeGL( int w, int h )
{
   double xRange, yRange, xRangeNew, yRangeNew;

   if (minX * maxX > 0)
     xRange = fabs( fabs(maxX) - fabs(minX) );
   else
     xRange = fabs( fabs(maxX) + fabs(minX) );
   if (minY * maxY > 0)
     yRange = fabs( fabs(maxY) - fabs(minY) );
   else
     yRange = fabs( fabs(maxY) + fabs(minY) );

   // double xRatio = xRange / double(wPrev);
   // double yRatio = yRange / double(hPrev);
   // double whRatio = double(wPrev) / double(hPrev);

   double midX = minX + 0.5 * xRange;
   double midY = minY + 0.5 * yRange;

   xRangeNew = xRange * double(w) / double(wPrev);
   yRangeNew = yRange * double(h) / double(hPrev);

   minX = midX - 0.5 * xRangeNew;
   maxX = midX + 0.5 * xRangeNew;
   minY = midY - 0.5 * yRangeNew;
   maxY = midY + 0.5 * yRangeNew;

   /*
   if (xRatio > yRatio) {
      // x axis will fit tight and y axis will have spaces at the top and bottom
      double newXrange = xRange * (double)wPrev / double(wPrev);
      minX = midX - 0.5 * newXrange;
      maxX = midX + 0.5 * newXrange;
      double newYrange = newXrange / whRatio;
      minY = midY - 0.5 * newYrange;
      maxY = midY + 0.5 * newYrange;
   } else {
      // y axis will fit tight and x axis will have spaces at the top and bottom
      double newYrange = yRange * double(hPrev) / double(hPrev);
      minY = midY - 0.5 * newYrange;
      maxY = midY + 0.5 * newYrange;
      double newXrange = newYrange * whRatio;
      minX = midX - 0.5 * newXrange;
      maxX = midX + 0.5 * newXrange;
   }
   */

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   //glOrtho( minXprev, maxXprev, minYprev, maxYprev, -1.0, 1.0 );   // cuneyt: yoksa burada glfrustrum() mi kullanilacak?
   glOrtho( minX, maxX, minY, maxY, -1.0, 1.0 );   // cuneyt: yoksa burada glfrustrum() mi kullanilacak?
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glViewport( 0, 0, (GLint)w, (GLint)h );

   paintGL();

   minXprev = minX;
   maxXprev = maxX;
   minYprev = minY;
   maxYprev = maxY;

   QString s1 = s1.number(minX);
   QString s2 = s2.number(maxX);
   QString s3 = s3.number(minY);
   QString s4 = s4.number(maxY);

}  // End of resizeGL()



/*
void GLWidget::resizeGL( int w, int h )
{

   double xRange, yRange;

   //showAll();  // Just having this line in this function works,
	           // but it auto scales every time the program window is resized, which is not good.

   //int w = this->width();
   //int h = this->height();

   if (minX * maxX > 0)
     xRange = fabs( fabs(maxX) - fabs(minX) );
   else
     xRange = fabs( fabs(maxX) + fabs(minX) );
   if (minY * maxY > 0)
     yRange = fabs( fabs(maxY) - fabs(minY) );
   else
     yRange = fabs( fabs(maxY) + fabs(minY) );

   double xRatio = xRange / double(w);   // 30 point space at the top/bottom/left/right of the window.
   double yRatio = yRange / double(h);
   double whRatio = double(w) / double(h);

   double midX = minX + xRange / 2.0;
   double midY = minY + yRange / 2.0;

   if (xRatio > yRatio) {
      // x axis will fit tight and y axis will have spaces at the top and bottom
      double newXrange = xRange * (double)w / double(w);
      minX = midX - 0.5 * newXrange;
      maxX = midX + 0.5 * newXrange;
      double newYrange = newXrange / whRatio;
      minY = midY - 0.5 * newYrange;
      maxY = midY + 0.5 * newYrange;
   } else {
      // y axis will fit tight and x axis will have spaces at the top and bottom
      double newYrange = yRange * (double)h / double(h);
      minY = midY - 0.5 * newYrange;
      maxY = midY + 0.5 * newYrange;
      double newXrange = newYrange * whRatio;
      minX = midX - 0.5 * newXrange;
      maxX = midX + 0.5 * newXrange;
   }

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   //glOrtho( minXprev, maxXprev, minYprev, maxYprev, -1.0, 1.0 );   // cuneyt: yoksa burada glfrustrum() mi kullanilacak?
   glOrtho( minX, maxX, minY, maxY, -1.0, 1.0 );   // cuneyt: yoksa burada glfrustrum() mi kullanilacak?
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glViewport( 0, 0, (GLint)w, (GLint)h );

   paintGL();

}  // End of resizeGL()
*/



void GLWidget::calculateMinMaxXY( int w, int h )
{

   // This function is not used anymore.

   double xRange = maxX - minX;
   double yRange = maxY - minY;

   double xyRatio = xRange / yRange;
   double whRatio = double(w) / double(h);

   double midX = minX + xRange / 2.0;
   double midY = minY + yRange / 2.0;

   // Cuneyt: The following is for the case where xXtoYratio = 1

   // Cuneyt: With the code given below drawings always get smaller when windows size is changed.

   // Make adjustments to minX, maxX, minY, maxY values so that xyRatio = whRatio
   if (xyRatio > whRatio) {
      // increase the range for y
      double newYrange = xRange / whRatio;
      minY = midY - 0.5 * newYrange;
      maxY = midY + 0.5 * newYrange;
   } else {
      // increase the range for x
      double newXrange = yRange * whRatio;
      minX = midX - 0.5 * newXrange;
      maxX = midX + 0.5 * newXrange;
   }
}  // End of function calculateMinMaxXY()




void GLWidget::mousePressEvent(QMouseEvent *event)
{
	int m, p;
	mainStates mState = problem->getMainState();
	glMouseStates glMState = problem->getGlMouseState();
	visualizeStates visState = problem->getVisualizeState();
	primitiveTypes drawingWhat = problem->getDrawingWhat();

   //  This is for continuos zooming in/out using the mid mouse button
   if (event->button() == Qt::MidButton) {
      click1Pos = event->pos();
      clicked1 = TRUE;
      isContinuousZooming = TRUE;
   }


   // This is for zooming in/out using a rubberband zoom rectangle
   if (glMState == ZOOM && event->button() == Qt::LeftButton) {
		click1Pos = event->pos();
		clicked1 = TRUE;

      // qrubberband
      // rubberBand = new QRubberBand(QRubberBand::Line, this);
      // rubberBand->setGeometry(QRect(click1Pos, QSize()));
      // rubberBand->show();
      // qrubberband
	}


   // This is for panning around using the right mouse button
   if (event->button() == Qt::RightButton ) {
      click1Pos = event->pos();
      isPanning = TRUE;
      clicked1 = TRUE;
      minXprev = minX;
      maxXprev = maxX;
      minYprev = minY;
      maxYprev = maxY;
   }


	// The following deletes the primitive under the mouse when left mouse button is clicked.
	if (mState == GEOM && drawingWhat == DEL && glMState == SELECTION && event->button() == Qt::LeftButton ) {
		if (problem->mesh->getnPrimitives() > 0) {
			p = primitiveAtPosition(event->pos());
			if (p > -1) {  // if primitive is -1 it means user did not click on a primitive.
				emit deletePrimitive(p);
				updateGL();
			}
//			if (p > -1) {
//				QString s = s.number(p);
//				emit appendOutput("Primitive " + s + " is deleted.");
//			}
		}
	}

	//  The following selects the primitive under the mouse to specify block & face properties.
   if (mState == MESH && glMState == SELECTION && event->button() == Qt::LeftButton ) {
		if (problem->mesh->getnPrimitives() > 0) {
			p = primitiveAtPosition(event->pos());
			if (p > -1 && problem->mesh->getPrimitiveUnderMouse() != p) {
				if (problem->mesh->getPrimitiveUnderMouse() != -1)
					problem->mesh->primitives[problem->mesh->getPrimitiveUnderMouse()].setColor(Qt::black);
				problem->mesh->primitives[p].setColor(Qt::red);
				problem->mesh->setPrimitiveUnderMouse(p);
			} else if (p == -1 && problem->mesh->getPrimitiveUnderMouse() != -1) {
				problem->mesh->primitives[problem->mesh->getPrimitiveUnderMouse()].setColor(Qt::black);
				problem->mesh->setPrimitiveUnderMouse(p);
			}
			updateGL();
			emit showPrimitivesBlockProperties(p);
		}
	}

	// The following selects the primitive under the mouse to specify boundary conditions.
	if (mState == BCIC && event->button() == Qt::LeftButton ) {
		if (problem->mesh->getnPrimitives() > 0) {
			p = primitiveAtPosition(event->pos());
			if (p > -1 && problem->mesh->getPrimitiveUnderMouse() != p) {
				if (problem->mesh->getPrimitiveUnderMouse() != -1)
					problem->mesh->primitives[problem->mesh->getPrimitiveUnderMouse()].setColor(Qt::black);
				problem->mesh->primitives[p].setColor(Qt::red);
				problem->mesh->setPrimitiveUnderMouse(p);
			} else if (p == -1 && problem->mesh->getPrimitiveUnderMouse() != -1) {
				problem->mesh->primitives[problem->mesh->getPrimitiveUnderMouse()].setColor(Qt::black);
				problem->mesh->setPrimitiveUnderMouse(p);
			}
			updateGL();
			emit showBC(p);
		}
	}

	// The following selects the mesh point under the mouse to mark it as a control point.
	// Cuneyt: No-multi block support. Works only for the 0th block.
	if (mState == CONTROLPOINTS && glMState == SELECTION && event->button() == Qt::LeftButton ) {
		if (problem->mesh->getIsMeshGenerated()) {
			m = meshPointAtPosition(event->pos());
			if (m > -1) {
				if (problem->mesh->blocks[0].addRemoveControlPoint(m) == 2)  // 2 means that no more control points can be added.
               emit appendOutput(tr("WARNING: You already selected 10 control points. No more control points can be added."), Qt::red);
			}
		}
	}

	// The following selects the mesh cell under the mouse to mark it as a blocked cell.
	// Cuneyt: No-multi block support. Works only for the 0th block.
	if (mState == BLOCKEDCELLS && glMState == SELECTION && event->button() == Qt::LeftButton ) {
		if (problem->mesh->getIsMeshGenerated()) {
			m = meshCellAtPosition(event->pos());
			if (m > -1) problem->mesh->blocks[0].addBlockedCell(m);
			updateGL();
		}
	}
	// The following selects the mesh cell under the mouse to mark it as a non-blocked cell.
	// Cuneyt: No-multi block support. Works only for the 0th block.
	if (mState == BLOCKEDCELLS && glMState == SELECTION && event->button() == Qt::RightButton ) {
		if (problem->mesh->getIsMeshGenerated()) {
			m = meshCellAtPosition(event->pos());
			if (m > -1) problem->mesh->blocks[0].removeBlockedCell(m);
			updateGL();
		}
	}

	// The following is for probing during post processing.
	if (mState == VISUALIZE && visState == PROBE && glMState == SELECTION && event->button() == Qt::LeftButton ) {
		if (problem->mesh->getIsMeshGenerated()) {	// cuneyt: Bu degismeli. isDatRead gibi bir degisken kullanilmali.
			float x = event->pos().x() * (maxX-minX) / (1.0*this->width()) + minX;
			float y = event->pos().y() * (minY-maxY) / (1.0*this->height()) + maxY;
			emit probe(x, y);
		}
	}

	// The following is for placing a streamline during post processing.
	if (mState == VISUALIZE && visState == ADDSTREAMLINE && glMState == SELECTION && event->button() == Qt::LeftButton ) {
      if (problem->getResultFileName() != "") {
			float x = event->pos().x() * (maxX-minX) / (1.0*this->width()) + minX;
			float y = event->pos().y() * (minY-maxY) / (1.0*this->height()) + maxY;
			emit addStreamline(x, y);
			drawStreamline();
		}
	}

	updateGL();
}  // End of function mousePressEvent()



void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int p, m;
	mainStates mState = problem->getMainState();
	glMouseStates glMState = problem->getGlMouseState();
	primitiveTypes drawingWhat = problem->getDrawingWhat();

	// Emit the signal that shows the x, y position of the mouse.
	float x = event->pos().x() * (maxX-minX) / (1.0*this->width()) + minX;
	float y = event->pos().y() * (minY-maxY) / (1.0*this->height()) + maxY;
	emit GLWidget::xyChanged(x, y);


   //  This is for continuos zooming in/out using the mid mouse button
   if (isContinuousZooming) {
      if ((event->pos().y() - click1Pos.y()) > 1) {
         this->zoomIn();
         click1Pos = event->pos();
      } else if ((event->pos().y() - click1Pos.y()) < 1) {
         this->zoomOut();
         click1Pos = event->pos();
      }
   }

   // This is for zooming in/out using a rubberband zoom rectangle
   if(clicked1 && glMState == ZOOM) {
      isRubberBandZooming = TRUE;
      // Draw zoom rectangle
      zoomRectangleX0 = minX + click1Pos.x()*(maxX-minX)/(1.0*this->width());   // cuneyt: bunu tekrar edip durma
      zoomRectangleY0 = maxY - click1Pos.y()*(maxY-minY)/(1.0*this->height());  // cuneyt: bunu tekrar edip durma
      zoomRectangleX1 = minX + event->pos().x()*(maxX-minX)/(1.0*this->width());
      zoomRectangleY1 = maxY - event->pos().y()*(maxY-minY)/(1.0*this->height());
      updateGL();


      // qrubberband
      // rubberBand->setGeometry(QRect(click1Pos, event->pos())); //.normalized());
      // qrubberband
	}


   // This is for panning around using the right mouse button
   if (isPanning) {
      pan(event->pos());
   }


	//  The following draws the primitive under the mouse with a different color when deleting a primitive.
	if (mState == GEOM && drawingWhat == DEL && glMState == SELECTION) {
		if (problem->mesh->getnPrimitives() > 0) {
			p = primitiveAtPosition(event->pos());
			// QString s = s.number(p);   emit appendOutput("Primitive under the mouse: " + s);
			if (p > -1 && problem->mesh->getPrimitiveUnderMouse() != p) {  // Cursor is on a primitive
				if (problem->mesh->getPrimitiveUnderMouse() != -1)
					problem->mesh->primitives[problem->mesh->getPrimitiveUnderMouse()].setColor(Qt::black);
				problem->mesh->primitives[p].setColor(Qt::red);
				problem->mesh->setPrimitiveUnderMouse(p);
         } else if (p == -1 && problem->mesh->getPrimitiveUnderMouse() != -1) {  // Cursor is NOT on a primitive.
				problem->mesh->primitives[problem->mesh->getPrimitiveUnderMouse()].setColor(Qt::black);
				problem->mesh->setPrimitiveUnderMouse(-1);
			}
			updateGL();
		}
	}

	// The following selects the mesh cell under the mouse to mark it as a blocked cell.
	// Cuneyt: No-multi block support. Works only for the 0th block.
	if (mState == BLOCKEDCELLS && glMState == SELECTION && event->buttons() == Qt::LeftButton ) {
		if (problem->mesh->getIsMeshGenerated()) {
			m = meshCellAtPosition(event->pos());
			if (m > -1) problem->mesh->blocks[0].addBlockedCell(m);
			updateGL();
		}
	}
	// The following selects the mesh cell under the mouse to mark it as a non-blocked cell.
	// Cuneyt: No-multi block support. Works only for the 0th block.
	if (mState == BLOCKEDCELLS && glMState == SELECTION && event->buttons() == Qt::RightButton ) {
		if (problem->mesh->getIsMeshGenerated()) {
			m = meshCellAtPosition(event->pos());
			if (m > -1) problem->mesh->blocks[0].removeBlockedCell(m);
			updateGL();
		}
	}

}  // End of mouseMoveEvent()




void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	glMouseStates glMState = problem->getGlMouseState();

   // This is for panning around using the right mouse button
   if (isPanning) {
      clicked1 = FALSE;
      isPanning = FALSE;
   }


   // This is for continuous zooming in/out using the mid mouse button
   if (isContinuousZooming) {
      clicked1 = FALSE;
      isContinuousZooming = FALSE;
   }

   // This is for zooming in/out using a rubberband zoom rectangle
   if (glMState == ZOOM && event->button() == Qt::LeftButton) {
      clicked1 = FALSE;
      isRubberBandZooming = FALSE;
      zoomIntoRectangle();
      //problem->setGlMouseState(SELECTION);
      emit releaseZoomButton();
   }

}  // End of mouseReleaseEvent




int GLWidget::primitiveAtPosition(const QPoint &pos)
{
  const int MaxSize = 512;
  GLuint buffer[MaxSize];
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glSelectBuffer(MaxSize, buffer);
  glRenderMode(GL_SELECT);

  glInitNames();
  glPushName(0);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix((GLdouble)pos.x(),
                (GLdouble)(viewport[3] - pos.y()),
                8.0, 8.0, viewport);
  glOrtho( minX, maxX, minY, maxY, -1.0, 1.0 );			// cuneyt : orijinalinde bu glFrustum idi.
  drawPrimitives();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  if (!glRenderMode(GL_RENDER))
    return -1;
  return buffer[3];
}  // End of function primitiveAtPosition()




int GLWidget::meshPointAtPosition(const QPoint &pos)
{
  const int MaxSize = 512;
  GLuint buffer[MaxSize];
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glSelectBuffer(MaxSize, buffer);
  glRenderMode(GL_SELECT);

  glInitNames();
  glPushName(0);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix((GLdouble)pos.x(),
                (GLdouble)(viewport[3] - pos.y()),
                6.0, 6.0, viewport);
  glOrtho( minX, maxX, minY, maxY, -1.0, 1.0 );			// cuneyt : orijinalinde bu glFrustum idi.
  drawMeshPoints();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  if (!glRenderMode(GL_RENDER))
    return -1;
  return buffer[3];
}  // End of function meshPointAtPosition()




int GLWidget::meshCellAtPosition(const QPoint &pos)
{
  const int MaxSize = 512;
  GLuint buffer[MaxSize];
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glSelectBuffer(MaxSize, buffer);
  glRenderMode(GL_SELECT);

  glInitNames();
  glPushName(0);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix((GLdouble)pos.x(),
                (GLdouble)(viewport[3] - pos.y()),
                2.0, 2.0, viewport);
  glOrtho( minX, maxX, minY, maxY, -1.0, 1.0 );			// cuneyt : orijinalinde bu glFrustum idi.
  drawStructuredMesh();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  if (!glRenderMode(GL_RENDER))
    return -1;
  return buffer[3];
}  // End of meshCellAtPosition




void GLWidget::setStateToZoom (bool flag)
{
	if (flag) {
      this->setCursor( Qt::CrossCursor );
		problem->setGlMouseState(ZOOM);
		clicked1 = FALSE;
   } else {
      this->setCursor( Qt::ArrowCursor );
      problem->setGlMouseState(SELECTION);
   }
}




void GLWidget::showAll(void)
{
   double xRange, yRange;

   minXprev = minX;
   maxXprev = maxX;
   minYprev = minY;
   maxYprev = maxY;

   int w = this->width();
   int h = this->height();
    
   if (problem->mesh->getMinXused() * problem->mesh->getMaxXused() > 0)
     xRange = fabs( fabs(problem->mesh->getMaxXused()) - fabs(problem->mesh->getMinXused()) );
   else
     xRange = fabs( fabs(problem->mesh->getMaxXused()) + fabs(problem->mesh->getMinXused()) );
   if (problem->mesh->getMinYused() * problem->mesh->getMaxYused() > 0)
     yRange = fabs( fabs(problem->mesh->getMaxYused()) - fabs(problem->mesh->getMinYused()) );
   else
     yRange = fabs( fabs(problem->mesh->getMaxYused()) + fabs(problem->mesh->getMinYused()) );

   double xRatio = xRange / double(w - 10);   // 15 point space at the top/bottom/left/right of the window.
   double yRatio = yRange / double(h - 10);
   double whRatio = double(w - 10) / double(h - 10);

   double midX = problem->mesh->getMinXused() + xRange / 2.0;
   double midY = problem->mesh->getMinYused() + yRange / 2.0;

   if (xRatio > yRatio) {
      // x axis will fit tight and y axis will have spaces at the top and bottom
      double newXrange = xRange * (double)w / double(w - 10);
      minX = midX - 0.5 * newXrange;
      maxX = midX + 0.5 * newXrange;
      double newYrange = newXrange / whRatio;
      minY = midY - 0.5 * newYrange;
      maxY = midY + 0.5 * newYrange;
   } else {
      // y axis will fit tight and x axis will have spaces at the top and bottom
      double newYrange = yRange * (double)h / double(h - 10);
      minY = midY - 0.5 * newYrange;
      maxY = midY + 0.5 * newYrange;
      double newXrange = newYrange * whRatio;
      minX = midX - 0.5 * newXrange;
      maxX = midX + 0.5 * newXrange;
   }

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( minX, maxX, minY, maxY, -1.0, 1.0 );   // Cuneyt: Should I use glfrustrum() here ?
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glViewport( 0, 0, (GLint)w, (GLint)h );

   updateGL();

}  // End of function showAll()




void GLWidget::showPreviousView(void)
{
   int w = this->width();
   int h = this->height();

   minX = minXprev;
   maxX = maxXprev;
   minY = minYprev;
   maxY = maxYprev;

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( minX, maxX, minY, maxY, -1.0, 1.0 );   // Cuneyt: Should I use glfrustrum() here ?
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glViewport( 0, 0, (GLint)w, (GLint)h );

   updateGL();
}




void GLWidget::redraw (void)
{
   updateGL();
}




void GLWidget::drawZoomRectangle()
{
   qglColor (Qt::magenta);
   glLineWidth (2.0);

   glBegin(GL_LINE_LOOP);
		glVertex2f(zoomRectangleX0, zoomRectangleY0);
		glVertex2f(zoomRectangleX0, zoomRectangleY1);
		glVertex2f(zoomRectangleX1, zoomRectangleY1);
		glVertex2f(zoomRectangleX1, zoomRectangleY0);
	glEnd();

   //glFlush();
}




void GLWidget::zoomIntoRectangle()
{
	float xMid, yMid, necessaryZoomRectangleHeight, necessaryZoomRectangleWidth;
	float zoomRectangleHeight = fabs(zoomRectangleY1 - zoomRectangleY0);
	float zoomRectangleWidth = fabs(zoomRectangleX1 - zoomRectangleX0);

   minXprev = minX;
   maxXprev = maxX;
   minYprev = minY;
   maxYprev = maxY;

	float aspectRatio = (maxY-minY)/(maxX-minX);  // This aspect ratio must be kept the same after the zoom also.
	float aspectRatio2 = zoomRectangleHeight / zoomRectangleWidth;  // This is the aspect ratio of the drawn zoom rectangle.

	if (aspectRatio2 > aspectRatio)	{
		maxY = max(zoomRectangleY0, zoomRectangleY1);
		minY = min(zoomRectangleY0, zoomRectangleY1);

		xMid = min(zoomRectangleX1, zoomRectangleX0) + 0.5 * zoomRectangleWidth;   // x coordinate of the mid of the zoom rectangle.
		necessaryZoomRectangleWidth = zoomRectangleHeight / aspectRatio;
		minX = xMid - 0.5 * necessaryZoomRectangleWidth;
		maxX = xMid + 0.5 * necessaryZoomRectangleWidth;

	} else {
		maxX = max(zoomRectangleX0, zoomRectangleX1);
		minX = min(zoomRectangleX0, zoomRectangleX1);

		yMid = min(zoomRectangleY1, zoomRectangleY0) + 0.5 * zoomRectangleHeight;   // x coordinate of the mid of the zoom rectangle.
		necessaryZoomRectangleHeight = zoomRectangleWidth * aspectRatio;
		minY = yMid - 0.5 * necessaryZoomRectangleHeight;
		maxY = yMid + 0.5 * necessaryZoomRectangleHeight;
	}

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, -1.0, 1.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glViewport( 0, 0, (GLint)this->width(), (GLint)this->height() );

	updateGL();
}  // End of function zoomIntoRectangle()




void GLWidget::zoomIn()
{
   zoom(1);
}




void GLWidget::zoomOut()
{
   zoom(-1);
}




void GLWidget::zoom(int inOrOut)  // Zoom in/out by a fixed ratio. Working.
{
	int w, h;
	double xRange, yRange;

   minXprev = minX;
   maxXprev = maxX;
   minYprev = minY;
   maxYprev = maxY;

   float zoomFactor = float(0.05);
	
	w = this->width();
	h = this->height();
	
   if (minX * maxX > 0)
		xRange = fabs( fabs(maxX) - fabs(minX) );
	else
		xRange = fabs( fabs(maxX) + fabs(minX) );

   if (minY * maxY > 0)
		yRange = fabs( fabs(maxY) - fabs(minY) );
	else
		yRange = fabs( fabs(maxY) + fabs(minY) );
	
	// Now zoom in or out
   minX = minX + xRange * zoomFactor * inOrOut;
   maxX = maxX - xRange * zoomFactor * inOrOut;
   minY = minY + yRange * zoomFactor * inOrOut;
   maxY = maxY - yRange * zoomFactor * inOrOut;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, -1.0, 1.0);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glViewport( 0, 0, (GLint)w, (GLint)h );

	updateGL();
}  // End of function zoomIn()




void GLWidget::pan(const QPoint &pos)
{
   int w, h, dxInt, dyInt;
   double xRange, yRange, dx, dy;

   w = this->width();
   h = this->height();
   
   dxInt = pos.x() - click1Pos.x();
   dyInt = click1Pos.y() - pos.y();

   if (minX * maxX > 0)
     xRange = fabs( fabs(maxX) - fabs(minX) );
   else
     xRange = fabs( fabs(maxX) + fabs(minX) );

   if (minY * maxY > 0)
     yRange = fabs( fabs(maxY) - fabs(minY) );
   else
     yRange = fabs( fabs(maxY) + fabs(minY) );

   dx = dxInt * xRange / float(w);
   dy = dyInt * yRange / float(h);

   minX = minX - dx;
   maxX = maxX - dx;
   minY = minY - dy;
   maxY = maxY - dy;

   //glTranslatef(dx,dy,0.0);   // Cuneyt: This is sometimes problematic. Why ?

   // The following 6 lines are used as an alternative to the above glTranslatef().
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho(minX, maxX, minY, maxY, -1.0, 1.0);
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glViewport( 0, 0, (GLint)w, (GLint)h );

   updateGL();

   click1Pos = pos;
}  // End of function pan()

