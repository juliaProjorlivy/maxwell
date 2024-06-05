#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "verror.h"
#include "particals.h"


// Create partical with random velocity direction and with the given position value 
partical *CreatePartical(Vector2 Pos)
{
    partical *Partical = (partical *)calloc(sizeof(partical), 1);
    if(!Partical)
    {
        VERROR_MEM;
        return NULL;
    }

    float angle = (float)GetRandomValue(0, 359);
    *Partical = {.pos = Pos, .v = {startVelocity * cos(angle), startVelocity * sin(angle)}};

    return Partical;
}

// Create an array of evenly spreaded particals
partical **CreateParticals(int NParticals, Vector2 WindowSize)
{
    partical **Particals = (partical **)calloc(sizeof(partical *), NParticals);
    if(!Particals)
    {
        VERROR_MEM;
        return NULL;
    }

    int Nw = (int)(WindowSize.x / (2 * r0));
    int Nh = (int)(WindowSize.y / (2 * r0));

    int Ncolumns = 1, Nstrings = 1;
    float ax = 1, ay = 1;

    if(Nw > NParticals)
    {
        Ncolumns = NParticals;
        Nstrings = 1;

        ax = WindowSize.x / (2 * NParticals);
        ay = WindowSize.y / 2;
    }
    else if(Nh > NParticals)
    {
        Ncolumns = 1;
        Nstrings = NParticals;

        ax = WindowSize.x / 2.;
        ay = WindowSize.y / (2 * NParticals);
    }
    else
    {
        Ncolumns = Nw;
        Nstrings = Nh;

        ax = WindowSize.x / (2 * Nw);
        ay = WindowSize.y / (2 * Nh);
    }

    for(int i = 0; i < Nstrings; i++)
    {
        int N = i * Ncolumns;
        float Y = ay * ((i << 1) + 1);
        for(int j = 0; (j < Ncolumns) && (N + j < NParticals); j++)
        {
            partical *Partical = CreatePartical({ax * ((j << 1) + 1), Y});
            if(!Partical)
            {
                VERROR_MEM;
                return NULL;
            }
            Particals[N + j] = Partical;
        }
    }
    return Particals;
}

void ParticalCollision(partical *Partical1, partical *Partical2)
{
    Vector2 delta = Vector2Subtract(Partical2->pos, Partical1->pos);
    float SquareSum = Vector2DotProduct(delta, delta);
    float DCollision = sqrt(SquareSum);
    if(SquareSum <= 4 * r0 * r0)
    {
        Vector2 e = {delta.x / DCollision, delta.y / DCollision};
        Vector2 vector = Vector2Scale(e, Vector2DotProduct(Vector2Subtract(Partical1->v,  Partical2->v), e));
        Partical1->v = Vector2Subtract(Partical1->v, vector);
        Partical2->v = Vector2Add(Partical2->v, vector);

        float delta = (2. * r0 - DCollision) * 0.5;
        Partical1->pos = Vector2Subtract(Partical1->pos, Vector2Scale(e, delta));
        Partical2->pos = Vector2Add(Partical2->pos, Vector2Scale(e, delta));


        /* Partical1->pos = Vector2Add(Partical1->pos, Vector2Scale(Vector2Scale(e, Vector2DotProduct(Partical1->v, e)), GetFrameTime())); */
        /* Partical2->pos = Vector2Add(Partical2->pos, Vector2Scale(Vector2Scale(e, Vector2DotProduct(Partical2->v, e)), GetFrameTime())); */
    }
}

/* void ParticalsCollision(partical **Particals, int NParticals) */
/* { */
/*     for(int i = 0; i < NParticals - 1; i++) */
/*     { */
/*         for(int j = i + 1; j < NParticals; j++) */
/*         { */
/*             ParticalCollision(Particals[i], Particals[j]); */
/*         } */
/*     } */
/* } */

void WallCollision(partical *Partical)
{
    // float tau = GetFrameTime();
    //
    // if((Partical->v.y < 0) && (-1 * (WindowHeight - Partical->pos.y) / Partical->v.y < tau))
    // {
    //     Partical->pos.x += Partical->v.x * tau;
    //     Partical->v.y *= -1;
    //     Partical->pos.y = WindowHeight - Partical->v.y * (tau - (WindowHeight - Partical->pos.y) / Partical->v.y);
    // }
    // else if((Partical->v.y > 0) && (Partical->pos.y / Partical->v.y < tau))
    // {
    //     Partical->pos.x += Partical->v.x * tau;
    //     Partical->v.y *= -1;
    //     Partical->pos.y = Partical->v.y * (tau + Partical->pos.y / Partical->v.y);
    // }
    //
    // if((Partical->v.x < 0) && (-1 * Partical->pos.x / Partical->v.x < tau))
    // {
    //     Partical->v.x *= -1;
    //     Partical->pos.x = Partical->v.x * (tau - Partical->pos.x / Partical->v.x);
    //     Partical->pos.y += Partical->v.y * tau;
    // }
    // else if((Partical->v.x > 0) && ((WindowWidth - Partical->pos.x) / Partical->v.x < tau))
    // {
    //     Partical->v.x *= -1;
    //     Partical->pos.x = WindowWidth - Partical->v.x * (tau + (WindowWidth - Partical->pos.x) / Partical->v.x);
    //     Partical->pos.y += Partical->v.y * tau;
    // }

    if(Partical->pos.x < (r0 + epsilon)) 
    { 
        Partical->pos.x = r0; 
        Partical->v.x *= -1.; 
    } 
    else if(Partical->pos.x > (WindowWidth - r0 - epsilon))
    { 
        Partical->pos.x = WindowWidth - r0; 
        Partical->v.x *= -1.; 
    } 

    if(Partical->pos.y < (r0 + epsilon)) 
    { 
        Partical->pos.y = r0; 
        Partical->v.y *= -1.; 
    } 
    else if(Partical->pos.y > (WindowHeight - r0 - epsilon)) 
    { 
        Partical->pos.y = WindowHeight - r0; 
        Partical->v.y *= -1.; 
    } 
}

void ChangePosition(partical *Partical, float frametime)
{
    Partical->pos = Vector2Add(Partical->pos, Vector2Scale(Partical->v, frametime));
}

void DrawParticals(partical **Particals, int NParticals)
{
    for(int i = 0; i < NParticals; i++)
    {
        partical *Partical = Particals[i];
        DrawCircle((int)(Partical->pos.x), (int)(Partical->pos.y), r0, BLUE);
        for(int j = i + 1; j < NParticals; j++)
        {
            ParticalCollision(Particals[i], Particals[j]);
        }
        ChangePosition(Partical, GetFrameTime());
        WallCollision(Partical);
    }
}

void DrawVelocity(partical **Particals, int NParticals)
{
    float sum = 0;
    for(int i = 0; i < NParticals; i++)
    {
        sum += Vector2DotProduct(Particals[i]->v, Particals[i]->v);
    }
    printf("Velocity SquareSum = %f\n", sum);
}

void DeleteParticals(partical **Particals, int NParticals)
{
    for(int i = 0; i < NParticals; i++)
    {
        free(Particals[i]);
    }
    free(Particals);
}

void PrintInfo(partical **Particals, int NParticals)
{
    for(int i = 0; i < NParticals; i++)
    {
        printf("%d. VELOCITY: %f\n", i, sqrt(Vector2DotProduct(Particals[i]->v, Particals[i]->v)));
    }
    printf("---------------------\n");
}



int main()
{
    InitWindow(WindowWidth, WindowHeight, "Maxwell");

    if((int)(WindowHeight / (2 * r0)) * (int)(WindowWidth / (2 * r0)) < N0)
    {
        VERROR("Size error too many particals");
        return 1;
    }
    Vector2 WindowSize = {.x = WindowWidth, .y = WindowHeight};

    partical **Particals = CreateParticals(N0, WindowSize);
    if(!Particals)
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
        DrawParticals(Particals, N0);
        PrintInfo(Particals, N0);
        DrawVelocity(Particals, N0);
        }
        EndDrawing();
    }

    DeleteParticals(Particals, N0);
    return 0;
}
