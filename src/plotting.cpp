#include "base_cf.h"
#include "canvas_cf.h"
#include "qt.h"
#include "mgl_cf.h"
#include "raylib.h"
#include "raymath.h"
#include <limits.h>

#include "particles.h"
#include "verror.h"

#include "plotting.h"

// KEY MAPPING
// SPACE - STOP
// CTRL+D - QUIT
// F - ZOOM ON THE GRAPH 
// S - ZOOM OUT OF THE GRAPH 
// E - INCREASE THE AMOUNT OF VELOCITY SECTOR DIVISION
// C - DECREASE THE AMOUNT OF VELOCITY SECTOR DIVISION

void Plot(particle **Particles, int NParticles, Vector2 BoxSize, float *zoomCoef, int *Partition, float epsV)
{
    if(IsKeyPressed(KEY_F) && *zoomCoef > 0)
    {
        *zoomCoef -= 0.1;
    }
    if(IsKeyPressed(KEY_S) && *zoomCoef < 1)
    {
        *zoomCoef += 0.1;
    }
    if(IsKeyPressed(KEY_E) && *Partition < INT_MAX / 2)
    {
        *Partition *= 2;
    }
    if(IsKeyPressed(KEY_C) && *Partition > 2)
    {
        *Partition >>= 1;
    }

    float deltaV = NParticles * startVelocity * startVelocity / *Partition + epsV;
    int *Ns = (int *)calloc(sizeof(int), *Partition);
    float *xs = (float *)calloc(sizeof(float), *Partition);

    for(int i = 0; i < *Partition; i++)
    {
        xs[i] = (i + 1) * deltaV;
    }

    for(int i = 0; i < NParticles; i++)
    {
        int index = (int)(Vector2DotProduct(Particles[i]->v, Particles[i]->v) / deltaV);
        Ns[index]++;
    }

    HMGL gr = mgl_create_graph(PlotWidth, BoxSize.y);

    HMDT dat1 = mgl_create_data_size(*Partition, 0, 0);
    HMDT dat2 = mgl_create_data_size(*Partition, 0, 0);
    for(int i = 0; i < *Partition; i++)
    {
        mgl_data_set_value(dat1, Ns[i], i, 0, 0);
        mgl_data_set_value(dat2, xs[i], i, 0, 0);
    }


    

    mgl_set_ranges(gr, 0, N0 * startVelocity * startVelocity * 0.1, 0, N0 * *zoomCoef, 0, 0);      // ranges of coordinates
    mgl_label(gr, 'x', "v^2", 0, "");
    mgl_label(gr, 'y', "N", 0, "");
    mgl_set_light(gr,1); 
    mgl_axis(gr,"xy","","");        // draw axis
    mgl_bars_xy(gr, dat2, dat1, "", "");
    mgl_get_rgb(gr);
    Image plot = {(void *) mgl_get_rgb(gr), PlotWidth, (int)BoxSize.y, 1,PIXELFORMAT_UNCOMPRESSED_R8G8B8};
    Texture2D plt_texture = LoadTextureFromImage(plot);
    DrawTexture(plt_texture, BoxSize.x, 0, WHITE);
    mgl_delete_data(dat1);          // free used memory
    mgl_delete_data(dat2);          // free used memory
    mgl_delete_graph(gr);

        

    float *Ns2 = (float *)calloc(sizeof(float), *Partition);
    deltaV = sqrt(NParticles * startVelocity * startVelocity) / *Partition;
    for(int i = 0; i < *Partition; i++)
    {
        xs[i] = (i + 1) * deltaV;
    }

    for(int i = 0; i < NParticles; i++)
    {
        int index = (int)(sqrt(Vector2DotProduct(Particles[i]->v, Particles[i]->v)) / deltaV);
        Ns2[index]++;
    }

    HMGL plt = mgl_create_graph(BoxSize.x + PlotWidth, plotHeight);

    HMDT data1 = mgl_create_data_size(*Partition, 0, 0);
    HMDT data2 = mgl_create_data_size(*Partition, 0, 0);
    for(int i = 0; i < *Partition; i++)
    {
        Ns2[i] /= NParticles;
        printf("%d %f\n", i, Ns2[i]);
        mgl_data_set_value(data1, Ns2[i], i, 0, 0);
        mgl_data_set_value(data2, xs[i], i, 0, 0);
    }


    mgl_set_ranges(plt, 0, startVelocity * sqrt(N0) * 0.7, 0, 1.1 / startVelocity, 1, 1);      // ranges of coordinates
    mgl_label(plt, 'x', "v", 0, "");
    mgl_label(plt, 'y', "F(v)", 0, "");
    mgl_set_light(plt,1); 
    mgl_axis(plt,"xy","","");        // draw axis
    mgl_fplot(plt, "( 3 / ( 500 * 500 ) ) * x * ((2.7182818)^( (-1.5) * (( x / 500 ) ^ 2) ))", "", "");
    //
    mgl_bars_xy(plt, data2, data1, "", "");
    mgl_get_rgb(plt);
    Image plot_maxwell = {(void *) mgl_get_rgb(plt), (int)BoxSize.x + PlotWidth, plotHeight, 1,PIXELFORMAT_UNCOMPRESSED_R8G8B8};
    Texture2D plot_maxwell_texture = LoadTextureFromImage(plot_maxwell);
    DrawTexture(plot_maxwell_texture, 0, BoxSize.y, WHITE);
    mgl_delete_data(data1);          // free used memory
    mgl_delete_data(data2);          // free used memory
    mgl_delete_graph(plt);

    free(Ns);
    free(Ns2);
    free(xs);

}





