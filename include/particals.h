#ifndef PARTICALS_H
#define PARTICALS_H

#include "raylib.h"

static const int N0 = 2;             // number of particals
static const int r0 = 100;             // radius of particals
static const int WindowHeight = 800;   // box height
static const int WindowWidth = 1000;   // box width

typedef struct
{
    Vector2 pos;
    Vector2 v;
} partical;



#endif

