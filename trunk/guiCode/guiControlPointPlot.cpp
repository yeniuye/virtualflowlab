// This implementation is based on renderarea example

#include <QtGui>
#include <math.h>
#include "mainWindow.h"
#include "guiControlPointPlot.h"
#include "guiTypedefs.h"
#include "guiProblem.h"

extern Problem *problem;




ControlPointPlot::ControlPointPlot(QWidget *parent) : QWidget(parent)
{
   //setBackgroundRole(QPalette::Light);	// This sets the background of the widget to white
   //setAutoFillBackground(TRUE);		   // This is necessary to make the previous line work properly

   whichVariable = 0;		// Plot u
   whichControlPoint = 0;	// Select first control point

   penWidth = 3;

   penColor[0] = Qt::blue;
   penColor[1] = Qt::red;
   penColor[2] = Qt::black;

   defaultMinX = 0;
   defaultMaxX = 500;
   defaultMinY = -10;
   defaultMaxY = 10;

   minX = defaultMinX;
   maxX = defaultMaxX;
   minY = defaultMinY;
   maxY = defaultMaxY;

   for (int i=0; i<MAX_CONTROL_POINTS; i++) {
      for (int j=0; j<3; j++) {
         minValues[i][j] = minY;
         maxValues[i][j] = maxY;
      }
   }

   emit setAxesLabels(defaultMinX, defaultMaxX, defaultMinY, defaultMaxY);

}  // End of function ControlPointPlot()




void ControlPointPlot::initialize(int maxIter)
{
   int i, nCP;

   nCP = MAX_CONTROL_POINTS;

   whichVariable = 0;	   // Plot u
   whichControlPoint = 0;	// Select first control point

   minX = defaultMinX;
   maxX = defaultMaxX;
   minY = defaultMinY;
   maxY = defaultMaxY;

   // Transformation between real data and screen pixels in the x direction (pixel = Ax + B)
   A = width()/(maxX - minX);
   B = - A * minX;
   
   // Transformation between real data and screen pixels in the y direction (pixel = Cy + D)
   C = - height()/(maxY - minY);
   D = - C * maxY;

   for (i=0; i<nCP; i++) {
      controlPointPaths[i][0] = emptyPath;	// Cuneyt: Will it be better to use delete-new ?
      controlPointPaths[i][1] = emptyPath;
      controlPointPaths[i][2] = emptyPath;

      controlPointPaths[i][0].moveTo(A * 0 + B, C * 0 + D);
      controlPointPaths[i][1].moveTo(A * 0 + B, C * 0 + D);
      controlPointPaths[i][2].moveTo(A * 0 + B, C * 0 + D);
   }

   emit setAxesLabels(minX, maxX, minY, maxY);

   update();
}  // End of function initialize()




void ControlPointPlot::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   //painter.setRenderHint(QPainter::Antialiasing);
   //drawLabels(painter);

   if(!problem->mesh->getIsMeshGenerated()) {
      return;
   }

   int nCP = problem->mesh->blocks[0].getnControlPoints();  // Number of control points defined for the problem.

   if (nCP == 0 || whichControlPoint >= nCP) {
      painter.fillRect(0,0,width(),height(), Qt::white);
   } else {
      painter.scale(width()/(maxX-minX), height()/(maxY-minY));	// Scales the plot properly.
      painter.translate(-minX, maxY);	// Translates the origin into the proper location.

      painter.setPen(QPen(penColor[whichVariable]));
      painter.drawPath(controlPointPaths[whichControlPoint][whichVariable]);
      
      update();
   }
}  // End of function paintEvent()




void ControlPointPlot::updateControlPoints(int iter)
{
   int i, nCP;
   double u, v, p;

   nCP = problem->mesh->blocks[0].getnControlPoints();

   if(nCP == 0)
      return;

   if (iter == 1) {
      for (i=0; i<nCP; i++) {
         u = problem->mesh->blocks[0].controlPointData[i][0];
         v = problem->mesh->blocks[0].controlPointData[i][1];
         p = problem->mesh->blocks[0].controlPointData[i][2];

         controlPointPaths[i][0].moveTo(iter, -u);  // Minus signs for y component are used because in widget coordinates +y axis points downwards.
         controlPointPaths[i][1].moveTo(iter, -v);
         controlPointPaths[i][2].moveTo(iter, -p);

         minValues[i][0] = u;   maxValues[i][0] = u;
         minValues[i][1] = v;   maxValues[i][1] = v;
         minValues[i][2] = p;   maxValues[i][2] = p;
      }
   } else {
      for (i=0; i<nCP; i++) {
         u = problem->mesh->blocks[0].controlPointData[i][0];
         v = problem->mesh->blocks[0].controlPointData[i][1];
         p = problem->mesh->blocks[0].controlPointData[i][2];

         controlPointPaths[i][0].lineTo(iter, -u);
         controlPointPaths[i][1].lineTo(iter, -v);
         controlPointPaths[i][2].lineTo(iter, -p);

         if (u < minValues[i][0]) {
            minValues[i][0] = u;
         } else if (u > maxValues[i][0]) {
            maxValues[i][0] = u;
         }

         if (v < minValues[i][1]) {
            minValues[i][1] = v;
         } else if (v > maxValues[i][1]) {
            maxValues[i][1] = v;
         }
         
         if (p < minValues[i][2]) {
            minValues[i][2] = p;
         } else if (p > maxValues[i][2]) {
            maxValues[i][2] = p;
         }
      }
   }
   autoAdjustXaxis(iter);
   autoAdjustYaxis();
}




void ControlPointPlot::resizeEvent(QResizeEvent *)
{
   // Transformation between real data and screen pixels in the x direction (pixel = Ax + B)
   A = width()/(maxX - minX);
   B = - A * minX;
   
   // Transformation between real data and screen pixels in the y direction (pixel = Cy + D)
   C = - height()/(maxY - minY);
   D = - C * maxY;

   update();
}  // End of function resizeEvent()




void ControlPointPlot::decreaseMinX()
{
   if (minX > 0 && minX <= 1000)
      minX = minX - 100;
   else if (minX > 1000)
      minX = minX - 1000;

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ControlPointPlot::increaseMinX()
{
   if (minX < 1000 && maxX-minX > 100)
      minX = minX + 100;
   else if (minX >= 1000 && maxX-minX > 1000)
      minX = minX + 1000;

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ControlPointPlot::decreaseMaxX()
{
   if (maxX <= 1000 && maxX-minX > 100)
      maxX = maxX - 100;
   else if (maxX > 1000 && maxX-minX > 1000)
      maxX = maxX - 1000;

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ControlPointPlot::increaseMaxX()
{
   if (maxX < 1000)
      maxX = maxX + 100;
   else if (maxX >= 1000)
      maxX = maxX + 1000;

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ControlPointPlot::decreaseMinY()
{
   if (minY >= -0.9 && minY <= 1)
        minY = minY - 0.1;
   else if (minY >= -9 && minY <= 10)
        minY = minY - 1;
   else if (minY >= -99 && minY <= 100)
      minY = minY - 10;
   else if (minY >= -999 && minY <= 1000)
      minY = minY - 100;
   else if (minY >= -9999 && minY <= 10000)
      minY = minY - 1000;
   else if (minY >= -99999 && minY <= 100000)
      minY = minY - 10000;

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ControlPointPlot::increaseMinY()
{
   double newMinY = minY;
   if (minY >= -1 && minY < 1)
        newMinY = minY + 0.1 ;
   else if (minY >= -10 && minY < 10)
        newMinY = minY + 1 ;
   else if (minY >= -100 && minY < 100)
      newMinY = minY + 10;
   else if (minY >= -1000 && minY < 1000)
      newMinY = minY + 100;
   else if (minY >= -10000 && minY < 10000)
      newMinY = minY + 1000;
   else if (minY >= -100000 && minY < 100000)
      newMinY = minY + 10000;

   if (newMinY < maxY) {
      minY = newMinY;
      emit setAxesLabels(minX, maxX, minY, maxY);
   }
}




void ControlPointPlot::decreaseMaxY()
{
   double newMaxY = maxY;
   //if(maxY > -1 && maxY <= 1)
    //    newMaxY = maxY - 0.1 ;
   //else
   if (maxY > -10 && maxY <= 10)
        newMaxY = maxY - 1 ;
   else if (maxY > -100 && maxY <= 100)
      newMaxY = maxY - 10;
   else if (maxY > -1000 && maxY <= 1000)
      newMaxY = maxY - 100;
   else if (maxY > -10000 && maxY <= 10000)
      newMaxY = maxY - 1000;
   else if (maxY > -100000 && maxY <= 100000)
      newMaxY = maxY - 10000;

   if (newMaxY > minY) {
      maxY = newMaxY;
      emit setAxesLabels(minX, maxX, minY, maxY);
   }
}




void ControlPointPlot::increaseMaxY()
{
   //if(maxY >= -1 && maxY < 1)
    //    maxY = maxY + 0.1;
   //else
   if (maxY >= -10 && maxY < 10)
        maxY = maxY + 1;
   else if (maxY >= -100 && maxY < 100)
      maxY = maxY + 10;
   else if (minY >= -1000 && maxY < 1000)
      maxY = maxY + 100;
   else if (minY >= -10000 && maxY < 10000)
      maxY = maxY + 1000;
   else if (minY >= -100000 && maxY < 100000)
      maxY = maxY + 10000;

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ControlPointPlot::autoAdjustXaxis(int iter)
{
   if(!problem->mesh->getIsMeshGenerated()) {
      return;
   }

   if (maxX - iter < 50) {
      maxX = maxX + 250;  // Increase maxX value in increments of 250
      emit setAxesLabels(minX, maxX, minY, maxY);
   }
}




void ControlPointPlot::autoAdjustYaxis()
{
   if(!problem->mesh->getIsMeshGenerated()) {
      return;
   }

   double minValue = minValues[whichControlPoint][whichVariable];
   double maxValue = maxValues[whichControlPoint][whichVariable];

   maxY = maxValue + 0.05 * (maxValue - minValue);  // To keep some space on the top of the plot window
   minY = minValue - 0.05 * (maxValue - minValue);  // Similar...

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ControlPointPlot::controlPointSpinBoxChanged(int cp)
{
   // Cuneyt: No multi-block support.

   if(!problem->mesh->getIsMeshGenerated()) {
      return;
   }

   whichControlPoint = cp - 1;
   autoAdjustYaxis();
   update();
}




void ControlPointPlot::changeUshow(bool state)
{
   whichVariable = 0;  // u-velocity
   autoAdjustYaxis();
   update();
}




void ControlPointPlot::changeVshow(bool state)
{
   whichVariable = 1;  // v-velocity
   autoAdjustYaxis();
   update();
}




void ControlPointPlot::changePshow(bool state)
{
   whichVariable = 2;  // pressure
   autoAdjustYaxis();
   update();
}
