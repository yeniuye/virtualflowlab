// Based on renderarea.h example

#ifndef CONTROLPOINTPLOT_H
#define CONTROLPOINTPLOT_H

#include <QtGui>
#include "guiTypedefs.h"

class ControlPointPlot : public QWidget
{
   Q_OBJECT

   public:
      ControlPointPlot(QWidget *parent = 0);
      void initialize(int);
      double minX, maxX, minY, maxY;	// Actual ranges of the plots. Note that u, v, p plots have their own minY, maxY values.
      double defaultMinX, defaultMaxX, defaultMinY, defaultMaxY;	// Default values used at program startup or at the start of a new problem.
      double minValues[MAX_CONTROL_POINTS][3];  // Minimum of u, v and p values that are shown on the plot
      double maxValues[MAX_CONTROL_POINTS][3];  // Maximum of u, v and p values that are shown on the plot
      bool showU, showV, showP;

   signals:
      void setAxesLabels(double, double, double, double);

   public slots:
      void updateControlPoints(int);
		void changeUshow(bool);
		void changeVshow(bool);
		void changePshow(bool);
		void decreaseMinX();
		void increaseMinX();
		void decreaseMaxX();
		void increaseMaxX();
		void decreaseMinY();
		void increaseMinY();
		void decreaseMaxY();
		void increaseMaxY();
		void controlPointSpinBoxChanged(int);
      //void autoAdjustXaxis();
      void autoAdjustYaxis();

    protected:
      void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

    private:
		void drawLabels(QPainter &painter);
		void formTicksAndGridsPath();

		QPainterPath emptyPath;			// Empty path
		QPainterPath controlPointPaths[MAX_CONTROL_POINTS][3];	// Paths for u, v, p control points
      int penWidth;					// Pen width for control point paths
      QColor penColor[3];				// Pen colors for u, v and p paths
		int plotWidth, plotHeight;		// Dimensions of the control point plot in pixel
		double A, B, C, D;				// Constants for linear transformation between real data and screen pixels in the x and y directions.
		int whichVariable;				// Indicates which variable we are plotting (0: u, 1: v, 2: p)
		int whichControlPoint;			// Indicates the control point that we are plotting for.
    };

    #endif



