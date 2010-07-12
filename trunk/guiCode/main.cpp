/****************************************************************************/
/*                             VIRTUAL FLOW LAB                             */
/*                                                                          */
/* A Computational Fluid Dynamics (CFD) software for educational purposes   */
/*                                                                          */
/* Developed by: Dr. Cuneyt SERT   csert@metu.edu.tr                        */
/*               Gunes NAKIBOGLU   gunesnakib@gmail.com                     */
/*                                                                          */
/* Website:      http://www.me.metu.edu.tr/people/cuneyt/virtualFlowLab     */
/*               http://code.google.com/p/virtualflowlab                    */
/*                                                                          */
/* License: GPL v3                                                          */
/****************************************************************************/

#include <QtGui>
#include "mainWindow.h"
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

   app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
   return app.exec();
}
