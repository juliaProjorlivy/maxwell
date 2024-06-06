#ifndef PARTICLES_H
#define PARTICLES_H

#include "raylib.h"

static const float epsilon = 1e-3;
static const float startVelocity = 50.;


static const int N0 = 500;             // number of particles
static const int r0 = 10;             // radius of particles
static const int WindowHeight = 800;   // box height
static const int WindowWidth = 1000;   // box width

typedef struct
{
    Vector2 pos;
    Vector2 v;
} particle;

particle *CreateParticle(Vector2 Pos);

particle **CreateParticles(int NParticles, Vector2 WindowSize);

void ParticleCollision(particle *Particle1, particle *Particle2);

void WallCollision(particle *Particle);

void ChangePosition(particle *Particle, float frametime);

void DrawParticles(particle **Particles, int NParticles);

void DeleteParticles(particle **Particles, int NParticles);

int xAxisSort(const void * p1, const void * p2);

void SortParticles(particle **Particles, int NParticles);

void AllParticlesCollision(particle **Particles, int NParticles);

#endif

