#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "particles.h"
#include "verror.h"

// Create particle with random velocity direction and with the given position value 
particle *CreateParticle(Vector2 Pos)
{
    particle *Particle = (particle *)calloc(sizeof(particle), 1);
    if(!Particle)
    {
        VERROR_MEM;
        return NULL;
    }

    float angle = (float)GetRandomValue(0, 359);
    *Particle = {.pos = Pos, .v = {startVelocity * cos(angle), startVelocity * sin(angle)}};

    return Particle;
}

// Create an array of evenly spreaded particles
particle **CreateParticles(int NParticles, Vector2 BoxSize)
{
    particle **Particles = (particle **)calloc(sizeof(particle *), NParticles);
    if(!Particles)
    {
        VERROR_MEM;
        return NULL;
    }

    int Nw = (int)(BoxSize.x / (2 * r0));
    int Nh = (int)(BoxSize.y / (2 * r0));

    int Ncolumns = 1, Nstrings = 1;
    float ax = 1, ay = 1;

    if(Nw >= NParticles)
    {
        Ncolumns = NParticles;
        Nstrings = 1;

        ax = BoxSize.x / (2. * NParticles);
        ay = BoxSize.y / 2.;
    }
    else if(Nh >= NParticles)
    {
        Ncolumns = 1;
        Nstrings = NParticles;

        ax = BoxSize.x / 2.;
        ay = BoxSize.y / (2. * NParticles);
    }
    else
    {
        Ncolumns = Nw;
        Nstrings = Nh;

        ax = BoxSize.x / (2. * Nw);
        ay = BoxSize.y / (2. * Nh);
    }

    for(int i = 0; i < Nstrings; i++)
    {
        int N = i * Ncolumns;
        float Y = ay * ((i << 1) + 1);
        for(int j = 0; (j < Ncolumns) && (N + j < NParticles); j++)
        {
            particle *Particle = CreateParticle({ax * ((j << 1) + 1), Y});
            if(!Particle)
            {
                VERROR_MEM;
                return NULL;
            }
            Particles[N + j] = Particle;
        }
    }
    return Particles;
}

// change of velocity and coordinates when two particles collide
void ParticleCollision(particle *Particle1, particle *Particle2)
{
    Vector2 delta = Vector2Subtract(Particle2->pos, Particle1->pos);
    float SquareSum = Vector2DotProduct(delta, delta);
    float DCollision = sqrt(SquareSum);
    if(SquareSum <= 4 * r0 * r0)
    {
        Vector2 e = {delta.x / DCollision, delta.y / DCollision};
        Vector2 vector = Vector2Scale(e, Vector2DotProduct(Vector2Subtract(Particle1->v,  Particle2->v), e));
        Particle1->v = Vector2Subtract(Particle1->v, vector);
        Particle2->v = Vector2Add(Particle2->v, vector);

        // overlaping problem
        float delta = (2. * r0 - DCollision) * 0.5;
        Particle1->pos = Vector2Subtract(Particle1->pos, Vector2Scale(e, delta));
        Particle2->pos = Vector2Add(Particle2->pos, Vector2Scale(e, delta));
    }
}

// sort by x axis 
int xAxisSort(const void *p1, const void *p2)
{
    const particle *p11 = *(const particle **)p1;
    const particle *p22 = *(const particle **)p2;

    return p11->pos.x > p22->pos.x ? 1 : -1;
}

void SortParticles(particle **Particles, int NParticles)
{
    qsort(Particles, NParticles, sizeof(particle *), xAxisSort);
}

void AllParticlesCollision(particle **Particles, int NParticles)
{
    SortParticles(Particles, NParticles);

    for(int i = 0; i < NParticles - 1; i++)
    {
        for(int j = i + 1; j < NParticles; j++)
        {
            if((Particles[j]->pos.x - r0) < (Particles[i]->pos.x + r0))
            {
                ParticleCollision(Particles[i], Particles[j]);
            }
        }
    }
}

// change velocity when wall wall collision happens
void WallCollision(particle *Particle, Vector2 BoxSize)
{
    // left wall
    if(Particle->pos.x < (r0 + epsilon)) 
    { 
        Particle->pos.x = r0; 
        Particle->v.x *= -1.; 
    } 
    // right wall
    else if(Particle->pos.x > (BoxSize.x - r0 - epsilon))
    { 
        Particle->pos.x = BoxSize.x - r0; 
        Particle->v.x *= -1.; 
    } 
    // upper wall
    if(Particle->pos.y < (r0 + epsilon)) 
    { 
        Particle->pos.y = r0; 
        Particle->v.y *= -1.; 
    }
    // lower wall
    else if(Particle->pos.y > (BoxSize.y - r0 - epsilon)) 
    { 
        Particle->pos.y = BoxSize.y - r0; 
        Particle->v.y *= -1.; 
    } 
}

// change coordinates according to the formula dx = v_x * dt
void ChangePosition(particle *Particle, float frametime)
{
    Particle->pos = Vector2Add(Particle->pos, Vector2Scale(Particle->v, frametime));
}

// draw particles considering collisions
void DrawParticles(particle **Particles, int NParticles, Vector2 BoxSize)
{
    for(int i = 0; i < NParticles; i++)
    {
        particle *Particle = Particles[i];
        // AllParticlesCollision(Particles, NParticles);
        for(int j = i + 1; j < NParticles; j++)
        {
            ParticleCollision(Particles[i], Particles[j]);
        }
        ChangePosition(Particle, GetFrameTime());
        WallCollision(Particle, BoxSize);
        DrawCircle((int)(Particle->pos.x), (int)(Particle->pos.y), r0, BLUE);
    }
}

void DeleteParticles(particle **Particles, int NParticles)
{
    for(int i = 0; i < NParticles; i++)
    {
        free(Particles[i]);
    }
    free(Particles);
}


