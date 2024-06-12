#ifndef PARTICLES_H
#define PARTICLES_H

#include "raylib.h"

static const float epsilon = 1e-1;          // error to prevent collision overlap beforehand
static const float startVelocity = 100.;    // starting Velocity for all particles

static const int N0 = 500;                  // number of particles
static const int r0 = 10;                   // radius of particles
static const int BoxHeight = 800;           // box height
static const int BoxWidth = 810;            // box width
static const double defaultError = 0.003;  // stop condition

typedef struct
{
    Vector2 pos;    // particle position (x, y)
    Vector2 v;      // particle Velocity (v_x, v_y)
} particle;

particle *CreateParticle(Vector2 Pos);

particle **CreateParticles(int NParticles = N0, Vector2 BoxSize = {BoxWidth, BoxHeight}, float radius = r0, float v0 = startVelocity);

void TwoParticlesCollision(particle *Particle1, particle *Particle2);

void WallCollision(particle *Particle, Vector2 BoxSize = {BoxWidth, BoxHeight});

void ChangePosition(particle *Particle, float frametime);

void DrawParticles(particle **Particles, int NParticles = N0, Vector2 BoxSize = {BoxWidth, BoxHeight});

void DeleteParticles(particle **Particles, int NParticles = N0);

static const int Partitions = 3000;          // division of the segment [0, (v_max)^2]
static const int PlotWidth = 400;           // width of the first plot

int Plot(particle **Particles, Vector2 *zoomCoef, int *Partition, double *error, int NParticles = N0, Vector2 BoxSize = {BoxWidth, BoxHeight});


#endif

