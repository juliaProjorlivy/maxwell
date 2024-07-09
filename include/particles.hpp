#ifndef _PARTICLES_H
#define _PARTICLES_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

//Ball default attributes
static const float default_radius = 20;
static const float default_start_velocity = 1000;
static const sf::Color default_fill_color = sf::Color::Green;

//Window default attributes
static const sf::Vector2<float> defautl_WindowSize {600, 800};

class Ball
{
    private:
    //Constant values
    const float pos_eps = 1e-3;
    float start_velocity;

    public:
    //Changing values
    sf::CircleShape circle;
    sf::Vector2<float> v;

   //Constructor/Destructor
    Ball(sf::Vector2<float> _pos, float _start_velocity = default_start_velocity, float _r = default_radius, sf::Color color = default_fill_color);
    ~Ball();

    //Motion functions
    void WallCollision(sf::Vector2<unsigned int> _BoxSize);
    void BallCollision(Ball *ball, float time);
    void ChangePosition(float time);
};

Ball **CreateBalls(int NBalls, sf::Vector2<unsigned int> _BoxSize, float radius);

void RenderBalls(Ball **balls, sf::RenderWindow &balls_window, int NBalls, float time, sf::Vector2<unsigned int> _BoxSize);

void DeleteBalls(Ball **balls, int NBalls);

#endif

