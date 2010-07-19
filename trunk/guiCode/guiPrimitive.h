// This is the interface for the classes Primitive, Line and CircularArc.

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include <QColor>
#include "guiTypedefs.h"
#include "../fparserCode/fparser.h"

using namespace std;




class Primitive
{
public:
   Primitive();
   ~Primitive();
   void setType(primitiveTypes value)             {type = value;}
   void setColor(QColor value)                    {color = value;}
   void setThickness(float value)                 {thickness = value;}
   void setIsDeleted(bool value)                  {isDeleted = value;}
   void setIsBCset(bool value)                    {isBCset = value;}
   void setNumberOfEnteredPoints(int value)       {nEnteredPoints = value;}
   void setNumberOfDefPoints(int value)           {nDefPoints = value;}
   void setNumberOfPoints(int value)              {nPoints = value;}
   void setClusterCoeff(float value)              {clusterCoeff = value;}
   void setClusterType(clusterTypes value)        {clusterType = value;}
   void setBCtype(int value)                      {BCtype = value;}
   void setBCstring(int i, string value)          {BCstring[i] = value;}
   void setEnteredPointCoor(float* &coor);
   void setLineDefPointCoor();
   void setFunction(string value)                 {function = value;}
   void setIsCW(bool value)                       {isCW = value;}
   void setUsesCenter(bool value)                 {usesCenter = value;}
   void setAttachedBlock(int i, int b)            {attachedBlock[i] = b;}
   void setAttachedFace(int i, char c)            {attachedFace[i] = c;}
   void setIsThereSecondBlock(bool value)         {isThereSecondBlock = value;}

   primitiveTypes getType()          {return type;}
   QColor getColor()                 {return color;}
   float getThickness()              {return thickness;}
   bool getIsDeleted()               {return isDeleted;}
   bool getIsBCset()                 {return isBCset;}
   int getNumberOfEnteredPoints()    {return nEnteredPoints;}
   int getNumberOfDefPoints()        {return nDefPoints;}
   int getNumberOfPoints()           {return nPoints;}
   float getClusterCoeff()           {return clusterCoeff;}
   clusterTypes getClusterType()     {return clusterType;}
   int getBCtype()                   {return BCtype;}
   string* getBCstring()             {return BCstring;}    // This function returns a pointer to a string array (Sawitch 427)
   float* getEnteredPointCoor()      {return enteredPointCoor;}
   float* getDefPointCoor()          {return defPointCoor;}
   float* getPointCoor()             {return pointCoor;}
   bool getIsCW()                    {return isCW;}        // Used for circular arcs only.
   bool getUsesCenter()              {return usesCenter;}  // Used for circular arcs only.
   string getFunction()              {return function;}    // Used for curves defined by a function only.
   int* getAttachedBlock()           {return attachedBlock;}
   char* getAttachedFace()           {return attachedFace;}
   bool getIsThereSecondBlock()      {return isThereSecondBlock;}

   void allocateEnteredPointCoor();
   void allocateDefPointCoor();
   void allocatePointCoor();
   void deallocateEnteredPointCoor();
   void deallocateDefPointCoor();
   void deallocatePointCoor();
   
   void calculateArcDefPointCoor();
   void calculateFunctionDefPointCoor();
   void deletePrimitive();
   void calculateBoundaryNodes();

   // cuneyt: Is it more appropriate for attachedBlock, attachedFace and isThereSecondBlock variable not be here ?

private:
   primitiveTypes type;       // Type of the geometric primitive, LINE, ARC, etc.
   QColor color;              // Color of the geometric primitive.
   float thickness;           // Thickness of the geometric primitive.
   bool isDeleted;            // TRUE for a deleted geometric primitive.
   bool isBCset;              // TRUE if a BC is set for this primitive.

   int nEnteredPoints;        // Number of points used entered by the user, e.g. 2 for a line, 3 for an arc.
   int nDefPoints;            // Number of points used to define the primitive, e.g. 2 for a line, 50 for an arc.
   int nPoints;               // Number of boundary points generated on the primitive. Default value is zero.

   float *enteredPointCoor;   // x and y coordinates of the entered points. Stored as a one dimensional array (x0, y0, x1, y1, x2, y2, ...)
   float *defPointCoor;       // x and y coordinates of the definition points. Stored as a one dimensional array (x0, y0, x1, y1, x2, y2, ...)
   float *pointCoor;          // x and y coordinates of the boundary points. Stored as a one dimensional array (x0, y0, x1, y1, x2, y2, ...)

   int attachedBlock[2];      // Block numbers that this geometric primitive is used for. An primitive can be attached to at most 2 blocks. Default is -1.
   char attachedFace[2];      // Face types that this geometric primitive is used for, can be S, E, N, W. Default is 'A'.
   bool isThereSecondBlock;   // TRUE if this geometric primitive is attached to two blocks.

   float clusterCoeff;        // Cluster coefficient used to squeeze the boundary points on a geometric primitive. 1.0 means equi-spaced points.
   clusterTypes clusterType;  // Clustering function that will be applied.
   int BCtype;                // 1: wall, 2: inflow, 3: outflow
   string BCstring[3];        // u, v and p BCs for the primitive. Will later be converted to numbers through a function parser.

   // The following two are used for circular arcs only.
   bool isCW;                 // Indicates the direction of the arc, as CW or not.
   bool usesCenter;           // Indicates whether the arc is defined by "two points and center" or "three points".

   // The following is used for curves defined as functions only.
   string function;           // Function entered by the user.
};



/* These derived classes are not currenty in use

class Line : public Primitive
{
public:
   Line() {}
   ~Line() {}
   static bool isValid(float x1, float x2, float y1, float y2);
private:

};


class CircularArc : public Primitive
{
public:
   CircularArc() {}
   ~CircularArc() {}
   void setIsCW(bool newIsCW)             { isCW = newIsCW;}
   void setUsesCenter(bool newUsesCenter) { usesCenter = newUsesCenter;}
   bool getIsCW()                         { return isCW;}
   bool getUsesCenter()                   { return usesCenter;}
private:
   bool isCW;			// Indicates the direction of the arc, as CW or not.
   bool usesCenter;	// Indicates whether the arc is defined by "two points and center" or "three points".
};
*/

#endif
