# #####################################################################
# Hand made Last Update 2 Dec 2009
# #####################################################################
TEMPLATE = vcapp
TARGET += 
DEPENDPATH += . forms guiCode solverCode
INCLUDEPATH += .

# Input
HEADERS += guiCode/mainWindow.h \
    guiCode/glwidget.h \
    guiCode/guiTypedefs.h \
    guiCode/guiStructuredMesh.h \
    guiCode/guiProblem.h \
    guiCode/guiPrimitive.h \
    guiCode/guiFace.h \
    guiCode/guiConvergencePlot.h \
    guiCode/guiControlPointPlot.h \
    guiCode/guiBlock.h \
    fparserCode/fptypes.h \
    fparserCode/fparser.h \
    fparserCode/fpconfig.h \
    solverCode/slvSolverThread.h \
    solverCode/slvFunctions.h
FORMS += forms/mainWindow.ui
SOURCES += guiCode/main.cpp \
    fparserCode/fparser.cpp \
    fparserCode/fpoptimizer.cpp \
    guiCode/TODO.cpp \
    guiCode/guiVisualizePage.cpp \
    guiCode/guiStructuredMesh.cpp \
    guiCode/guiSolvePage.cpp \
    guiCode/guiReadWriteCfdFile.cpp \
    guiCode/guiProblem.cpp \
    guiCode/guiPrimitive.cpp \
    guiCode/guiMeshPage.cpp \
    guiCode/guiMainWindow.cpp \
    guiCode/guiGLwidget.cpp \
    guiCode/guiGeometryPage.cpp \
    guiCode/guiFace.cpp \
    guiCode/guiConvergencePlot.cpp \
    guiCode/guiControlPointPlot.cpp \
    guiCode/guiBlockFacePage.cpp \
    guiCode/guiBlock.cpp \
    guiCode/guiBCICPage.cpp \
    solverCode/slvVmomMatrixFormer.cpp \
    solverCode/slvUmomMatrixFormer.cpp \
    solverCode/slvTotalMemoryAllocate.cpp \
    solverCode/slvTimeMarchingApproach.cpp \
    solverCode/slvSolverThread.cpp \
    solverCode/slvScalarVariableMatrixFormer.cpp \
    solverCode/slvResidualCalculator.cpp \
    solverCode/slvReadInputFile.cpp \
    solverCode/slvPseudoVelocityCalculator.cpp \
    solverCode/slvPressureVelocityCorrector.cpp \
    solverCode/slvPressureCorrection.cpp \
    solverCode/slvMemoryAllocater2D.cpp \
    solverCode/slvMainSolver.cpp \
    solverCode/slvLineTDMA.cpp \
    solverCode/slvDiffusiveConducCalc.cpp \
    solverCode/slvConvecFluxCalc.cpp \
    solverCode/slvCollocatedGridTransformer.cpp \
    solverCode/slvBoundaryCondition.cpp \
    solverCode/slvCoordinateCreator.cpp \
    solverCode/slvWriteOutputFiles.cpp
RESOURCES += forms/resources.qrc
QT += opengl
