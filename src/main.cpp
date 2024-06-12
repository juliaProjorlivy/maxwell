#include <stdio.h>

#include "raylib.h"

#include "verror.h"
#include "particles.h"

int main()
{
    InitWindow(BoxWidth + PlotWidth, BoxHeight, "Maxwell");

    Vector2 BoxSize = {.x = BoxWidth, .y = BoxHeight};

    particle **Particles = CreateParticles();
    if(!Particles)
    {
        VERROR("something went wrong");
        return 1;
    }

    int rangeNumber = Partitions;
    Vector2 zoomCoef = {1., 1.};
    int shouldStop = 1;                 // stop program checkup
    while(!WindowShouldClose())
    {
        if(IsKeyPressed(KEY_SPACE))
        {
            shouldStop = shouldStop ? 0 : 1;
            printf("is pressed\n");
        }

        BeginDrawing();
        if(!shouldStop)
        {
            ClearBackground(WHITE);
            DrawLineBezier({BoxSize.x, 0}, {BoxSize.x, BoxSize.y}, 1, BLACK);
            DrawLineBezier({0, BoxSize.y}, {BoxSize.x, BoxSize.y}, 1, BLACK);
            DrawParticles(Particles);
        }

        if(Plot(Particles, &zoomCoef, &rangeNumber))
        {
            VERROR("something went wrong");
            return 1;
        }

        EndDrawing();
    }

    DeleteParticles(Particles);
    return 0;
}
