// This is the implementation file primitive.cpp.

#include "guiPrimitive.h"
#include "guiTypedefs.h"
#include <QColor>
#include <math.h>




Primitive::Primitive(void)
{
	type = NOPRIMITIVE;
	color = Qt::black;
	thickness = 2.0;
	isDeleted = TRUE;
   isBCset = FALSE;
	clusterType = NONE;
	clusterCoeff = 1.00;
	BCtype = 1;		// Default value is 1 (Wall type boundary)
	BCstring[0] = BCstring[1] = BCstring[2] = "";
	isCW = TRUE;
	usesCenter = TRUE;   // Used for circular arcs only.
	function = " ";

	attachedBlock[0] = 0;
	attachedBlock[1] = 0;
	attachedFace[0] = 'A';   // cuneyt: Bu 'A' larin bir onemi var mi? Kullaniliyorlar mi?
	attachedFace[1] = 'A';
	isThereSecondBlock = FALSE;

	nEnteredPoints = 0;
	nDefPoints = 0;
	nPoints = 0;

	enteredPointCoor = NULL;
	defPointCoor = NULL;
	pointCoor = NULL;
}




Primitive::~Primitive(void)
{
	deallocateEnteredPointCoor();
	deallocateDefPointCoor();
	deallocatePointCoor();
}




void Primitive::allocateEnteredPointCoor()
{
	enteredPointCoor = new float[2*nEnteredPoints];
}




void Primitive::allocateDefPointCoor()
{
	defPointCoor = new float[2*nDefPoints];
}




void Primitive::allocatePointCoor()
{
	pointCoor = new float[2*nPoints];
}




void Primitive::deallocateEnteredPointCoor()
{
   if (enteredPointCoor == NULL) return;
	delete[] enteredPointCoor;
	enteredPointCoor = NULL;
}




void Primitive::deallocateDefPointCoor()
{
   if (defPointCoor == NULL) return;
	delete[] defPointCoor;
	defPointCoor = NULL;
}




void Primitive::deallocatePointCoor()
{
   if (pointCoor == NULL) return;
	delete[] pointCoor;
	pointCoor = NULL;
}




void Primitive::setEnteredPointCoor(float* &coor)
{
	for (int i = 0; i < nEnteredPoints; i++)
	{
		enteredPointCoor[2*i] = coor[2*i];
		enteredPointCoor[2*i+1] = coor[2*i+1];
	}
} 




void Primitive::setLineDefPointCoor()
{
	for (int i = 0; i < nDefPoints; i++)
	{
		defPointCoor[2*i] = enteredPointCoor[2*i];
		defPointCoor[2*i+1] = enteredPointCoor[2*i+1];
	}
}




void Primitive::calculateArcDefPointCoor()
{
	float angle, angle1, angle2, angleArc;
	float x1, y1, x2, y2, xC, yC, radius;
	
	if (usesCenter) {
		// Setup first and last point of the arc
		defPointCoor[0] = enteredPointCoor[0];
		defPointCoor[1] = enteredPointCoor[1];
		defPointCoor[2*(nDefPoints-1)] = enteredPointCoor[2];
		defPointCoor[2*(nDefPoints-1)+1] = enteredPointCoor[3];
	
		// Define some shotcuts
		x1 = enteredPointCoor[0];  // First point of the arc
		y1 = enteredPointCoor[1];
		x2 = enteredPointCoor[2];  // Last point of the arc
		y2 = enteredPointCoor[3];
		xC = enteredPointCoor[4];  // Center of the arc
		yC = enteredPointCoor[5];
	} else {
		// cuneyt: Burasi eksik. Uc noktasi verilen arc 
	}
	
	// Calculate the radius of the arc
	radius = sqrt( (x1-xC)*(x1-xC) + (y1-yC)*(y1-yC) );
	
	// The following angles are between -PI and PI.
	angle1 = atan2(y1-yC, x1-xC);
	angle2 = atan2(y2-yC, x2-xC);
	
	// Convert these angles so that they are between 0 and 2*PI.
	if (angle1 < 0)  angle1 += 2*PI;
	if (angle2 < 0)  angle2 += 2*PI;

	// Calculate the angle of the circular arc.
	if (angle1 < angle2 && isCW == FALSE)
		angleArc = angle2 - angle1;
	else if (angle1 < angle2 && isCW)
		angleArc = 2* PI - (angle2 - angle1);
	else if (angle1 > angle2 && isCW == FALSE)
		angleArc = 2* PI - (angle1 - angle2);
	else if (angle1 > angle2 && isCW)
		angleArc = angle1 - angle2;
   else if (angle1 == angle2)    // Full circle.  Cuneyt: Will this be allowed ?
		angleArc = 2*PI;

	// Generate the points that will define the arc.
	if (isCW) {
		for (int j = 1; j < nDefPoints - 1; j++) {
			angle = angle1 - j * angleArc / (nDefPoints - 1);
			defPointCoor[2*j] = xC + radius * cos(angle);
			defPointCoor[2*j+1] = yC + radius * sin(angle);
		}
	} else {
		for (int j = 1; j < nDefPoints - 1; j++) {
			angle = angle1 + j * angleArc / (nDefPoints - 1);
			defPointCoor[2*j] = xC + radius * cos(angle);
			defPointCoor[2*j+1] = yC + radius * sin(angle);
		}
	}
}  // End of function calculateArcDefPointCoor()




void Primitive::calculateFunctionDefPointCoor()
{
	float x, y, dx, x1, y1, x2, y2;

	FunctionParser fparser;     // Function parser object
	double funcVals[1];

	// Setup first and last point of the curve
	defPointCoor[0] = enteredPointCoor[0];
	defPointCoor[1] = enteredPointCoor[1];
	defPointCoor[2*(nDefPoints-1)] = enteredPointCoor[2];
	defPointCoor[2*(nDefPoints-1)+1] = enteredPointCoor[3];
	
	// Define some shotcuts
	x1 = enteredPointCoor[0];  // First point of the curve
	y1 = enteredPointCoor[1];
	x2 = enteredPointCoor[2];  // Last point of the curve
	y2 = enteredPointCoor[3];

	dx = (x2-x1)/(FUNCTION_DEF_POINTS-1);

	// Generate the points that will define the curve
	for (int j = 1; j < nDefPoints - 1; j++) {
		x = x1 + j*dx;
		funcVals[0] = x;
		fparser.Parse(function, "x");
		y = fparser.Eval(funcVals);

		defPointCoor[2*j] = x;
		defPointCoor[2*j+1] = y;
	}
}  // End of calculateFunctionDefPointCoor




void Primitive::deletePrimitive()
{
	type = NOPRIMITIVE;
	color = Qt::black;
	thickness = 2.0;
	isDeleted = TRUE;
   isBCset = FALSE;
	clusterType = NONE;
	clusterCoeff = 1.00;
	BCtype = 1;		// default value is 1 (Wall type boundary)
	BCstring[0] = BCstring[1] = BCstring[2] = "";
	isCW = TRUE;
	usesCenter = TRUE;

	attachedBlock[0] = 0;
	attachedBlock[1] = 0;
	attachedFace[0] = 'A';
	attachedFace[1] = 'A';
	isThereSecondBlock = FALSE;

	deallocateEnteredPointCoor();
	deallocateDefPointCoor();
	deallocatePointCoor();

	nEnteredPoints = 0;
	nDefPoints = 0;
	nPoints = 0;
}  // End of deletePrimitive




void Primitive::calculateBoundaryNodes()
{
  int i, j, ii;
  float dL1, dL2;
  float *distanceOfDefPoints, *distanceOfPoints;
  float x1, x2, y1, y2;
  float clustCoeff;
  double dx, dy, length, MAX, xx, xxx;
  bool clusterAtTheEnd;

  // Delete the old pointCoor array and allocate a new one.
  deallocatePointCoor();
  allocatePointCoor();

  if (clusterCoeff >= 0) {
    clusterAtTheEnd = FALSE;
    clustCoeff = clusterCoeff;
  } else {
    clusterAtTheEnd = TRUE;    // Cluster the nodes at the end of the primitive.
    clustCoeff = -1.0 * clusterCoeff;
  }

  distanceOfDefPoints = new float [nDefPoints];
  distanceOfPoints = new float [nPoints];

  // These are the end points of the curve defined by the FUNCTION
  x1 = enteredPointCoor[0];
  y1 = enteredPointCoor[1];
  x2 = enteredPointCoor[2];
  y2 = enteredPointCoor[3];

  // Calculate the total curve length.
  // Sum the line segments between defPointCoor's.
  // Also calculate the distance of def points from the starting point.
  length = 0.0;
  distanceOfDefPoints[0] = 0.0;
  for(i=0; i<nDefPoints-1; i++) {
    dx = defPointCoor[2*(i+1)] - defPointCoor[2*i];
	dy = defPointCoor[2*(i+1)+1] - defPointCoor[2*i+1];
	length = length + sqrt(dx*dx + dy*dy);

	distanceOfDefPoints[i+1] = length;
  }

  // Calculate the distance of boundary points from the starting point.
  distanceOfPoints[0] = 0.0;
  for(i=0; i<nPoints; i++) {
    if (clusterType == NONE || clusterCoeff == 0.0) {
	  distanceOfPoints[i] = i * length / (nPoints-1);
	} else {
	  xx = 1.0 * i / double(nPoints-1);           // 0 < xx < 1

	  if (clusterType == EXP) {
	    MAX = exp( clustCoeff * 1.0) - 1.0;
		xxx = exp(clustCoeff * xx) - 1.0;   // 0 < xxx < MAX
	  } else if (clusterType == SINH) {
		MAX = sinh( clustCoeff * 1.0);
		xxx = sinh(clustCoeff * xx);        // 0 < xxx < MAX
	  } else if (clusterType == TANH) {
		MAX = tanh( clustCoeff * 1.0);
		xxx = tanh(clustCoeff * xx);        // 0 < xxx < MAX
	  }

	  if (clusterAtTheEnd == FALSE && (clusterType == EXP || clusterType == SINH)
	      || (clusterAtTheEnd == TRUE && clusterType == TANH)) {  // This long if is necessary due to the clustering
					                                                  // characteristics of EXP and SINH vs that of TANH
	    distanceOfPoints[i] = length / MAX * xxx;
	  } else {
		ii = nPoints - 1 - i;
		distanceOfPoints[ii] = length - length / MAX * xxx;
	  }
	}
  }

  // Calculate the mid-boundary point coordinates.

  // First and last points are already known
  pointCoor[0] = x1;
  pointCoor[1] = y1;
  pointCoor[2*(nPoints-1)] = x2;
  pointCoor[2*(nPoints-1)+1] = y2;

  // Calculate mid-point coordinates
  for(i=1; i<nPoints-1; i++) {
    // Boundary point i lies between def points of (j-1) and (j). Find idp.
	for(j=1; j<nDefPoints; j++) {
     if (distanceOfPoints[i] <= distanceOfDefPoints[j]) break;
	}
	// Calcute the distance between (j-1) and (j)th def points.
	dL1 = distanceOfDefPoints[j] - distanceOfDefPoints[j-1];
	// Calcute the distance between (j-1) def point and (i)th boundary point.
	dL2 = distanceOfPoints[i] - distanceOfDefPoints[j-1];
	// Calculate the coordinates of boundary point i.
	pointCoor[2*i] = defPointCoor[2*(j-1)] + dL2/dL1 * (defPointCoor[2*j] - defPointCoor[2*(j-1)]);
	pointCoor[2*i+1] = defPointCoor[2*(j-1)+1] + dL2/dL1 * (defPointCoor[2*j+1] - defPointCoor[2*(j-1)+1]);
  }
  delete distanceOfDefPoints;
  delete distanceOfPoints;

}  // End of function calculateBoundaryNodes()

