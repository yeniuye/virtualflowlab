#ifndef TYPEDEFS_H
#define TYPEDEFS_H

// Used in PROBLEM class
typedef enum {NOPRIMITIVE, LINE, ARC, BEZIER, FUNCTION, COORDS, MODIFY, DIVIDE, DEL} primitiveTypes;
typedef enum {STRUCTURED, UNSTRUCTURED} meshTypes;
typedef enum {NONE, EXP, SINH, TANH} clusterTypes;


// Different states of the problem solution and GUI
typedef enum {GEOM, BLOCK, MESH, BCIC, PARAMETERS, BLOCKEDCELLS, CONTROLPOINTS, VISUALIZE} mainStates;
typedef enum {SELECT, ADDSTREAMLINE, PROBE, DATAEXTRACT} visualizeStates;
typedef enum {SELECTION, ZOOM} glMouseStates;  // Used by the openGL widget
//typedef enum {NOTHING, PRIMITIVESADDED, BLOCKFACESET, MESHGENERATED, SOLNSTARTED, SOLNPAUSED, SOLNFINISHED, POSTPROCESSDONE} whatsDoneState;
//typedef enum {NOTSTARTED, RUNNING, PAUSED, TERMINATED} solveStates;
//typedef enum {NODRAWING, MIDDLE, FINISHED} drawingStates;


// Constants
const int    MAX_PRIMITIVES          = 50;
const int    CIRCULAR_ARC_DEF_POINTS = 50;
const int    FUNCTION_DEF_POINTS     = 50;
const int    MAX_CONTROL_POINTS      = 10;
const int    MAX_STREAMLINES         = 100;
const double PI                      = 3.14159265358979323;

#endif
