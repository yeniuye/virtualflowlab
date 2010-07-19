#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QGLWidget>
#include "guiTypedefs.h"




class GLWidget : public QGLWidget
{
   Q_OBJECT

public:
   GLWidget(QWidget *parent);
   ~GLWidget();

   double XtoYratio;
   QColor backgroundColor;
   bool showCoordinateAxes;
   double minX, maxX;  // Minimum and maximum x values showing on the openGL window
   double minY, maxY;  // Minimum and maximum y values showing on the openGL window
                       // Cuneyt: for the time being x to y ratio is taken to be 1.0 (fixed value),
                       //         which means that these min and max values for x and y are somewhat
                       //         related and can not be set arbitrarily.
   double minXprev, maxXprev;  // Same as above but shows the min and max values before a zoom, pan or window resize.
   double minYprev, maxYprev;  // Minimum and maximum y values showing on the OpenGL window.
   int wPrev, hPrev;           // Size of the GL window before a resize.
   QPoint click1Pos;	// Will be used for zooming & panning.
   bool clicked1;		// Will be used for zooming & panning.

   QRubberBand *rubberBand;


public slots:
   void drawPrimitives();
   void drawBoundaryPoints();
   void generateMeshList();
   void drawStructuredMesh();
   void drawMeshPoints();
   void drawControlPoints();
   void drawBlockedCells(QColor);
   void generateMeshPointList();
   void drawContour();
   void drawStreamline();
   void drawBWCells();
   void drawAverageBWCells();
   void drawColoredCells();
   void drawAverageColoredCells();
   int  primitiveAtPosition(const QPoint &);
   int  meshPointAtPosition(const QPoint &);
   int  meshCellAtPosition(const QPoint &);
   void setStateToZoom(bool);
   void zoomIn();
   void zoomOut();
   void showAll();
   void showPreviousView();
   void redraw();
   int findContourInterval(double, int, double* &, int);
   //void showGLPreferences();
   // void setStateToSelection(bool);
   // void setStateToPan(bool);

signals:
   void appendOutput(QString, QColor);
   void showPrimitivesBlockProperties(int);
   void deletePrimitive(int);
   void showBC(int);
   void xyChanged(float, float);
   void probe(float, float);
   void addStreamline(float, float);
   void releaseZoomButton();

protected:
   void initializeGL();
   void paintGL();
   void resizeGL( int, int );
   void calculateMinMaxXY( int, int );
   void mousePressEvent( QMouseEvent * );
   void mouseMoveEvent( QMouseEvent * );
   void mouseReleaseEvent( QMouseEvent * );
   void zoom(int);
   void zoomIntoRectangle();
   void pan(const QPoint &);
   void resizeEvent(QResizeEvent * );
   // virtual GLuint  makeObject();
   // void zoom(const QPoint &);

private:
   bool isRubberBandZooming;
   bool isContinuousZooming;
   bool isPanning;
   void drawZoomRectangle();
   float zoomRectangleX0, zoomRectangleX1, zoomRectangleY0, zoomRectangleY1;
   GLuint object;
   // void createActions();
};

#endif
