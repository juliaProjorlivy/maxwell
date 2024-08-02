#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <climits>

#include "mgl2/data.h"
#include "mgl2/mgl.h"

#include "particles.hpp"
#include "verror.h"

void *Plot(void *_args)
{
    plot_args args = *((plot_args *)_args);

    float start_velocity = args.shared.float_arg;
    unsigned int NBalls = args.shared.NBalls;
    sf::Vector2u PltSize = args.shared.Size;
    unsigned int Partition = args.Partition;
    sf::Vector2f zoomCoef = args.zoomCoef;
    Ball **balls = args.shared.balls;

    // some useful data
    float start_velocity2 = start_velocity * start_velocity;
    float deltaV = (float)NBalls * start_velocity2 / (float)Partition;
    //coefficients in linear function
    float a = -1.5f / start_velocity2;
    float b = log((float)NBalls * deltaV * (-a));

    //data for plot
    float *data_y = new float[Partition];
    float *data_x = new float[Partition];
    if(!data_x | !data_y)
    {
        VERROR_MEM;
        *args.error = MEM_ERR;
        return args.error;
    }

    //filling data
    for(unsigned int i = 0; i < NBalls; i++)
    {
        sf::Vector2f v = balls[i]->getV();
        float v2 = v * v;
        int index = (int)(v2 / deltaV);
        data_y[index]++;
    }
    for(unsigned int i = 0; i < Partition; i++) 
    {
        data_y[i] = (data_y[i] == 0) ? 0.f : log(data_y[i]);
    }
    for(unsigned int i = 0; i < Partition; i++)
    {
        data_x[i] = (float)i * deltaV;
    }

    //setting data for plotting
    mglData y(Partition);
    mglData x(Partition);

    y.Set(data_y, Partition);
    x.Set(data_x, Partition);

    //plot on data
    mglGraph gr(0, (int)PltSize.x, (int)PltSize.y);

    //some plot settings
    gr.SetRanges(0., (float)NBalls * start_velocity2 * 0.02 * zoomCoef.x, 0., log(NBalls) * zoomCoef.y);
    gr.Axis();
    gr.Label('x', "v^2");
    gr.Label('y', "ln(N)");
    gr.Bars(x, y);

    //plot on linear function
    const size_t max_fplot_len = 30;
    char fit_lfunc[max_fplot_len] = {};
    if(snprintf(fit_lfunc, max_fplot_len, "%f * x + %f", a, b) <= 0)
    {
        VERROR("output error is encountered");
        *args.error = MEM_ERR;
        return args.error;
    }
    gr.FPlot(fit_lfunc, "r-");

    sf::Image img;
    sf::Texture tex;
    sf::Sprite spr;

    img.create(PltSize.x, PltSize.y, gr.GetRGBA());
    tex.loadFromImage(img);
    spr.setOrigin({0.f, 0.f});
    spr.setPosition({(float)BoxSize.x, 0.f});
    spr.setTexture(tex);

    args.shared.window.setActive();
    args.shared.window.draw(spr);
    args.shared.window.setActive(false);

    delete [] data_x;
    delete [] data_y;

    return NULL;

}


