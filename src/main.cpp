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

    int Nframes = 0;
    int shouldStopProgram = 0;
    double error = 0;
    int rangeNumber = Partitions;
    Vector2 zoomCoef = {1., 1.};
    int shouldStopDrawing = 0;                 // stop drawing checkup
    while(!WindowShouldClose() && !shouldStopProgram)
    {
        if(IsKeyPressed(KEY_SPACE))
        {
            shouldStopDrawing = shouldStopDrawing ? 0 : 1;
            printf("is pressed\n");
        }

        BeginDrawing();
        if(!shouldStopDrawing)
        {
            ClearBackground(WHITE);
            DrawLineBezier({BoxSize.x, 0}, {BoxSize.x, BoxSize.y}, 1, BLACK);
            DrawLineBezier({0, BoxSize.y}, {BoxSize.x, BoxSize.y}, 1, BLACK);
            DrawParticles(Particles);
        }

        error = 0;
        if(Plot(Particles, &zoomCoef, &rangeNumber, &error))
        {
            VERROR("something went wrong");
            return 1;
        }

        if(error < defaultError && Nframes > 2)
        {
            printf("SquareMeanFault = %lf | relevant fault = %lf\ntime = %f\n", error, defaultError, GetTime());
            shouldStopProgram = 1;
        }

        Nframes++;
        EndDrawing();
    }

    CloseWindow();
    DeleteParticles(Particles);
    return 0;
}
