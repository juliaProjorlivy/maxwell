#include "base_cf.h"
#include "canvas_cf.h"
#include "qt.h"
#include "mgl_cf.h"
#include "raylib.h"
#include "raymath.h"
#include <limits.h>

#include "particles.h"
#include "verror.h"

// KEY MAPPING
// SPACE - STOP
// CTRL+D - QUIT
// F - ZOOM ON THE GRAPH BY OY 
// S - ZOOM OUT OF THE GRAPH BY OY
// G - ZOOM ON THE GRAPH BY OX
// A - ZOOM OUT OF THE GRAPH BY OX
// E - INCREASE THE AMOUNT OF VELOCITY SECTOR DIVISION
// C - DECREASE THE AMOUNT OF VELOCITY SECTOR DIVISION

int Plot(particle **Particles, Vector2 *zoomCoef, int *Partition, int NParticles, Vector2 BoxSize)
{
    if(IsKeyPressed(KEY_F))
    {
        zoomCoef->y /= 1.5;
    }
    if(IsKeyPressed(KEY_S) && zoomCoef->y < 1)
    {
        zoomCoef->y *= 1.5;
    }
    if(IsKeyPressed(KEY_E) && *Partition < INT_MAX / 2)
    {
        *Partition *= 2;
    }
    if(IsKeyPressed(KEY_C) && *Partition > 2)
    {
        *Partition >>= 1;
    }
    if(IsKeyPressed(KEY_G))
    {
        zoomCoef->x /= 1.5;
    }
    if(IsKeyPressed(KEY_A) && zoomCoef->x < 1)
    {
        zoomCoef->x *= 1.5;
    }

    float startVelocityIn2 = startVelocity * startVelocity;
    float alpha = 1.5 / startVelocityIn2;
    float deltaV = NParticles * startVelocityIn2 / *Partition;

    int *y = (int *)calloc(sizeof(int), *Partition);                // number of particles with the v^2 in range [v, v + deltaV]
    float *x = (float *)calloc(sizeof(float), *Partition);          // v^2
    if(!x | !y)
    {
        VERROR_MEM;
        return 1;
    }

    for(int i = 0; i < *Partition; i++)
    {
        x[i] = i * deltaV;
    }

    for(int i = 0; i < NParticles; i++)
    {
        int index = (int)(Vector2DotProduct(Particles[i]->v, Particles[i]->v) / deltaV);
        y[index]++;
    }

    HMGL gr = mgl_create_graph(PlotWidth, BoxSize.y);

    // filling the data
    HMDT dat1 = mgl_create_data_size(*Partition, 0, 0);
    HMDT dat2 = mgl_create_data_size(*Partition, 0, 0);
    for(int i = 0; i < *Partition; i++)
    {
        mgl_data_set_value(dat1, log(y[i]), i, 0, 0);
        mgl_data_set_value(dat2, x[i], i, 0, 0);
    }

    // setting the plot
    mgl_set_ranges(gr, 0, NParticles * startVelocityIn2 * zoomCoef->x, 0, log(NParticles) * zoomCoef->y, 0, 0);
    mgl_label(gr, 'x', "v^2", 0, "");
    mgl_label(gr, 'y', "N", 0, "");
    mgl_set_light(gr, 1);
    mgl_axis(gr,"xy","","");
    mgl_bars_xy(gr, dat2, dat1, "", "");

    // linear graph
    char fitFunc[100] = {};
    sprintf(fitFunc, "(-1 * %f * x + %f", alpha, log(NParticles * deltaV * alpha));
    mgl_fplot(gr, fitFunc, "", "");

    // draw in memory and then draw in window
    mgl_get_rgb(gr);
    Image plot = {(void *) mgl_get_rgb(gr), PlotWidth, (int)BoxSize.y, 1,PIXELFORMAT_UNCOMPRESSED_R8G8B8};
    Texture2D plt_texture = LoadTextureFromImage(plot);
    DrawTexture(plt_texture, BoxSize.x, 0, WHITE);

    // clean
    mgl_delete_data(dat1);
    mgl_delete_data(dat2);
    mgl_delete_graph(gr);
    free(y);
    free(x);

    return 0;
}





