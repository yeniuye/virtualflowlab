#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "ui_mainWindow.h"
#include "../guiCode/guiSolverThread.h"

class GLWidget;
class ConvergencePlot;
class ControlPointPlot;




class mainWindow:public QMainWindow, Ui::MainWindow
{
Q_OBJECT
public:
   GLWidget *glWidget;
   ConvergencePlot *convergencePlot;
   ControlPointPlot *controlPointPlot;
   mainWindow();

private:
   SolverThread solverThread;
   void createActions();
   void setupLineEditValidators();
   void resetMainWindow();
   void resetTabPages();
   void readResultFile();
   void closeEvent(QCloseEvent *event);

public slots:
   void showAbout();
   void newProblem();
   void openProblem();
   bool saveProblem();
   void saveProblemAs();
   void captureMainWindow();
   void captureViewWindow();
   void captureConvergencePlot();
   void captureControlPointPlot();
   void geometryButtonsClicked();
   void readCfdFile();
   void writeCfdFile();
   void writeInputFile();
   void lineDrawButtonClicked();
   void functionDrawButtonClicked();
   void arcDrawButtonClicked();
   void leftTabSelection (QWidget * w);
   void rightTabSelection (QWidget * w);
   void postProcessTabSelection (QWidget * w);
   void showPrimitivesBlockProperties(int);
   void blockFaceSetButtonClicked();
   void clusterTypeChanged(int);
   void showBC(int);
   void setBC();
   void setIC();
   void boundaryTypeChanged(int);
   void setParameters();
   void deletePrimitive(int);
   void deleteAllPrimitives();
   void generateMeshClicked();
   void startSolution();
   void pauseSolution();
   void continueSolution();
   void terminateSolution();
   void solverThreadDone();
   void appendMessage(QString, QColor);
   void restartStateChanged(int);
   void schemeOrStrategyChanged(int);
   void setConvergenceAxesLabels(double, double, double, double);
   void setControlPointAxesLabels(double, double, double, double);
   void openResultFile();
   void clearAllControlPoints();
   void clearAllBlockedCells();
   void visualizeButtonsClicked();
   void visualizeChecksToggled();
   void contourVarChanged(int);
   void contourStyleChanged(int);
   void colorMapChanged(int);
   void contourLevelChanged(const QString &);
   void showContoursButtonClicked();
   void probeAtPosition(float, float);
   void probeButtonClicked();
   void addStreamline(float, float);
   void placeStreamlineRadioButtonsToggled(bool);
   void probeRadioButtonsToggled(bool);
   void placeStreamlineButtonClicked();
   void clearAllStreamlinesButtonClicked();
   void clearTheLastStreamlineButtonClicked();
   void zoomButtonReleased();
   void updateCoorLabel(float, float);
   void updateIterLabel(int);
   void updateStatusLabel(QString);
   void updateInfoLabel(QString);

   bool checkBC();
   bool checkIC();
   bool checkParameters();
};

#endif
