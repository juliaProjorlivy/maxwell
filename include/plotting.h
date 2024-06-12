#ifndef PLOTTING_H
#define PLOTTING_H

#include "particles.h"
static const int Partitions = 500;          // division of the segment [0, (v_max)^2]
static const int PlotWidth = 400;           // width of the first plot

int Plot(particle **Particles, Vector2 *zoomCoef, int *Partition, int NParticles = N0, Vector2 BoxSize = {BoxWidth, BoxHeight});

#endif
