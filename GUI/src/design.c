
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmshc.h>
//#include "../gmsh-sdk/include/gmshc.h"

void designTuningFork(double r1, double r2, double e, double l, double meshSizeFactor, char * filename) {
  /**
   * r1 = inner radius (half-distance between prongs)
   * r2 = outer radius (half-width of fork)
   * e  = length of handle
   * l  = length of prongs
   * meshSizeFactor = meshSize / width of prongs
   * if `filename` is not NULL, save to file
  */
  
  int ierr;

  gmshClear(&ierr);

  double h = r2 - r1; // width of prongs
  double meshSize = h * meshSizeFactor;

  // Add points
  double x = 0;
  double y = 0;
  double z = 0;
  gmshModelOccAddPoint(x,y,z,meshSize,1,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,2,&ierr);
  y += e;
  gmshModelOccAddPoint(x,y,z,meshSize,3,&ierr);
  x += r2;
  y += r2;
  gmshModelOccAddPoint(x,y,z,meshSize,4,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,5,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,6,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,7,&ierr);
  x -= r1;
  y -= r1;
  gmshModelOccAddPoint(x,y,z,meshSize,8,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,9,&ierr);
  x -= r1;
  y += r1;
  gmshModelOccAddPoint(x,y,z,meshSize,10,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,11,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,12,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,13,&ierr);
  x += r2;
  y -= r2;
  gmshModelOccAddPoint(x,y,z,meshSize,14,&ierr);
  y += (h+r1);
  gmshModelOccAddPoint(x,y,z,meshSize,15,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,16,&ierr);
  
  // Add curves
  gmshModelOccAddLine(1,2,1,&ierr);
  gmshModelOccAddLine(2,3,2,&ierr);
  gmshModelOccAddCircleArc(3,16,4,3,&ierr);
  gmshModelOccAddLine(4,5,4,&ierr);
  gmshModelOccAddLine(5,6,5,&ierr);
  gmshModelOccAddLine(6,7,6,&ierr);
  gmshModelOccAddCircleArc(7,16,8,7,&ierr);
  gmshModelOccAddLine(8,9,8,&ierr);
  gmshModelOccAddCircleArc(9,15,10,9,&ierr);
  gmshModelOccAddLine(10,11,10,&ierr);
  gmshModelOccAddLine(11,12,11,&ierr);
  gmshModelOccAddLine(12,13,12,&ierr);
  gmshModelOccAddCircleArc(13,15,14,13,&ierr);
  gmshModelOccAddLine(14,1,14,&ierr);

  // Add wire (closed curve)
  int curveTags[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  gmshModelOccAddWire(curveTags, 14, 1, 1, &ierr);

  // Add surface
  int wireTags[1] = {1};
  gmshModelOccAddPlaneSurface(wireTags, 1, 100, &ierr);

  // Sync
  gmshModelOccSynchronize(&ierr);

  // Create physical group for surface
  int surfaceTags[1] = {100};
  gmshModelAddPhysicalGroup(2, surfaceTags, 1, -1, "bulk", &ierr);

  // Create physical group for clamped curves
  int clampedCurveTags[3] = {1, 2, 14};
  gmshModelAddPhysicalGroup(1, clampedCurveTags, 3, -1, "clamped", &ierr);

  gmshModelMeshGenerate(2, &ierr);

  if(filename != NULL) gmshWrite(filename, &ierr);
}

void designTuningForkMultipleFrequencies(double r1, double r2, double e, double l, double r3, double r4, double l2, double meshSizeFactor, char * filename) {
  /**
   * r1 = inner radius (half-distance between prongs)
   * r2 = outer radius (half-width of fork)
   * e  = length of handle
   * l  = length of prongs
   * meshSizeFactor = meshSize / width of prongs
   * if `filename` is not NULL, save to file
  */
  
  int ierr;

  gmshClear(&ierr);

  double h = r2 - r1; // width of prongs
  double h2 = r4 - r3; // width of the small prongs
  double meshSize = h * meshSizeFactor;

  // Add points
  double x = 0;
  double y = 0;
  double z = 0;
  gmshModelOccAddPoint(x,y,z,meshSize,1,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,2,&ierr);
  y += e;
  gmshModelOccAddPoint(x,y,z,meshSize,3,&ierr);
  x += r2;
  y += r2;
  gmshModelOccAddPoint(x,y,z,meshSize,4,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,5,&ierr);
  // Starts here

  x+=r4;
  y+=r4;
  gmshModelOccAddPoint(x,y,z,meshSize,17,&ierr);
  y+=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,18,&ierr);
  x-=h2;
  gmshModelOccAddPoint(x,y,z,meshSize,19,&ierr);
  y-=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,20,&ierr);
  x-=r3;
  y-=r3;
  gmshModelOccAddPoint(x,y,z,meshSize,21,&ierr);
  x-=h;
  gmshModelOccAddPoint(x,y,z,meshSize,22,&ierr);
  x-=r3;
  y+=r3;
  gmshModelOccAddPoint(x,y,z,meshSize,23,&ierr);
  y+=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,24,&ierr);
  x-=h2;
  gmshModelOccAddPoint(x,y,z,meshSize,25,&ierr);
  y-=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,26,&ierr);
  x+=h2;
  x+=r3;
  gmshModelOccAddPoint(x,y,z,meshSize,27,&ierr);
  x+=h;
  gmshModelOccAddPoint(x,y,z,meshSize,28,&ierr);
  y-=r3;
  y-=h2;
  x-=h;

  // Ends here
  gmshModelOccAddPoint(x,y,z,meshSize,6,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,7,&ierr);
  x -= r1;
  y -= r1;
  gmshModelOccAddPoint(x,y,z,meshSize,8,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,9,&ierr);
  x -= r1;
  y += r1;
  gmshModelOccAddPoint(x,y,z,meshSize,10,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,11,&ierr);
  // Starts here

  x+=r4;
  y+=r4;
  gmshModelOccAddPoint(x,y,z,meshSize,29,&ierr);
  y+=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,30,&ierr);
  x-=h2;
  gmshModelOccAddPoint(x,y,z,meshSize,31,&ierr);
  y-=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,32,&ierr);
  x-=r3;
  y-=r3;
  gmshModelOccAddPoint(x,y,z,meshSize,33,&ierr);
  x-=h;
  gmshModelOccAddPoint(x,y,z,meshSize,34,&ierr);
  x-=r3;
  y+=r3;
  gmshModelOccAddPoint(x,y,z,meshSize,35,&ierr);
  y+=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,36,&ierr);
  x-=h2;
  gmshModelOccAddPoint(x,y,z,meshSize,37,&ierr);
  y-=l2;
  gmshModelOccAddPoint(x,y,z,meshSize,38,&ierr);
  x+=h2;
  x+=r3;
  gmshModelOccAddPoint(x,y,z,meshSize,39,&ierr);
  x+=h;
  gmshModelOccAddPoint(x,y,z,meshSize,40,&ierr);
  y-=r3;
  y-=h2;
  x-=h;

  // Ends here
  gmshModelOccAddPoint(x,y,z,meshSize,12,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,13,&ierr);
  x += r2;
  y -= r2;
  gmshModelOccAddPoint(x,y,z,meshSize,14,&ierr);
  y += (h+r1);
  gmshModelOccAddPoint(x,y,z,meshSize,15,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,16,&ierr);
  
  // Add curves
  gmshModelOccAddLine(1,2,1,&ierr);
  gmshModelOccAddLine(2,3,2,&ierr);
  gmshModelOccAddCircleArc(3,16,4,3,&ierr);
  gmshModelOccAddLine(4,5,4,&ierr);
  // gmshModelOccAddLine(5,6,5,&ierr);
  gmshModelOccAddLine(6,7,6,&ierr);
  gmshModelOccAddCircleArc(7,16,8,7,&ierr);
  gmshModelOccAddLine(8,9,8,&ierr);
  gmshModelOccAddCircleArc(9,15,10,9,&ierr);
  gmshModelOccAddLine(10,11,10,&ierr);
  // gmshModelOccAddLine(11,12,11,&ierr);
  gmshModelOccAddLine(12,13,12,&ierr);
  gmshModelOccAddCircleArc(13,15,14,13,&ierr);
  gmshModelOccAddLine(14,1,14,&ierr);


  // Starts here
  gmshModelOccAddCircleArc(5,28,17,5,&ierr);
  gmshModelOccAddLine(17,18,17,&ierr);
  gmshModelOccAddLine(18,19,18,&ierr);
  gmshModelOccAddLine(19,20,19,&ierr);
  gmshModelOccAddCircleArc(20,28,21,20,&ierr);
  gmshModelOccAddLine(21,22,21,&ierr);
  gmshModelOccAddCircleArc(22,27,23,22,&ierr);
  gmshModelOccAddLine(23,24,23,&ierr);
  gmshModelOccAddLine(24,25,24,&ierr);
  gmshModelOccAddLine(25,26,25,&ierr);
  gmshModelOccAddCircleArc(26,27,6,26,&ierr);
  // Ends here

  // Starts here
  gmshModelOccAddCircleArc(11,40,29,11,&ierr);
  gmshModelOccAddLine(29,30,29,&ierr);
  gmshModelOccAddLine(30,31,30,&ierr);
  gmshModelOccAddLine(31,32,31,&ierr);
  gmshModelOccAddCircleArc(32,40,33,32,&ierr);
  gmshModelOccAddLine(33,34,33,&ierr);
  gmshModelOccAddCircleArc(34,39,35,34,&ierr);
  gmshModelOccAddLine(35,36,35,&ierr);
  gmshModelOccAddLine(36,37,36,&ierr);
  gmshModelOccAddLine(37,38,37,&ierr);
  gmshModelOccAddCircleArc(38,39,12,38,&ierr);
  // Ends here

  // Add wire (closed curve)
  int curveTags[34] = {1,2,3,4,5,17,18,19,20,21,22,23,24,25,26,6,7,8,9,10,11,29,30,31,32,33,34,35,36,37,38,12,13,14};
  gmshModelOccAddWire(curveTags, 34, 1, 1, &ierr);

  // Add surface
  int wireTags[1] = {1};
  gmshModelOccAddPlaneSurface(wireTags, 1, 100, &ierr);

  // Sync
  gmshModelOccSynchronize(&ierr);

  // Create physical group for surface
  int surfaceTags[1] = {100};
  gmshModelAddPhysicalGroup(2, surfaceTags, 1, -1, "bulk", &ierr);

  // Create physical group for clamped curves
  int clampedCurveTags[3] = {1, 2, 14};
  gmshModelAddPhysicalGroup(1, clampedCurveTags, 3, -1, "clamped", &ierr);

  gmshModelMeshGenerate(2, &ierr);

  if(filename != NULL) gmshWrite(filename, &ierr);
}