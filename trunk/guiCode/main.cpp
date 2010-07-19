/************************************************************************************

                                VIRTUAL FLOW LAB (VFL)

 VFL is a Computational Fluid Dynamics (CFD) software for educational purposes. It
 can solve 2D, laminar, incompressible flows using the SIMPLE algorithm and its
 variants. Although it can only work on Cartesian meshes with no cut cell support,
 it is still possible to create complex flow doamins using Patankar's "blocked cell"
 approach.

 Developed by  : Dr. Cuneyt Sert      csert@metu.edu.tr
                 Mr. Gunes Nakiboglu  gunesnakib@gmail.com

 Maintained by : Dr. Cuneyt Sert

 Websites      : http://www.me.metu.edu.tr/people/cuneyt/virtualFlowLab
                 http://code.google.com/p/virtualFlowLab

 License       : GPL v3

 ************************************************************************************/

#include <QtGui>
#include "mainWindow.h"
#include "glwidget.h"
#include "guiProblem.h"

Problem *problem;
bool pauseSolveThread;
bool terminateSolveThread;
QWaitCondition continueSolveThread;




int main(int argc, char *argv[])
{
   qRegisterMetaType< QColor >("QColor");

   QApplication app(argc, argv);

   QTranslator translator;
   translator.load("virtualFlowLab_tr");
   app.installTranslator(&translator);

   mainWindow myMainWindow;
   myMainWindow.show();
   myMainWindow.glWidget->showAll();

   app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
   return app.exec();
}
