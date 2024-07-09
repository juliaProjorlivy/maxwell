#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include "particles.hpp"

static const unsigned int BoxWidth = 800;
static const unsigned int BoxHeight = 600;
static const unsigned int N0 = 3;

int main()
{
    sf::RenderWindow balls_window {sf::VideoMode(BoxWidth, BoxHeight), "Maxwell distribution", sf::Style::Default};

    sf::Vector2<unsigned int> BoxSize = {BoxWidth, BoxHeight};
    Ball **balls = CreateBalls(N0, {BoxWidth, BoxHeight}, default_radius);

    //game loop
    while(balls_window.isOpen())
    {
        sf::Event event;
        sf::Clock clock;
        while(balls_window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed | event.type == sf::Event::KeyPressed)
            {
                balls_window.close();
            }
        }

        balls_window.clear(sf::Color::Black);

        sf::Time time = clock.getElapsedTime();
        RenderBalls(balls, balls_window, N0, time.asSeconds(), BoxSize);

        balls_window.display();
        clock.restart();

    }

    DeleteBalls(balls, N0);

    return 0;
}
