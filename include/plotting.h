#ifndef PLOTTING_H
#define PLOTTING_H

static const int Partition = 500;           // division of the segment [0, (v_max)^2]
static const float epsVelocity = 20.;       // considering floating point representation
static const int PlotWidth = 400;           // width of the plot

void Plot(particle **Particles, int NParticles, Vector2 BoxSize, float epsV = epsVelocity);

#endif
