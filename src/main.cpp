#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <time.h>
#include <climits>
#include <iostream>
#include <pthread.h>

#include "particles.hpp"

int main()
{
    sf::RenderWindow window {sf::VideoMode(WindowSize.x, WindowSize.y), "Maxwell distribution"};

    Ball **balls = CreateBalls(N0, {BoxSize.x, BoxSize.y}, default_radius);
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f((float)BoxSize.x, 0.f);
    line[0].color  = sf::Color::Red;
    line[1].position = sf::Vector2f((float)BoxSize.x, (float)BoxSize.y);
    line[1].color = sf::Color::Magenta;

    unsigned int Partition = default_partition;

    //arguments for RenderBalls function
    shared_args shared = {
        .balls = balls,
        .NBalls = N0,
        .window = window,
        .Size = BoxSize,
        .float_arg = 1.f / 30,
    };

    //arguments for Plot function
    ret_err ret_plt_val = OK;
    sf::Vector2f zoomCoef = {1.f, 1.f};
    plot_args plt_args = {
        .shared = shared,
        .Partition = Partition,
        .zoomCoef = zoomCoef,
        .error = &ret_plt_val,
    };
    plt_args.shared.float_arg = default_start_velocity;

    //variables to calculate game loop time
    clock_t start = 0, end = 0; 
    float delta_time = 0;

    //game loop
    while(window.isOpen())
    {
        start = clock();
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::F)
                {
                    zoomCoef.y /= 1.5f;
                }
                if(event.key.code == sf::Keyboard::S && zoomCoef.y < 1.f)
                {
                    zoomCoef.y *= 1.5f;
                }
                if(event.key.code == sf::Keyboard::E && Partition < INT_MAX / 2)
                {
                    Partition *= 2;
                }
                if(event.key.code == sf::Keyboard::C && Partition > 2)
                {
                    Partition >>= 1;
                }
                if(event.key.code == sf::Keyboard::G)
                {
                    zoomCoef.x /= 1.5f;
                }
                if(event.key.code == sf::Keyboard::A && zoomCoef.x < 1.f)
                {
                    zoomCoef.x *= 1.5f;
                }

            }
        }

        window.clear(sf::Color::White);

        // RenderBalls(balls, window, N0, time.asSeconds(), BoxSize);
        // Plot(balls, N0, window, _PlotSize, Partition, default_start_velocity);
        //
        // pthread_t render, plot;
        // window.setActive(false);
        // pthread_create(&render, NULL, RenderBalls, &shared);
        // plot_args plt_args = {
        //     .shared = shared,
        //     .Partition = Partition,
        // };
        // plt_args.shared.float_arg = default_start_velocity;
        // pthread_create(&plot, NULL, Plot, &plt_args);
        //
        // pthread_join(render, NULL);
        // pthread_join(plot, NULL);
        // window.setActive();
        //
        RenderBalls(&shared);
        shared.float_arg = delta_time;
        Plot(&plt_args);
        window.draw(line);

        window.display();

        //calculate frame time
        end = clock();
        delta_time = float(end - start) / CLOCKS_PER_SEC;
        std::cout << "it took " << end - start << "ticks, or " << (float)(end - start)/CLOCKS_PER_SEC << "seconds." << std::endl;

    }

    DeleteBalls(balls, N0);

    return 0;
}
