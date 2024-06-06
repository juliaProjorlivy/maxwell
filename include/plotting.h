#ifndef PLOTTING_H
#define PLOTTING_H

static const int Partition = 10;


int VelocityCompare(const void *p1, const void *p2);

void SortByVelocity(particle **Particles, int NParticles);

void Plot(particle **Particles, int NParticles);

#endif
