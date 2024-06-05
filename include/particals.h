#ifndef PARTICALS_H
#define PARTICALS_H

#include "raylib.h"

static const float epsilon = 1e-3;
static const float startVelocity = 1000.;


static const int N0 = 5;             // number of particals
static const int r0 = 100;             // radius of particals
static const int WindowHeight = 800;   // box height
static const int WindowWidth = 1020;   // box width

typedef struct
{
    Vector2 pos;
    Vector2 v;
} partical;



#endif

