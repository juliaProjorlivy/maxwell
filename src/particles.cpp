#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <cstddef>
#include <math.h>
#include <ctime>

#include "particles.hpp"
#include "verror.h"

template<typename T>
T operator*(sf::Vector2<T> v1, sf::Vector2<T> v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

Ball::Ball(sf::Vector2f pos, unsigned int seed, float _start_velocity, float r, sf::Color color)
{
    //setting Color
    circle.setFillColor(color);

    //setting constant values
    circle.setRadius(r);
    start_velocity = _start_velocity;

    //setting position
    circle.setOrigin(r, r);
    circle.setPosition(pos);

    //setting velocity
    srand(seed);
    float angle = float(rand() % 360);
    v = {start_velocity * cos(angle), start_velocity * sin(angle)};
}

Ball::~Ball(){};

void Ball::WallCollision(sf::Vector2u _BoxSize)
{
    sf::Vector2f pos = circle.getPosition();
    float r = circle.getRadius();

    // left wall
    if(pos.x < r + pos_eps)
    {
        circle.setPosition(r, pos.y);
        v.x *= -1.f;
    }
    // right wall
    else if(pos.x > (float)_BoxSize.x - r - pos_eps)
    {
        circle.setPosition((float)_BoxSize.x - r, pos.y);
        v.x *= -1.f;
    }
    // upper wall
    if(pos.y < r + pos_eps) 
    { 
        circle.setPosition(pos.x, r);
        v.y *= -1.f;
    }
    // lower wall
    else if(pos.y > (float)_BoxSize.y - r - pos_eps)
    {
        circle.setPosition(pos.x, (float)_BoxSize.y - r);
        v.y *= -1.f;
    }
}

void Ball::BallCollision(Ball *ball, float time)
{
    sf::Vector2f pos = circle.getPosition();
    float r = circle.getRadius();

    sf::Vector2f pos1 = v * time + pos;
    sf::Vector2f pos2 = ball->v * time + ball->circle.getPosition();

    if(pos1.x + r < pos2.x - r || pos1.x - r > pos2.x + r) return;
    if(pos1.y + r < pos2.y - r || pos1.y - r > pos2.y + r) return;

    sf::Vector2f delta = pos1 - pos2;
    float SquareSum = delta * delta;
    float DCollision = sqrt(SquareSum);
    if(SquareSum <= 4 * r * r)
    {
        sf::Vector2f e = delta / DCollision;
        sf::Vector2f vector = e * (e * (v - ball->v));
        v -= vector;
        ball->v += vector;

        float overlap_d = 0.5f * (2 * r - DCollision);
        sf::Vector2f overlap = e * overlap_d;
        circle.move(-1.f * overlap);
        ball->circle.move(overlap);
    }
}

void Ball::ChangePosition(float time)
{
    circle.move(time * v);
}

void *RenderBalls(void *_args)
{
    shared_args args = *((shared_args *)_args);
    for(unsigned int i = 0; i < args.NBalls; i++)
    {
        Ball *ball = args.balls[i];
        for(unsigned int j = i + 1; j < args.NBalls; j++)
        {
            ball->BallCollision(args.balls[j], args.float_arg);
        }

        ball->WallCollision(args.Size);
        ball->ChangePosition(args.float_arg);

        args.window.setActive();
        args.window.draw(ball->getCircle());
        args.window.setActive(false);
    }
    return NULL;
}

Ball **CreateBalls(unsigned int NBalls, sf::Vector2u _BoxSize, float radius)
{
    // float time = 1. / 30; // 30 starting fps
    radius += radius * 0.3f;

    unsigned int Nw = (unsigned int)((float)_BoxSize.x / (2 * radius));
    unsigned int Nh = (unsigned int)((float)_BoxSize.y / (2 * radius));

    if(Nw * Nh < NBalls)
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


    unsigned int Ncolumns = 1, Nstrings = 1;
    float ax = 1.f, ay = 1.f;

    if(Nw >= NBalls)
    {
        Ncolumns = NBalls;
        Nstrings = 1;

        ax = (float)_BoxSize.x / (2.f * (float)NBalls);
        ay = (float)_BoxSize.y / 2.f;
    }
    else if(Nh >= NBalls)
    {
        Ncolumns = 1;
        Nstrings = NBalls;

        ax = (float)_BoxSize.x / 2.f;
        ay = (float)_BoxSize.y / (2.f * (float)NBalls);
    }
    else
    {
        Ncolumns = Nw;
        Nstrings = Nh;

        ax = (float)_BoxSize.x / (2.f * (float)Nw);
        ay = (float)_BoxSize.y / (2.f * (float)Nh);
    }

    unsigned int time_seed = (unsigned int)time(0);
    for(unsigned int i = 0; i < Nstrings; i++)
    {
        unsigned int N = i * Ncolumns;
        float Y = ay * ((float)(i << 1) + 1.f);
        for(unsigned int j = 0; (j < Ncolumns) && (N + j < NBalls); j++)
        {
            balls[N + j] = new Ball({ax * ((float)(j << 1) + 1.f), Y}, time_seed + N + j);
        }
    }
    return balls;
}

void DeleteBalls(Ball **balls, unsigned int NBalls)
{
    for(unsigned int i = 0; i < NBalls; i++)
    {
        delete balls[i];
    }
    delete [] balls;
}

