#ifndef PARTICLES_H
#define PARTICLES_H

#include "raylib.h"

static const float epsilon = 1e-1;          // error to prevent collision overlap beforehand
static const float startVelocity = 800.;    // starting Velocity for all particles

static const int N0 = 100;                  // number of particles
static const int r0 = 10;                   // radius of particles
static const int BoxHeight = 500;           // box height
static const int BoxWidth = 810;            // box width

typedef struct
{
    Vector2 pos;    // particle position (x, y)
    Vector2 v;      // particle Velocity (v_x, v_y)
} particle;

particle *CreateParticle(Vector2 Pos);

particle **CreateParticles(int NParticles, Vector2 BoxSize);

void ParticleCollision(particle *Particle1, particle *Particle2);

void WallCollision(particle *Particle, Vector2 BoxSize);

void ChangePosition(particle *Particle, float frametime);

void DrawParticles(particle **Particles, int NParticles, Vector2 BoxSize);

void DeleteParticles(particle **Particles, int NParticles);

int xAxisSort(const void * p1, const void * p2);

void SortParticles(particle **Particles, int NParticles);

void AllParticlesCollision(particle **Particles, int NParticles);

#endif

