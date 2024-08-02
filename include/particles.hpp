#ifndef _PARTICLES_H
#define _PARTICLES_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

//Window Sizes
static const sf::Vector2u WindowSize {1000, 600};
static const sf::Vector2u BoxSize {500, WindowSize.y};
static const sf::Vector2u PlotSize = {WindowSize.x - BoxSize.x, WindowSize.y};
//Default balls attributes
static const float default_radius = 10.f;
static const float default_start_velocity = 20.f;
static const sf::Color default_fill_color = sf::Color::Green;
static const unsigned int N0 = 50;
//Plot Partition
static const unsigned int default_partition = 3000;

class Ball
{
    private:
    //Constant values
    const float pos_eps = 1e-3f;
    float start_velocity;

    sf::CircleShape circle;
    sf::Vector2f v;

    public:
    // Setting private functions
    sf::CircleShape &getCircle() {return circle;};
    sf::Vector2f &getV() {return v;};

    //Constructor/Destructor
    Ball(sf::Vector2f pos, unsigned int seed, float _start_velocity = default_start_velocity, float r = default_radius, sf::Color color = default_fill_color);
    ~Ball();

    //Motion functions
    void WallCollision(sf::Vector2u _BoxSize);
    void BallCollision(Ball *ball, float time);
    void ChangePosition(float time);
};

enum ret_err
{
    OK = 0,
    MEM_ERR = 1,
};

// struct for arguments of RednerBalls function
struct shared_args
{
    Ball **balls;
    unsigned int NBalls;
    sf::RenderWindow &window;
    sf::Vector2u Size;
    float float_arg;
};

// struct for arguments of Plot function
struct plot_args
{
    shared_args shared;
    unsigned int &Partition;
    sf::Vector2f &zoomCoef;
    ret_err *error;
};

// dot product of vectors
template<typename T>
T operator*(sf::Vector2<T> v1, sf::Vector2<T> v2);

Ball **CreateBalls(unsigned int NBalls, sf::Vector2u _BoxSize, float radius);

void *RenderBalls(void *args);

void DeleteBalls(Ball **balls, unsigned int NBalls);

void *Plot(void *args);

#endif

