#include "abstract.h"
#include "data_cf.h"
#include "plot.h"
#include "raylib.h"
#include "raymath.h"

#include "mgl.h"

#include "particles.h"
#include "verror.h"

#include "plotting.h"

void Plot(particle **Particles, int NParticles, Vector2 BoxSize, float epsV)
{

    float deltaV = NParticles * startVelocity * startVelocity / Partition + epsV;
    int *Ns = (int *)calloc(sizeof(int), Partition);
    float *xs = (float *)calloc(sizeof(float), Partition);

    for(int i = 0; i < Partition; i++)
    {
        xs[i] = (i + 1) * deltaV;
    }

    for(int i = 0; i < NParticles; i++)
    {
        int index = (int)(Vector2DotProduct(Particles[i]->v, Particles[i]->v) / deltaV);
        Ns[index]++;
    }

    HMGL gr = mgl_create_graph(PlotWidth, BoxSize.y);
    HMDT dat1 = mgl_create_data_size(Partition, 0, 0);
    HMDT dat2 = mgl_create_data_size(Partition, 0, 0);
    for(int i = 0; i < Partition; i++)
    {
        mgl_data_set_value(dat1, Ns[i], i, 0, 0);
        mgl_data_set_value(dat2, xs[i], i, 0, 0);
    }
    mgl_set_ranges(gr, 0, N0 * startVelocity * startVelocity * 0.1, 0, N0 * 0.5, 0, 0);      // ranges of coordinates
    mgl_set_light(gr,1); 
    mgl_axis(gr,"xy","","");        // draw axis
    mgl_bars_xy(gr, dat2, dat1, "", "");
    mgl_get_rgb(gr);
    Image plot = {(void *) mgl_get_rgb(gr), PlotWidth, (int)BoxSize.y, 1,PIXELFORMAT_UNCOMPRESSED_R8G8B8};
    Texture2D plt_texture = LoadTextureFromImage(plot);
    DrawTexture(plt_texture, BoxSize.x, 0, WHITE);
    mgl_delete_data(dat1);          // free used memory
    mgl_delete_graph(gr);

    free(Ns);
    free(xs);
}





