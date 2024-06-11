#ifndef PLOTTING_H
#define PLOTTING_H

static const int Partitions = 500;          // division of the segment [0, (v_max)^2]
static const float epsVelocity = 20.;       // considering floating point representation
static const int PlotWidth = 400;           // width of the first plot
static const int plotHeight = 400;          // height of the second plot

void Plot(particle **Particles, int NParticles, Vector2 BoxSize, float *zoomCoef, int *Partition, float epsV = epsVelocity);

#endif
