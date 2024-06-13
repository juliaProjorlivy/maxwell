#include <stdio.h>

#include "raylib.h"

#include "verror.h"
#include "particles.h"

// int InputHandle(int argc, char *argv[], float *n, float *v)
// {
//     switch (argc) 
//     {
//         case 1:
//             {
//                 return 0;
//             }
//         case 2:
//             {
//                 if(strlen(argv[1]) != 2)
//                 {
//                     VERROR("incorrect flag");
//                     return 1;
//                 }
//                 if(*argv[1] == '-' && *(argv[1] + 1) == 'p')
//                 {
//                     printf("enter dencity and velocity:\n");
//                     scanf("%f %f", n, v);
//                     return 0;
//                 }
//                 return 1;
//             }
//         default:
//             {
//                 VERROR("incorrect arguments\n//nothing - default param; -p set param//");
//                 return 0;
//             }
//
//     }
// }

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
    int NFcalculate = 0;
    int shouldStopProgram = 0;
    double error = 0;
    int rangeNumber = Partitions;
    Vector2 zoomCoef = {1., 1.};
    int shouldStopDrawing = 0; // stop drawing checkup
    double sum = 0;
    int stop2 = 0;
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
        int ret_val = Plot(Particles, &zoomCoef, &rangeNumber, &error);
        if(ret_val == ERROR)
        {
            VERROR("something went wrong");
            return 1;
        }

        sum += error;
        Nframes++;
        if(ret_val == STOP_PROG)
        {
            Nframes++;
            shouldStopProgram = Nframes > 50 ? 1 : 0;
        }
        else {
            Nframes = 0;
        }
        // if(sum / Nframes < defaultError && Nframes > 20)
        // {
        //     shouldStopProgram = 1;
        // }

        // stop2 = NFcalculate > 1000 ? 1 : 0;

        EndDrawing();
    }

    // printf("fault = %lf\n", sum / NFcalculate);
    printf("SquareMeanFault = %lf | relevant fault = %lf\ntime = %f\n", error, defaultError, GetTime());
    CloseWindow();
    DeleteParticles(Particles);
    return 0;
}
