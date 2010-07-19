// This header file is based on renderarea.h example

#ifndef CONVERGENCEPLOT_H
#define CONVERGENCEPLOT_H

#include <QtGui>




class ConvergencePlot : public QWidget
{
   Q_OBJECT

   public:
      ConvergencePlot(QWidget *parent = 0);
      void initialize(int);
      double minX, maxX, minY, maxY;	// Actual ranges of the plot.
      double defaultMinX, defaultMaxX, defaultMinY, defaultMaxY;  // Default values used at program startup or at the start of a new problem.
      bool showUresidual, showVresidual, showPresidual, showMresidual;

   signals:
      void setAxesLabels(double, double, double, double);

   public slots:
      void updateResidual(int, double, double, double, double);
      void changeUresidualShow(bool);
      void changeVresidualShow(bool);
      void changePresidualShow(bool);
      void changeMresidualShow(bool);
      void decreaseMinX();
      void increaseMinX();
      void decreaseMaxX();
      void increaseMaxX();
      void decreaseMinY();
      void increaseMinY();
      void decreaseMaxY();
      void increaseMaxY();

   protected:
      void paintEvent(QPaintEvent *event);
      void resizeEvent(QResizeEvent *event);

   private:
      void drawLabels(QPainter &painter);
      void formTicksAndGridsPath();

      QPainterPath emptyPath;             // Empty path
      QPainterPath convergencePaths[4];   // Paths for u, v, p & mass convergence
      int penWidth;                       // Pen width for convergence paths
      QColor penColor[4];                 // Pen colors for convergence paths
      int plotWidth, plotHeight;          // Dimensions of the convergence plot in pixel
      double A, B, C, D;                  // Constants for linear transformation between real data and screen pixels in the x and y directions.
};

#endif
