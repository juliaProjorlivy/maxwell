#include "abstract.h"
#include "data_cf.h"
#include "plot.h"
#include "raylib.h"
#include "raymath.h"
#include <cstddef>
#include <stdio.h>

#include "mgl.h"

#include "particles.h"
#include "verror.h"

#include "plotting.h"


int VelocityCompare(const void *p1, const void *p2)
{
    const particle *p11 = *(const particle **)p1;
    const particle *p22 = *(const particle **)p2;
    return (Vector2DotProduct(p11->v, p11->v) > Vector2DotProduct(p22->v, p22->v)) ? 1 : -1;
}

void SortByVelocity(particle **Particles, int NParticles)
{
    qsort(Particles, NParticles, sizeof(particle *), VelocityCompare);
}

static const float epsV = 20.;

void Plot(particle **Particles, int NParticles)
{
    particle **Particles2 = (particle **)calloc(sizeof(particle *), NParticles );
    memcpy(Particles2, Particles, NParticles * sizeof(particle *));
    for(int i = 0; i < NParticles; i++)
    {
        Particles2[i] = (particle *)calloc(sizeof(particle), 1);
        memcpy(Particles2[i], Particles[i], sizeof(particle ));
    }
    qsort(Particles2, NParticles, sizeof(particle *), VelocityCompare);
    //
    //
    float deltaV = NParticles * startVelocity * startVelocity / Partition + epsV;
    int *Ns = (int *)calloc(sizeof(int), Partition);
    float *xs = (float *)calloc(sizeof(float), Partition);
    // //
    int i = 0;
    int j = 0;
    int sum = 0;
    while(i < Partition)
    {
        xs[i] = (i + 1) * deltaV;
        printf("deltaV%d = %f\t", i, xs[i]);
        while(Vector2DotProduct(Particles2[j]->v, Particles2[j]->v) <= xs[i])
        {
            Ns[i]++;
            // printf("\nv = %f, j = %d\n ", Vector2DotProduct(Particles[j]->v, Particles[j]->v), j);
            j++;
            if(j >= NParticles)
            {
                // printf("j = %d, NParticles = %d kiked\n", j, NParticles);
                break;
            }
        }
        sum += Ns[i];
        printf("| Nx = %d\n", Ns[i]);
        if(j >= NParticles)
        {
            break;
        }
        i++;
    }
    printf("sum = %d\n", sum);

    HMGL gr = mgl_create_graph(600, 400);
    HMDT dat1 = mgl_create_data_size(Partition, 0, 0);
    HMDT dat2 = mgl_create_data_size(Partition, 0, 0);
    for(int i = 0; i < Partition; i++)
    {
        mgl_data_set_value(dat1, Ns[i], i, 0, 0);
        mgl_data_set_value(dat2, xs[i], i, 0, 0);
    }
    mgl_set_ranges(gr, 0, N0 * startVelocity * startVelocity * 1.10, 0, N0, 0, 0);      // ranges of coordinates
    mgl_set_light(gr,1); 
    mgl_axis(gr,"xy","","");        // draw axis
    char string[20] = {};
    sprintf(string, "\\sum = %d", sum);
    mgl_puts(gr, 1, 3, 0, string, "",-1);
    mgl_bars_xy(gr, dat2, dat1, "", "");
    // mgl_plot_xy(gr, dat2, dat1, "", "");
    // mgl_write_frame(gr,"sample.png","");
    mgl_get_rgb(gr);
    Image plot = {(void *) mgl_get_rgb(gr), 600, 400, 1,PIXELFORMAT_UNCOMPRESSED_R8G8B8};
    Texture2D plt_texture = LoadTextureFromImage(plot);
    DrawTexture(plt_texture, 0, 0, WHITE);
    mgl_delete_data(dat1);          // free used memory
    mgl_delete_graph(gr);

    for(int i = 0; i < NParticles; i++)
    {
        free(Particles2[i]);
    }
    free(Particles2);
    free(Ns);
    free(xs);
}





