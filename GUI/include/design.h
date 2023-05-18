#ifndef DESIGN_H // le header guard
#define DESIGN_H 

void designTuningFork(double r1, double r2, double e, double l, double meshSizeFactor, char * filename);

void designTuningForkMultipleFrequencies(double r1, double r2, double e, double l, double r3, double r4, double l2, double meshSizeFactor, char * filename);

#endif