#include <QtGui>
#include <math.h>
#include <iomanip>
#include "mainWindow.h"
#include "glwidget.h"
#include "guiConvergencePlot.h"
#include "guiControlPointPlot.h"
#include "guiTypedefs.h"
#include "guiProblem.h"
#include "../guiCode/guiSolverThread.h"

extern Problem *problem;




mainWindow::mainWindow():QMainWindow()
{
   QString str;
   setupUi(this);
   
   glWidget = new GLWidget(this);
   convergencePlot = new ConvergencePlot(this);
   controlPointPlot = new ControlPointPlot(this);

   problem = new Problem;
   
   createActions();
   setupLineEditValidators();

   QHBoxLayout *glFrameLayout = new QHBoxLayout;
   glFrameLayout->setMargin(0);
   glFrameLayout->addWidget(glWidget);
   glFrame->setLayout(glFrameLayout);

   QHBoxLayout *convergenceLayout = new QHBoxLayout;
   convergenceLayout->setMargin(0);
   convergenceLayout->addWidget(convergencePlot);
   convergencePlotFrame->setLayout(convergenceLayout);

   QHBoxLayout *controlPointPlotLayout = new QHBoxLayout;
   controlPointPlotLayout->setMargin(0);
   controlPointPlotLayout->addWidget(controlPointPlot);
   controlPointPlotFrame->setLayout(controlPointPlotLayout);

   // SIGNALS and SLOTS

   connect(lineButton,                   SIGNAL( clicked() ),                    this, SLOT( geometryButtonsClicked() ));
   connect(arcButton,                    SIGNAL( clicked() ),                    this, SLOT( geometryButtonsClicked() ));
   connect(functionButton,               SIGNAL( clicked() ),                    this, SLOT( geometryButtonsClicked() ));
   connect(deleteButton,                 SIGNAL( clicked() ),                    this, SLOT( geometryButtonsClicked() ));
   connect(lineDraw,                     SIGNAL( clicked() ),                    this, SLOT( lineDrawButtonClicked() ));
   connect(arcDraw,                      SIGNAL( clicked() ),                    this, SLOT( arcDrawButtonClicked() ));
   connect(functionDraw,                 SIGNAL( clicked() ),                    this, SLOT( functionDrawButtonClicked() ));
   
   //connect(arcWithThreePoints,         SIGNAL( toggled(bool) ),                arcX3,          SLOT( setEnabled(bool) ));
   //connect(arcWithThreePoints,         SIGNAL( toggled(bool) ),                arcXcenter,     SLOT( setDisabled(bool) ));
   //connect(arcWithThreePoints,         SIGNAL( toggled(bool) ),                arcY3,          SLOT( setEnabled(bool) ));
   //connect(arcWithThreePoints,         SIGNAL( toggled(bool) ),                arcYcenter,     SLOT( setDisabled(bool) ));
   //connect(twoBlocksCheckBox,          SIGNAL( toggled(bool) ),                block2groupBox, SLOT( setEnabled(bool) ));

   connect(clusterTypeComboBox,          SIGNAL( currentIndexChanged(int) ),     this, SLOT( clusterTypeChanged(int) ));
   connect(generateMeshButton,           SIGNAL( clicked() ),                    this, SLOT( generateMeshClicked() ));
   connect(leftTab,                      SIGNAL( currentChanged (QWidget *) ),   this, SLOT( leftTabSelection (QWidget *) ));
   connect(rightTab,                     SIGNAL( currentChanged (QWidget *) ),   this, SLOT( rightTabSelection (QWidget *) ));
   connect(postProcessTab,               SIGNAL( currentChanged (QWidget *) ),   this, SLOT( postProcessTabSelection (QWidget *) ));
   connect(blockFaceSetButton,           SIGNAL( clicked() ),                    this, SLOT( blockFaceSetButtonClicked() ));
   connect(boundaryTypeComboBox,         SIGNAL( currentIndexChanged(int) ),     this, SLOT( boundaryTypeChanged(int) ));
   connect(setBCButton,                  SIGNAL( clicked() ),                    this, SLOT( setBC() ));
   //connect(setICButton,                SIGNAL( clicked() ),                    this, SLOT( setIC() ));
   //connect(setParametersButton,        SIGNAL( clicked() ),                    this, SLOT( setParameters() ));
   connect(restartCheck,                 SIGNAL( stateChanged(int) ),            this, SLOT( restartStateChanged(int) ));
   connect(strategyCombo,                SIGNAL( currentIndexChanged(int) ),     this, SLOT( schemeOrStrategyChanged(int) ));
   connect(schemeCombo,                  SIGNAL( currentIndexChanged(int) ),     this, SLOT( schemeOrStrategyChanged(int) ));
   connect(deleteAllButton,              SIGNAL( clicked() ),                    this, SLOT( deleteAllPrimitives() ));
   connect(startButton,                  SIGNAL( clicked() ),                    this, SLOT( startSolution() ));
   connect(pauseButton,                  SIGNAL( clicked() ),                    this, SLOT( pauseSolution() ));
   connect(continueButton,               SIGNAL( clicked() ),                    this, SLOT( continueSolution() ));
   connect(terminateButton,              SIGNAL( clicked() ),                    this, SLOT( terminateSolution() ));
   connect(openResultFileButton,         SIGNAL( clicked() ),                    this, SLOT( openResultFile() ));
   connect(clearAllBlockedCellsButton,   SIGNAL( clicked() ),                    this, SLOT( clearAllBlockedCells() ));
   connect(clearAllControlPointsButton,  SIGNAL( clicked() ),                    this, SLOT( clearAllControlPoints() ));

   // About the visualize page
   connect(boundaryCheckBox,             SIGNAL( toggled(bool) ),                this, SLOT(visualizeChecksToggled() ));
   connect(meshCheckBox,                 SIGNAL( toggled(bool) ),                this, SLOT(visualizeChecksToggled() ));
   connect(contourCheckBox,              SIGNAL( toggled(bool) ),                this, SLOT(visualizeChecksToggled() ));
   connect(streamlineCheckBox,           SIGNAL( toggled(bool) ),                this, SLOT(visualizeChecksToggled() ));
   connect(contourVarComboBox,           SIGNAL( currentIndexChanged(int) ),     this, SLOT( contourVarChanged(int) ));
   connect(contourStyleComboBox,         SIGNAL( currentIndexChanged(int) ),     this, SLOT( contourStyleChanged(int) ));
   connect(colorMapComboBox,             SIGNAL( currentIndexChanged(int) ),     this, SLOT( colorMapChanged(int) ));
   connect(contourLevelEdit,             SIGNAL( textChanged(const QString &) ), this, SLOT( contourLevelChanged(const QString &) ));
   connect(showContoursButton,           SIGNAL( clicked() ),                    this, SLOT( showContoursButtonClicked() ));
   connect(visualizeSelectButton,        SIGNAL( clicked() ),                    this, SLOT( visualizeButtonsClicked() ));
   connect(visualizeContourButton,       SIGNAL( clicked() ),                    this, SLOT( visualizeButtonsClicked() ));
   connect(visualizeStreamlineButton,    SIGNAL( clicked() ),                    this, SLOT( visualizeButtonsClicked() ));
   connect(visualizeProbeButton,         SIGNAL( clicked() ),                    this, SLOT( visualizeButtonsClicked() ));
   connect(probeXYradio,                 SIGNAL( toggled(bool) ),                this, SLOT( probeRadioButtonsToggled(bool) ));
   connect(probeButton,                  SIGNAL( clicked() ),                    this, SLOT( probeButtonClicked() ));
   connect(controlPointSpinBox,          SIGNAL( valueChanged(int) ),            controlPointPlot, SLOT( controlPointSpinBoxChanged(int) ));
   connect(placeStreamlineXYradio,       SIGNAL( toggled(bool) ),                this, SLOT( placeStreamlineRadioButtonsToggled(bool) ));
   connect(placeStreamlineButton,        SIGNAL( clicked() ),                    this, SLOT( placeStreamlineButtonClicked() ));
   connect(clearAllStreamlinesButton,    SIGNAL( clicked() ),                    this, SLOT( clearAllStreamlinesButtonClicked() ));
   connect(clearTheLastStreamlineButton, SIGNAL( clicked() ),                    this, SLOT( clearTheLastStreamlineButtonClicked() ));
   //connect(visualizeDataExtractButton, SIGNAL( clicked() ),                    this, SLOT( visualizeButtonsClicked() ));

   // Signals coming from the glWidget
   connect(glWidget, SIGNAL( appendOutput(QString, QColor) ),      this, SLOT( appendMessage(QString, QColor) ));
   connect(glWidget, SIGNAL( showPrimitivesBlockProperties(int) ), this, SLOT( showPrimitivesBlockProperties(int) ));
   connect(glWidget, SIGNAL( showBC(int) ),                        this, SLOT( showBC(int) ));
   connect(glWidget, SIGNAL( deletePrimitive(int) ),               this, SLOT( deletePrimitive(int) ));
   connect(glWidget, SIGNAL( xyChanged(float, float) ),            this, SLOT( updateCoorLabel(float, float) ));
   connect(glWidget, SIGNAL( probe(float, float) ),                this, SLOT( probeAtPosition(float, float) ));
   connect(glWidget, SIGNAL( addStreamline(float, float) ),        this, SLOT( addStreamline(float, float) ));
   connect(glWidget, SIGNAL( releaseZoomButton() ),                this, SLOT( zoomButtonReleased() ));

   // Signals going to the glWidget
   //connect(glSelectionButton,  SIGNAL( toggled(bool) ), glWidget, SLOT( setStateToSelection(bool) ));
   connect(glZoomButton,         SIGNAL( toggled(bool) ), glWidget, SLOT( setStateToZoom(bool) ));
   connect(glZoomInButton,       SIGNAL( clicked() ),     glWidget, SLOT( zoomIn() ));
   connect(glZoomOutButton,      SIGNAL( clicked() ),     glWidget, SLOT( zoomOut() ));
   connect(glRedrawButton,       SIGNAL( clicked() ),     glWidget, SLOT( redraw() ));
   connect(glPreviousViewButton, SIGNAL( clicked() ),     glWidget, SLOT( showPreviousView() ));
   connect(glShowAllButton,      SIGNAL( clicked() ),     glWidget, SLOT( showAll() ));

   connect(&solverThread, SIGNAL(sendIteration(int)),                                this, SLOT(updateIterLabel(int)));
   connect(&solverThread, SIGNAL(sendRunStatus(QString)),                            this, SLOT(updateStatusLabel(QString)));
   connect(&solverThread, SIGNAL(sendResidual(int, double, double, double, double)), convergencePlot, SLOT(updateResidual(int, double, double, double, double)));
   connect(&solverThread, SIGNAL(sendControlPointData(int)),                         controlPointPlot, SLOT(updateControlPoints(int)));
   connect(&solverThread, SIGNAL(finished()),                                        this, SLOT(solverThreadDone()));
   connect(&solverThread, SIGNAL(appendOutput(QString, QColor) ),                    this, SLOT( appendMessage(QString, QColor) ));

   connect(uResidualCheck, SIGNAL(toggled(bool) ), convergencePlot, SLOT( changeUresidualShow(bool) ));
   connect(vResidualCheck, SIGNAL(toggled(bool) ), convergencePlot, SLOT( changeVresidualShow(bool) ));
   connect(pResidualCheck, SIGNAL(toggled(bool) ), convergencePlot, SLOT( changePresidualShow(bool) ));
   connect(mResidualCheck, SIGNAL(toggled(bool) ), convergencePlot, SLOT( changeMresidualShow(bool) ));

   connect(convergencePlot,         SIGNAL(setAxesLabels(double, double, double, double)), this, SLOT(setConvergenceAxesLabels(double, double, double, double)));
   connect(convergenceMinXdecrease, SIGNAL( clicked() ), convergencePlot, SLOT( decreaseMinX() ));
   connect(convergenceMinXincrease, SIGNAL( clicked() ), convergencePlot, SLOT( increaseMinX() ));
   connect(convergenceMaxXdecrease, SIGNAL( clicked() ), convergencePlot, SLOT( decreaseMaxX() ));
   connect(convergenceMaxXincrease, SIGNAL( clicked() ), convergencePlot, SLOT( increaseMaxX() ));
   connect(convergenceMinYdecrease, SIGNAL( clicked() ), convergencePlot, SLOT( decreaseMinY() ));
   connect(convergenceMinYincrease, SIGNAL( clicked() ), convergencePlot, SLOT( increaseMinY() ));
   connect(convergenceMaxYdecrease, SIGNAL( clicked() ), convergencePlot, SLOT( decreaseMaxY() ));
   connect(convergenceMaxYincrease, SIGNAL( clicked() ), convergencePlot, SLOT( increaseMaxY() ));

   connect(controlPointUcheck,      SIGNAL(toggled(bool) ), controlPointPlot, SLOT( changeUshow(bool) ));
   connect(controlPointVcheck,      SIGNAL(toggled(bool) ), controlPointPlot, SLOT( changeVshow(bool) ));
   connect(controlPointPcheck,      SIGNAL(toggled(bool) ), controlPointPlot, SLOT( changePshow(bool) ));

   connect(controlPointPlot, SIGNAL(setAxesLabels(double, double, double, double)), this, SLOT(setControlPointAxesLabels(double, double, double, double)));
   connect(controlPointMinXdecrease, SIGNAL( clicked() ), controlPointPlot, SLOT( decreaseMinX() ));
   connect(controlPointMinXincrease, SIGNAL( clicked() ), controlPointPlot, SLOT( increaseMinX() ));
   connect(controlPointMaxXdecrease, SIGNAL( clicked() ), controlPointPlot, SLOT( decreaseMaxX() ));
   connect(controlPointMaxXincrease, SIGNAL( clicked() ), controlPointPlot, SLOT( increaseMaxX() ));
   connect(controlPointMinYdecrease, SIGNAL( clicked() ), controlPointPlot, SLOT( decreaseMinY() ));
   connect(controlPointMinYincrease, SIGNAL( clicked() ), controlPointPlot, SLOT( increaseMinY() ));
   connect(controlPointMaxYdecrease, SIGNAL( clicked() ), controlPointPlot, SLOT( decreaseMaxY() ));
   connect(controlPointMaxYincrease, SIGNAL( clicked() ), controlPointPlot, SLOT( increaseMaxY() ));
   //connect(controlPointXaxisAuto,  SIGNAL( clicked() ), controlPointPlot, SLOT( autoAdjustXaxis() ));
   connect(controlPointYaxisAuto,    SIGNAL( clicked() ), controlPointPlot, SLOT( autoAdjustYaxis() ));

   // End of SIGNALS and SLOTS

   // Some initializations on the mainWindow
   resetMainWindow();
   outputText->setTextColor(Qt::black);

}  // End of function mainWindow()




void mainWindow::resetMainWindow()
{
   // Make some initializations so that the GUI looks like the way VFL is started the first time

   resetTabPages();

   setWindowTitle(tr("Virtual Flow Lab - Working on a new problem"));

   deleteButton->setEnabled(FALSE);
   deleteAllButton->setEnabled(FALSE);
   visualizeSelectButton->setChecked(true);
      
   uICedit->setText("");
   vICedit->setText("");
   pICedit->setText("");
   statusLabel->setText(tr("Status: NOT STARTED"));
   iterLabel->setText(tr("Iter: 0"));
      
   schemeCombo->setCurrentIndex(0);
   discretizationCombo->setCurrentIndex(0);
   strategyCombo->setCurrentIndex(0);
   timeStepEdit->setText("");
   kinvisEdit->setText("");
   densityEdit->setText("");
   uRelaxationEdit->setText("");
   vRelaxationEdit->setText("");
   pRelaxationEdit->setText("");
   //scalarRelaxationEdit->setText("");
   maxOuterIterEdit->setText("");
   outerToleranceEdit->setText("");
   outputIntervalEdit->setText("");
   controlPointUpdateIntervalEdit->setText("");
   timeDependentCheck->setChecked(FALSE);
   timeDependentCheck->setEnabled(FALSE);
   DATfileCheck->setChecked(TRUE);
   restartCheck->setChecked(FALSE);

}  // End of function resetMainWindow()




void mainWindow::resetTabPages()
{
   postProcessTab->setCurrentIndex(1);  // This is used just to make the following line work properly.
   postProcessTab->setCurrentIndex(0);  // Show the "Contour" page of postProcessTab

   leftTab->setCurrentIndex(1);         // This is used just to make the following line work properly.
   leftTab->setCurrentIndex(0);         // Show the "Geometry" page of leftTab

   rightTab->setCurrentIndex(0);        // Show the "Messages / Output" page of rightTab
   
   geometryStack->setCurrentIndex(1);   // Show the Draw Line page of geometryStack
}




void mainWindow::createActions()
{
   connect(actionAbout,               SIGNAL(triggered()), this, SLOT( showAbout() ));
   connect(actionNew,                 SIGNAL(triggered()), this, SLOT( newProblem() ));
   connect(actionOpen,                SIGNAL(triggered()), this, SLOT( openProblem() ));
   connect(actionSave,                SIGNAL(triggered()), this, SLOT( saveProblem() ));
   connect(actionSave_As,             SIGNAL(triggered()), this, SLOT( saveProblemAs() ));
   connect(actionExit,                SIGNAL(triggered()), this, SLOT( close() ));
   connect(actionCaptureMain,         SIGNAL(triggered()), this, SLOT( captureMainWindow() ));
   connect(actionCaptureView,         SIGNAL(triggered()), this, SLOT( captureViewWindow() ));
   connect(actionCaptureConvergence,  SIGNAL(triggered()), this, SLOT( captureConvergencePlot() ));
   connect(actionCaptureControlPoint, SIGNAL(triggered()), this, SLOT( captureControlPointPlot() ));
}



void mainWindow::setupLineEditValidators()
{
   // Cuneyt: These validators are incomplete.
   //         QdoubleValidator allows "e1", but look like that converting it to double is peroblematic, e.g. clusterCoeffEdit

   // To draw lines
   lineX1->setValidator(new QDoubleValidator(lineX1));
   lineY1->setValidator(new QDoubleValidator(lineY1));
   lineX2->setValidator(new QDoubleValidator(lineX2));
   lineY2->setValidator(new QDoubleValidator(lineY2));

   // To draw arcs
   arcX1->setValidator(new QDoubleValidator(arcX1));
   arcY1->setValidator(new QDoubleValidator(arcY1));
   arcX2->setValidator(new QDoubleValidator(arcX2));
   arcY2->setValidator(new QDoubleValidator(arcY2));
   arcX3->setValidator(new QDoubleValidator(arcX3));
   arcY3->setValidator(new QDoubleValidator(arcY3));

   // To setup Blocks/Faces
   NoNodesEdit->setValidator(new QIntValidator(NoNodesEdit));
   clusterCoeffEdit->setValidator(new QDoubleValidator(clusterCoeffEdit));      // Cuneyt: Can NOT be too large or too small
    
   // To create mesh
   // meshTolerance->setValidator(new QDoubleValidator(meshTolerance));         // Cuneyt: Can NOT be negative
   // meshWeightFactor->setValidator(new QDoubleValidator(meshWeightFactor));   // Cuneyt: Can NOT be negative or too large

   // To set BC/IC strings
   // Maximum number of characters that these boxes can take are set to be 20 by QtDesigner.

   // To set Solver parameters
   timeStepEdit->setValidator(new QDoubleValidator(timeStepEdit));                   // Cuneyt: Can NOT be negative
   kinvisEdit->setValidator(new QDoubleValidator(kinvisEdit));                       // Cuneyt: Can NOT be negative
   densityEdit->setValidator(new QDoubleValidator(densityEdit));                     // Cuneyt: Can NOT be negative
   uRelaxationEdit->setValidator(new QDoubleValidator(uRelaxationEdit));             // Cuneyt: Can NOT be negative
   vRelaxationEdit->setValidator(new QDoubleValidator(vRelaxationEdit));             // Cuneyt: Can NOT be negative
   pRelaxationEdit->setValidator(new QDoubleValidator(pRelaxationEdit));             // Cuneyt: Can NOT be negative
   //scalarRelaxationEdit->setValidator(new QDoubleValidator(scalarRelaxationEdit)); // Cuneyt: Can NOT be negative
   maxOuterIterEdit->setValidator(new QIntValidator(maxOuterIterEdit));              // Cuneyt: Can NOT be negative. Can NOT be larger than the limit of type int.
   outerToleranceEdit->setValidator(new QDoubleValidator(outerToleranceEdit));       // Cuneyt: Can NOT be negative.
   outputIntervalEdit->setValidator(new QIntValidator(outputIntervalEdit));          // Cuneyt: Can NOT be negative. Can NOT be larger than the limit of type int.
   controlPointUpdateIntervalEdit->setValidator(new QIntValidator(controlPointUpdateIntervalEdit));    // Cuneyt: Can NOT be negative. Can NOT be larger than the limit of type int.

   // To visualize
   contourLevelEdit->setValidator(new QIntValidator(1,500,contourLevelEdit));
   streamlineStepSizeEdit->setValidator(new QDoubleValidator(0.001,100,6,streamlineStepSizeEdit));
   streamlineMaxStepsEdit->setValidator(new QIntValidator(1,9999,streamlineMaxStepsEdit));
   placeStreamlineXedit->setValidator(new QDoubleValidator(placeStreamlineXedit));   // Cuneyt: This box sometimes requires int and sometimes double. Be careful.
   placeStreamlineYedit->setValidator(new QDoubleValidator(placeStreamlineYedit));   // Cuneyt: This box sometimes requires int and sometimes double. Be careful.
   probeX->setValidator(new QDoubleValidator(probeX));   // Cuneyt: This box sometimes requires int and sometimes double. Be careful.
   probeY->setValidator(new QDoubleValidator(probeY));   // Cuneyt: This box sometimes requires int and sometimes double. Be careful.

}  // End of function setupLineEditValidators()




void mainWindow::showAbout()
{
   // Opens up an About window with the following information

   QMessageBox::about(this, QString::fromUtf8("About Virtual Flow Lab"),
                      QString::fromUtf8("A CFD program for educational purposes. \n\n"
                                        "Cuneyt Sert (csert@metu.edu.tr) \n"
                                        "Gunes Nakiboglu (gunesnakib@gmail.com) \n\n"
                                        "Department of Mechanical Engineering \n"
                                        "Middle East Technical University \n"
                                        "Ankara, Turkey \n\n"
                                        "http://www.me.metu.edu.tr/people/cuneyt \n"
                                        "http://code.google.com/p/virtualflowlab \n\n"
                                        "2006-2010  Licensed by GPL v3"));
}




void mainWindow::closeEvent(QCloseEvent *event)
{
   if (solverThread.isRunning()) {
      QMessageBox::warning(this, QString::fromUtf8("Solver is running or paused."), QString::fromUtf8("Before quitting VFL, you first need to terminate the solution."));
      event->ignore();
   } else {
      if (problem->needToSave) {
         QMessageBox::StandardButton msgBox;
         msgBox = QMessageBox::warning(this, tr("Virtual Flow Lab"), tr("Do you want to save your possible changes?"),
                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         //msgBox.setDefaultButton(QMessageBox::Save);
         if (msgBox == QMessageBox::Save) {
            if (saveProblem()) {
               event->accept();  // Problem is saved. Can quit now.
            } else {
               event->ignore();  // Problem is not saved. Can not quit now.
            }
         } else if (msgBox == QMessageBox::Discard) {
            event->accept();     // User selected not to save. Can quit now.
         } else if (msgBox == QMessageBox::Cancel) {
            event->ignore();     // User selected to cancel the action. Do not quit.
         }
      } else {
         event->accept();        // No need to save. Can quit now.
      }
   }
}




void mainWindow::newProblem()
{
   // Cuneyt: Should ask about storing the old problem.

   delete problem;			// Delete the old problem
   problem = new Problem;	// Create a new problem
   outputText->clear();
   appendMessage(tr("A new problem is created."), Qt::black);
   resetMainWindow();
}




void mainWindow::openProblem()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open Problem"), "", tr("Problem Files (*.cfd)"));

   if (!fileName.isEmpty()){
      delete problem;			// Delete the old problem
      problem = new Problem;	// Create a new problem
      
      QFileInfo file( fileName );
      problem->setDir(file.absolutePath().toStdString());             // Path to the CFD file, e.g C:\VFL\Problems
      problem->setName(file.completeBaseName().toStdString());        // Problem name, e.g. MyProblem
      problem->setFileNames(problem->getDir(), problem->getName());   // Full problem name with path and extension, e.g. C:\VFL\Problems\MyProblem.cfd

      resetMainWindow();
      readCfdFile();
      
      setWindowTitle(tr("Virtual Flow Lab - Working on problem %1").arg(problem->getName().c_str()));
      outputText->clear();
      appendMessage(tr("Problem %1 is opened.").arg(problem->getName().c_str()), Qt::black);

      glWidget->showAll();    // Show all the primitives after drawing them

      if (problem->mesh->getIsMeshGenerated()) {
         glWidget->generateMeshList();        // Generate the glList for the mesh.
         glWidget->generateMeshPointList();   // Generate the glList for the mesh points (used for the selection of control points).
      }
   }
}  // End of function openProblem()




bool mainWindow::saveProblem()
{
   QString directory( problem->getDir().c_str() );
   if (problem->getName() == "") {	// Saving the first time
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save Problem"), directory, tr("Problem Files (*.cfd)"));

      if (fileName.isEmpty()) {
         return false;   // Will not be saved
      } else {
         if (! fileName.endsWith(".cfd", Qt::CaseInsensitive)) {  // Add the extension .cfd
            fileName = fileName + ".cfd";
         }
         QFileInfo file( fileName );
         problem->setDir(file.absolutePath().toStdString());             // Path to the CFD file, e.g C:\VFL\Problems
         problem->setName(file.completeBaseName().toStdString());        // Problem name, e.g. MyProblem
         problem->setFileNames(problem->getDir(), problem->getName());   // Full problem name with path and extension, e.g. C:\VFL\Problems\MyProblem.cfd

         //setParameters();
         writeCfdFile();
         writeInputFile();
         setWindowTitle(tr("Virtual Flow Lab - Working on problem %1").arg(problem->getName().c_str()));
         appendMessage(tr("Problem is saved as %1.").arg(problem->getName().c_str()), Qt::black);
         return true;
      }
   } else {
      writeCfdFile();
      writeInputFile();
      setWindowTitle(tr("Virtual Flow Lab - Working on problem %1").arg(problem->getName().c_str()));
      appendMessage(tr("Problem %1 is saved.").arg(problem->getName().c_str()), Qt::black);
      return true;
   }
}  // End of function saveProblem()




void mainWindow::saveProblemAs()
{
   QString directory( problem->getDir().c_str() );
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save Problem"), directory, tr("Problem Files (*.cfd)"));

   if (!fileName.isEmpty()) {
      if (! fileName.endsWith(".cfd", Qt::CaseInsensitive))  // Add the extension .cfd
         fileName = fileName + ".cfd";

      QFileInfo file( fileName );
         problem->setDir(file.absolutePath().toStdString());             // Path to the CFD file, e.g C:\VFL\Problems
         problem->setName(file.completeBaseName().toStdString());        // Problem name, e.g. MyProblem
         problem->setFileNames(problem->getDir(), problem->getName());   // Full problem name with path and extension, e.g. C:\VFL\Problems\MyProblem.cfd

      //setParameters();
      writeCfdFile();
      writeInputFile();
      setWindowTitle(tr("Virtual Flow Lab - Working on problem %1").arg(problem->getName().c_str()));
      appendMessage(tr("Problem is saved as %1.").arg(problem->getName().c_str()), Qt::black);
   }
}  // End of function saveProblemAs()




void mainWindow::captureMainWindow()
{
   QString directory( problem->getDir().c_str() );
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save Captured Image As"), directory, "*.png");

   if (!fileName.isEmpty()) {
      if (! fileName.endsWith(".png", Qt::CaseInsensitive))   // Add the extension .png
         fileName = fileName + ".png";

      QPixmap tempImage = QPixmap::grabWidget(this);
      tempImage.save(fileName, "PNG", 0);   // Last parameter is about the image quality. 0 is the lowest quality,
                                            // but it gives good results with PNG.
      appendMessage(tr("Captured image is saved."), Qt::black);
   }
}




void mainWindow::captureViewWindow()
{
   QString directory( problem->getDir().c_str() );
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save Captured Image As"), directory, "*.png");

   if (!fileName.isEmpty()){
      if (! fileName.endsWith(".png", Qt::CaseInsensitive))  // Add the extension .png
         fileName = fileName + ".png";
      
      glWidget->makeCurrent();
      QImage tempImage = QImage( glWidget->grabFrameBuffer() );
      tempImage.save(fileName, "PNG", 0);   // Last parameter is about the image quality. 0 is the lowest quality,
                                            // but it gives god results with PNG.
      appendMessage(tr("Captured image is saved."), Qt::black);
   }
}




void mainWindow::captureConvergencePlot()
{
   QString directory( problem->getDir().c_str() );
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save Captured Image As"), directory, "*.png");

   if (!fileName.isEmpty()){
      if (! fileName.endsWith(".png", Qt::CaseInsensitive))  // Add the extension .png
         fileName = fileName + ".png";

      QPixmap tempImage = QPixmap::grabWidget(convergencePlot);
      tempImage.save(fileName, "PNG", 0);   // Last parameter is about the image quality. 0 is the lowest quality,
                                            // but it gives good results with PNG.
      appendMessage(tr("Captured image is saved."), Qt::black);
   }
}




void mainWindow::captureControlPointPlot()
{
   QString directory( problem->getDir().c_str() );
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save Captured Image As"), directory, "*.png");

   if (!fileName.isEmpty()){
      if (! fileName.endsWith(".png", Qt::CaseInsensitive))  // Add the extension .png
         fileName = fileName + ".png";

      QPixmap tempImage = QPixmap::grabWidget(controlPointPlot);
      tempImage.save(fileName, "PNG", 0);   // Last parameter is about the image quality. 0 is the lowest quality,
                                            // but it gives good results with PNG.
      appendMessage(tr("Captured image is saved."), Qt::black);
   }
}




void mainWindow::leftTabSelection(QWidget * w)
{
   int i;
   i = leftTab->currentIndex();
   
   if (i == 0) {  // "Geometry" page is selected
      problem->setMainState(GEOM);
   } else if (i == 1)  {  // "Mesh" page is selected
      problem->setMainState(MESH);

      // Reset the page
      // twoBlocksCheckBox->setChecked(FALSE);
      // block1spinBox->setValue(1);
      // block2spinBox->setValue(1);

      // These details are necessary to uncheck autoexclusive radio buttons
      block1SouthRadio->setAutoExclusive( FALSE );  block1SouthRadio->setChecked( FALSE );  block1SouthRadio->setAutoExclusive( TRUE );
      block1EastRadio->setAutoExclusive( FALSE );	 block1EastRadio->setChecked( FALSE );   block1EastRadio->setAutoExclusive( TRUE );
      block1NorthRadio->setAutoExclusive( FALSE );  block1NorthRadio->setChecked( FALSE );  block1NorthRadio->setAutoExclusive( TRUE );
      block1WestRadio->setAutoExclusive( FALSE );	 block1WestRadio->setChecked( FALSE );   block1WestRadio->setAutoExclusive( TRUE );

      // block2SouthRadio->setAutoExclusive( FALSE );  block2SouthRadio->setChecked( FALSE );  block2SouthRadio->setAutoExclusive( TRUE );
      // block2EastRadio->setAutoExclusive( FALSE );	 block2EastRadio->setChecked( FALSE );   block2EastRadio->setAutoExclusive( TRUE );
      // block2NorthRadio->setAutoExclusive( FALSE );  block2NorthRadio->setChecked( FALSE );  block2NorthRadio->setAutoExclusive( TRUE );
      // block2WestRadio->setAutoExclusive( FALSE );	 block2WestRadio->setChecked( FALSE );   block2WestRadio->setAutoExclusive( TRUE );

      NoNodesEdit->setText("");
      clusterTypeComboBox->setCurrentIndex(0);
      clusterCoeffEdit->setText("");

      meshMethodCombo->setCurrentIndex(0);

   } else if (i == 2) {   // "BC/IC" page is selected
      problem->setMainState(BCIC);
      
      // Reset the "BC/IC" page
      boundaryTypeComboBox->setCurrentIndex(0);
      uBCedit->setText("");
      vBCedit->setText("");
      pBCedit->setText("");
   } else if (i == 3) {    // "Blocked Cells" page is selected
      problem->setMainState(BLOCKEDCELLS);
      problem->mesh->setPrimitiveUnderMouse(-1);                       // Cuneyt: Is this necessary ?
      glWidget->updateGL();                                            // Cuneyt: Is this necessary ?
   } else if (i == 4) {    // "Control Points" page is selected
      problem->setMainState(CONTROLPOINTS);
      problem->mesh->setPrimitiveUnderMouse(-1);                       // Cuneyt: Is this necessary ?
      glWidget->updateGL();                                            // Cuneyt: Is this necessary ?
   } else if (i == 5) {    // "Solve" page is selected
      problem->setMainState(PARAMETERS);
      problem->mesh->setPrimitiveUnderMouse(-1);                       // Cuneyt: Buna gerek var mi?
      glWidget->updateGL();                                            // Cuneyt: Buna gerek var mi?
   } else if (i == 6) {    // "Visualize" page is selected
      problem->setMainState(VISUALIZE);
   }

   // Draw all primitives with a thickness of 2.0 and black color and update the drawing
   for(int j = 0; j < MAX_PRIMITIVES; j++) {
      problem->mesh->primitives[j].setColor(Qt::black);
      problem->mesh->primitives[j].setThickness(2.0);
   }
   
   problem->mesh->setPrimitiveUnderMouse(-1);
   glWidget->updateGL();

}  // End of function leftTabSelection()




void mainWindow::rightTabSelection(QWidget * w)
{
   int i;

   i = rightTab->currentIndex();

   if (i == 2) {   // "Control Points" page is selected
      this->controlPointPlot->autoAdjustYaxis();
      this->controlPointPlot->update();
   }
}




void mainWindow::appendMessage(QString message, QColor color)
{
   //QString s = outputText->toPlainText();
   //outputText->setTextColor(Qt::gray);
   //outputText->clear();
   //outputText->append(s);
   outputText->setTextColor(color);
   outputText->append(message);
   //rightTab->setCurrentIndex(0);  // Show the "Messages / Output" page of the rightTab
}




void mainWindow::updateCoorLabel(float x, float y)
{
   QString str;
   str = QString("Coor: %1, %2").arg(x).arg(y);
   coorLabel->setText(str);
}




void mainWindow::updateInfoLabel(QString str)
{
   infoLabel->setText(str);
}




void mainWindow::updateStatusLabel(QString str)
{
   QString str2;
   str2 = QString("Status: %1").arg(str);
   statusLabel->setText(str2);
}




void mainWindow::updateIterLabel(int iter)
{
   QString str;
   str = QString("Iter: %1").arg(iter);
   iterLabel->setText(str);
}




void mainWindow::zoomButtonReleased()
{
   this->glZoomButton->toggle();
}
