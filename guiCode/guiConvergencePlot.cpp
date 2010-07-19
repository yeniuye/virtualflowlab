// This implementation is based on renderarea example

#include <math.h>
#include "guiConvergencePlot.h"
#include "mainWindow.h"




ConvergencePlot::ConvergencePlot(QWidget *parent) : QWidget(parent)
{
   //setBackgroundRole(QPalette::Light);  // This sets background of the widget to white
   //setAutoFillBackground(TRUE);         // This is necessary to make the previous line work properly

   penWidth = 2;

   penColor[0] = Qt::blue;
   penColor[1] = Qt::red;
   penColor[2] = Qt::black;
   penColor[3] = Qt::green;

   defaultMinX = 0;
   defaultMaxX = 1000;
   defaultMinY = -9;
   defaultMaxY = 2;

   minX = defaultMinX;
   maxX = defaultMaxX;
   minY = defaultMinY;
   maxY = defaultMaxY;

   showUresidual = TRUE;
   showVresidual = TRUE;
   showPresidual = TRUE;
   showMresidual = TRUE;

   emit setAxesLabels(defaultMinX, defaultMaxX, defaultMinY, defaultMaxY);

   // Initialize the starting point of the convergence lines
   //convergencePaths[0].moveTo(A*0+B, C*0+D);
   //convergencePaths[1].moveTo(A*0+B, C*0+D);
   //convergencePaths[2].moveTo(A*0+B, C*0+D);
   //convergencePaths[3].moveTo(A*0+B, C*0+D);

   /*QFont newFont = font();
   newFont.setPixelSize(12);
   setFont(newFont);
   
   QFontMetrics fontMetrics(newFont);
   xBoundingRect = fontMetrics.boundingRect(tr("x"));
   yBoundingRect = fontMetrics.boundingRect(tr("y"));
   */
}




void ConvergencePlot::initialize(int maxIter)
{
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

   convergencePaths[0] = emptyPath;	// Cuneyt: Will it be better to use delete-new ?
   convergencePaths[1] = emptyPath;
   convergencePaths[2] = emptyPath;
   convergencePaths[3] = emptyPath;

   convergencePaths[0].moveTo(A * 0 + B, C * 0 + D);
   convergencePaths[1].moveTo(A * 0 + B, C * 0 + D);
   convergencePaths[2].moveTo(A * 0 + B, C * 0 + D);
   convergencePaths[3].moveTo(A * 0 + B, C * 0 + D);

   emit setAxesLabels(minX, maxX, minY, maxY);

   update();
}  // end of initialize




void ConvergencePlot::paintEvent(QPaintEvent *)
{
   QPainter painter(this);
   //painter.setRenderHint(QPainter::Antialiasing);
   //drawLabels(painter);

   painter.scale(width()/(maxX-minX), height()/(maxY-minY));	// Scales the plot properly.
   painter.translate(-minX, maxY);	// Translates the origin into the proper location.

   if (showUresidual) {
      painter.setPen(QPen(penColor[0]));
      painter.drawPath(convergencePaths[0]);
   }
   if (showVresidual) {
      painter.setPen(QPen(penColor[1]));
      painter.drawPath(convergencePaths[1]);
   }
   if (showPresidual) {
      painter.setPen(QPen(penColor[2]));
      painter.drawPath(convergencePaths[2]);
   }
   if (showMresidual) {
      painter.setPen(QPen(penColor[3]));
      painter.drawPath(convergencePaths[3]);
   }

   //drawTicksAndGrids(painter);

}  // End of function paintEvent()




void ConvergencePlot::resizeEvent(QResizeEvent *)
{
   // Transformation between real data and screen pixels in the x direction (pixel = Ax + B)
   A = width()/(maxX - minX);
   B = - A * minX;
   
   // Transformation between real data and screen pixels in the y direction (pixel = Cy + D)
   C = - height()/(maxY - minY);
   D = - C * maxY;

   update();
}




void ConvergencePlot::formTicksAndGridsPath()
{

}




void ConvergencePlot::changeUresidualShow(bool state)
{
   showUresidual = state;
   update();
}




void ConvergencePlot::changeVresidualShow(bool state)
{
   showVresidual = state;
   update();
}




void ConvergencePlot::changePresidualShow(bool state)
{
   showPresidual = state;
   update();
}




void ConvergencePlot::changeMresidualShow(bool state)
{
   showMresidual = state;
   update();
}




void ConvergencePlot::updateResidual(int iter, double uRes, double vRes, double pRes, double mRes)
{
   if (iter == 1) {
      convergencePaths[0].moveTo(iter, -log10(uRes));  // Minus signs for the y component are used because in widget coordinates +y axis points downwards.
      convergencePaths[1].moveTo(iter, -log10(vRes));
      convergencePaths[2].moveTo(iter, -log10(pRes));
      convergencePaths[3].moveTo(iter, -log10(mRes));
   } else {
      convergencePaths[0].lineTo(iter, -log10(uRes));
      convergencePaths[1].lineTo(iter, -log10(vRes));
      convergencePaths[2].lineTo(iter, -log10(pRes));
      convergencePaths[3].lineTo(iter, -log10(mRes));
   }
   update();
}




void ConvergencePlot::decreaseMinX()
{
   if (minX > 0 && minX <= 1000)
      minX = minX - 100;
   else if (minX > 1000)
      minX = minX - 1000;
   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ConvergencePlot::increaseMinX()
{
   if (minX < 1000 && maxX-minX > 100)
      minX = minX + 100;
   else if (minX >= 1000 && maxX-minX > 1000)
      minX = minX + 1000;
   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ConvergencePlot::decreaseMaxX()
{
   if (maxX <= 1000 && maxX-minX > 100)
      maxX = maxX - 100;
   else if (maxX > 1000 && maxX-minX > 1000)
      maxX = maxX - 1000;

   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ConvergencePlot::increaseMaxX()
{
   if (maxX < 1000)
      maxX = maxX + 100;
   else if (maxX >= 1000)
      maxX = maxX + 1000;
   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ConvergencePlot::decreaseMinY()
{
   minY = minY - 1;
   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ConvergencePlot::increaseMinY()
{
   if (maxY-minY > 1)
      minY = minY + 1;
   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ConvergencePlot::decreaseMaxY()
{
   if (maxY-minY > 1)
      maxY = maxY - 1;
   emit setAxesLabels(minX, maxX, minY, maxY);
}




void ConvergencePlot::increaseMaxY()
{
   maxY = maxY + 1;
   emit setAxesLabels(minX, maxX, minY, maxY);
}
