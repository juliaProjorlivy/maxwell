#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <math.h>
#include <ctime>
#include <cstdlib>

#include "particles.hpp"
#include "verror.h"

Ball::Ball(sf::Vector2<float> _pos, float _start_velocity, float _r, sf::Color color)
{
    //setting Color
    circle.setFillColor(color);

    //setting constant values
    circle.setRadius(_r);
    start_velocity = _start_velocity;

    //setting position
    circle.setOrigin(_r, _r);
    circle.setPosition(_pos);

    //setting velocity
    srand(_pos.x);
    float angle = float(rand() % 360);
    v = {start_velocity * cos(angle), start_velocity * sin(angle)};
}

Ball::~Ball(){};

void Ball::WallCollision(sf::Vector2<unsigned int> _BoxSize)
{
    sf::Vector2<float> pos = circle.getPosition();
    float r = circle.getRadius();

    // left wall
    if(pos.x < r + pos_eps)
    {
        circle.setPosition(r, pos.y);
        v.x *= -1;
    }
    // right wall
    else if(pos.x > _BoxSize.x - r - pos_eps)
    {
        circle.setPosition(_BoxSize.x - r, pos.y);
        v.x *= -1.;
    }
    // upper wall
    if(pos.y < r + pos_eps) 
    { 
        circle.setPosition(pos.x, r);
        v.y *= -1.;
    }
    // lower wall
    else if(pos.y > _BoxSize.y - r - pos_eps)
    {
        circle.setPosition(pos.x, _BoxSize.y - r);
        v.y *= -1.;
    }
}

template<typename T>
T operator*(sf::Vector2<T> v1, sf::Vector2<T> v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

void Ball::BallCollision(Ball *ball, float time)
{
    sf::Vector2<float> pos = circle.getPosition();
    float r = circle.getRadius();

    sf::Vector2<float> pos1 = v * time + pos;
    sf::Vector2<float> pos2 = ball->v * time + ball->circle.getPosition();

    if(pos1.x + r < pos2.x - r || pos1.x - r > pos2.x + r) return;
    if(pos1.y + r < pos2.y - r || pos1.y - r > pos2.y + r) return;

    sf::Vector2<float> delta = pos1 - pos2;
    float SquareSum = delta * delta;
    float DCollision = sqrt(SquareSum);
    if(SquareSum <= 4 * r * r)
    {
        sf::Vector2<float> e = delta / DCollision;
        sf::Vector2<float> vector = e * (e * (v - ball->v));
        v -= vector;
        ball->v += vector;

        float overlap_d = 0.5 * (2 * r - DCollision);
        sf::Vector2<float> overlap = e * overlap_d;
        circle.move(-1.f * overlap);
        ball->circle.move(overlap);
    }
}

void Ball::ChangePosition(float time)
{
    circle.move(time * v);
}

void RenderBalls(Ball **balls, sf::RenderWindow &balls_window, int NBalls, float time, sf::Vector2<unsigned int> _BoxSize)
{
    for(int i = 0; i < NBalls; i++)
    {
        Ball *ball = balls[i];
        for(int j = i + 1; j < NBalls; j++)
        {
            ball->BallCollision(balls[j], time);
        }

        ball->WallCollision(_BoxSize);
        ball->ChangePosition(time);

        balls_window.draw(ball->circle);
    }
}

Ball **CreateBalls(int NBalls, sf::Vector2<unsigned int> _BoxSize, float radius)
{
    // float time = 1. / 30; // 30 starting fps
    radius += radius * 0.3;

    if((int)(_BoxSize.y / (2 * radius)) * (int)(_BoxSize.x / (2 * radius)) < NBalls)
    {
        VERROR("Size error too many particles");
        return NULL;
    }

    Ball **balls = new Ball*[NBalls];
    if(!balls)
    {
        VERROR_MEM;
        return NULL;
    }

    int Nw = (int)(_BoxSize.x / (2 * radius));
    int Nh = (int)(_BoxSize.y / (2 * radius));

    int Ncolumns = 1, Nstrings = 1;
    float ax = 1, ay = 1;

    if(Nw >= NBalls)
    {
        Ncolumns = NBalls;
        Nstrings = 1;

        ax = _BoxSize.x / (2. * NBalls);
        ay = _BoxSize.y / 2.;
    }
    else if(Nh >= NBalls)
    {
        Ncolumns = 1;
        Nstrings = NBalls;

        ax = _BoxSize.x / 2.;
        ay = _BoxSize.y / (2. * NBalls);
    }
    else
    {
        Ncolumns = Nw;
        Nstrings = Nh;

        ax = _BoxSize.x / (2. * Nw);
        ay = _BoxSize.y / (2. * Nh);
    }

    for(int i = 0; i < Nstrings; i++)
    {
        int N = i * Ncolumns;
        float Y = ay * ((i << 1) + 1);
        for(int j = 0; (j < Ncolumns) && (N + j < NBalls); j++)
        {
            balls[N + j] = new Ball({ax * ((j << 1) + 1), Y});
        }
    }
    return balls;
}

void DeleteBalls(Ball **balls, int NBalls)
{
    for(int i = 0; i < NBalls; i++)
    {
        delete balls[i];
    }
    delete [] balls;
}

