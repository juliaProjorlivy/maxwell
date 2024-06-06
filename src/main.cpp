#include <stdio.h>
#include <math.h>
#include <float.h>

#include "raylib.h"
#include "raymath.h"

#include "verror.h"
#include "particles.h"
#include "plotting.h"

//DEBUGINFO
void DrawVelocity(particle **Particles, int NParticles)
{
    float sum = 0;
    for(int i = 0; i < NParticles; i++)
    {
        sum += Vector2DotProduct(Particles[i]->v, Particles[i]->v);
    }
    printf("Velocity SquareSum = %f\n", sum);
}

void PrintInfo(particle **Particles, int NParticles)
{
    SortParticles(Particles, NParticles);
    for(int i = 0; i < NParticles; i++)
    {
        printf("%d. VELOCITY: %f | x = %f\n", i, sqrt(Vector2DotProduct(Particles[i]->v, Particles[i]->v)), Particles[i]->pos.x);
    }
    printf("---------------------\n");
}


int main()
{
    InitWindow(WindowWidth, WindowHeight, "Maxwell");

    if((int)(WindowHeight / (2 * r0)) * (int)(WindowWidth / (2 * r0)) < N0)
    {
        VERROR("Size error too many particles");
        return 1;
    }
    Vector2 WindowSize = {.x = WindowWidth, .y = WindowHeight};

    particle **Particles = CreateParticles(N0, WindowSize);
    if(!Particles)
    {
        VERROR_MEM;
        return 1;
    }

    int flag = 1;
    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_SPACE))
        {
            flag = flag ? 0 : 1;
            printf("is pressed\n");
        }
        BeginDrawing();
        if(!flag)
        {
        ClearBackground(WHITE);
        DrawParticles(Particles, N0);
        // Plot(Particles, N0);
        // PrintInfo(Particles, N0);       // DEBUGINFO DELETE!!!   
        // DrawVelocity(Particles, N0);    // DEBUGINFO DELETE!!!
        }

        Plot(Particles, N0);
        EndDrawing();
        // flag = 1;
    }

    DeleteParticles(Particles, N0);
    printf("MAX FLOAT = %f\n", FLT_MAX);
    return 0;
}
