#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include "guiTypedefs.h"
#include "guiStructuredMesh.h"

using namespace std;




struct Streamline {
   int nPoints;
   float **coor;
};




class Problem
{
public:
   Problem();
   ~Problem();
   void setMainState(mainStates newState)              {mainState = newState;}
   void setGlMouseState(glMouseStates newState)        {glMouseState = newState;}
   void setvisualizeState(visualizeStates newState)    {visualizeState = newState;}
   void setDrawingWhat(primitiveTypes newDrawingWhat)  {drawingWhat = newDrawingWhat;}
   void setDir(string newDir)                          {dir = newDir;}
   void setName(string newName)                        {name = newName;}
   void setFileNames(string s1, string s2);
   void setResultFileName(string newName)              {resultFileName = newName;}
   void setNparameters(int newnParameters)             {nParameters = newnParameters;}
   void setMeshGenerationAlgorithm(int newAlgorithm)   {meshGenerationAlgorithm = newAlgorithm;}
   void setICstring(int i, string newString)           {ICstring[i] = newString;}

   void setScheme(int newScheme)                       {scheme = newScheme;}
   void setDiscretization(int newDiscretization)       {discretization = newDiscretization;}
   void setStrategy(int newStrategy)                   {strategy = newStrategy;}
   void setTimeStep(int newTimeStep)                   {timeStep = newTimeStep;}
   void setKinvis(float newKinvis)                     {kinvis = newKinvis;}
   void setDensity(float newDensity)                   {density = newDensity;}
   void setRelaxation(int i, float newRelaxation)      {relaxations[i] = newRelaxation;}
   void setOuterTolerance(float newOuterTolerance)     {outerTolerance = newOuterTolerance;}
   void setMaxOuterIter(int newMaxOuterIter)           {maxOuterIter = newMaxOuterIter;}
   void setOutputInterval(int newOutputInterval)       {outputInterval = newOutputInterval;}
   void setPlotUpdateInterval(int newPlotUpdateInterval)   {plotUpdateInterval = newPlotUpdateInterval;}
   void setIsRestart(bool newIsRestart)                {isRestart = newIsRestart;}
   void setIsTecplot(bool newIsTecplot)                {isTecplot = newIsTecplot;}
   void setIsTimeDependent(bool newTimeDependent)      {isTimeDependent = newTimeDependent;}

   mainStates getMainState()           {return mainState;}
   glMouseStates getGlMouseState()     {return glMouseState;}
   visualizeStates getVisualizeState() {return visualizeState;}
   primitiveTypes getDrawingWhat()     {return drawingWhat;}
   string getDir()                     {return dir;}
   string getName()                    {return name;}
   string getCfdFileName()             {return cfdFileName;}
   string getInputFileName()           {return inputFileName;}
   string getResultFileName()          {return resultFileName;}
   int getMeshGenerationAlgorithm()    {return meshGenerationAlgorithm;}
   int getNparameters()                {return nParameters;}

   int getScheme()              {return scheme;}
   int getDiscretization()      {return discretization;}
   int getStrategy()            {return strategy;}
   float getTimeStep()          {return timeStep;}
   float getKinvis()            {return kinvis;}
   float getDensity()           {return density;}
   float getRelaxation(int i)   {return relaxations[i];}
   float getOuterTolerance()    {return outerTolerance;}
   int getMaxOuterIter()        {return maxOuterIter;}
   int getOutputInterval()      {return outputInterval;}
   int getPlotUpdateInterval()  {return plotUpdateInterval;}
   bool getIsRestart()          {return isRestart;}
   bool getIsTecplot()          {return isTecplot;}
   bool getIsTimeDependent()    {return isTimeDependent;}

   StructuredMesh *mesh;

   bool needToSave;
   
   // Cuneyt: The following are about the visualization of the results. They can be put into a separate struct or class.
   double **uResult;   // u results on the colocated mesh. Used in visualization
   double **vResult;   // v results on the colocated mesh. Used in visualization
   double **pResult;   // p results on the colocated mesh. Used in visualization
   bool showBoundary;  // True if the related check box of the visualization page is checked.
   bool showMesh;      // True if the related check box of the visualization page is checked.
   bool showContour;   // True if the related check box of the visualization page is checked.
   bool showStreamline;// True if the related check box of the visualization page is checked.
   int contourVar;     // 0: u velocity, 1: v velocity, 2: pressure
   int contourStyle;   // 0: Continuous flood, 1: Average cell flood
   int nContourLevels; // Number of contour levels.
   int colorMap;       // 0: Black and white, 1: Small rainbow
   int probeType;      // 1: Probe at position, 2: Probe at index
   double probeResultX, probeResultY, probeResultU, probeResultV, probeResultP;
   int nStreamlines;   // Number of streamlines that are shown during visualization.
   Streamline streamlines[MAX_STREAMLINES];

private:
   string name;            // Name of the problem. Files associated with this problem will include this name
   string dir;             // Directory of the problem.
   string cfdFileName;     // cfdFile has the whole problem data. It has the extension .CFD
   string inputFileName;   // inputFile has the whole problem data ready to be read by the solver. It has the extension .INP
   string resultFileName;  // resultFile has the solution at the cell corners. It has the extension .DAT

   string ICstring[3];     // Hold ICs for u, v and p. Will later be converted to numbers through a function parser.

   // States
   mainStates mainState;
   glMouseStates glMouseState;
   visualizeStates visualizeState;
   primitiveTypes drawingWhat;
   
   int meshGenerationAlgorithm;  // 0: TFI Linear, 1: ..... Cuneyt: Complete this

   // Solver parameters
   int nParameters;           // Number of parameters. cuneyt: Bu bir ise yaramiyor.
   int scheme;                // 0: SIMPLE, 1: SIMPLEC, 2: SIMPLER
   int discretization;        // 0: Central, 1: Upwind, 2: Hybrid, 3: Power law
   int strategy;              // 0: Iterative, 1: Time marching
   float timeStep;            // (Pseudo) time step
   float kinvis;              // Kinematic viscosity of the fluid (m2/s)
   float density;             // Density of the fluid (kg/m3)
   float relaxations[4];      // u, v, p and scalar relaxation parameters
   float outerTolerance;      // Convergence tolerance for outer iterations
   int maxOuterIter;          // Maximum number of outer iterations
   int outputInterval;        // Take an output at every *** iterations
   int plotUpdateInterval;    // Update convergence and control point plots at every ...... iterations
   bool isRestart;            // Set TRUE to start from a previous solution
   bool isTecplot;            // Set TRUE to generate Tecplot outputs
   bool isTimeDependent;      // Set TRUE to solve time dependent flows
};

#endif
