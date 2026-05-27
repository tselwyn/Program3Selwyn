// Tyler Selwyn
// CPSC 440 - Game Programming
// Program 3 - Space Defense Game
// alien.cpp - implementation for the dropping alien enemy

#include "alien.h"
#include <cstdlib>

Alien::Alien()
{
    x = 0;
    y = 0;
    live = false;
    speed = 1;
    boundx = 50;
    boundy = 60;
    image = NULL;
}

Alien::~Alien()
{
    // image is shared across all aliens, destroyed in main
}

// draw the alien if it is alive
void Alien::Draw()
{
    if (live && image)
    {
        al_draw_bitmap(image, x, y, 0);
    }
}

// spawn alien at random x position at top of screen
void Alien::Start(int WIDTH, int HEIGHT)
{
    if (!live)
    {
        x = rand() % (WIDTH - boundx);
        y = -boundy;
        live = true;
        speed = 1 + rand() % 2;
    }
}

// move alien downward each frame
void Alien::Update()
{
    if (live)
    {
        y += speed;
    }
}

// check if a projectile hit this alien
bool Alien::CollideBullet(int bx, int by)
{
    if (live)
    {
        if (bx > x && bx < x + boundx && by > y && by < y + boundy)
        {
            live = false;
            return true;
        }
    }
    return false;
}

// check if alien reached the base
bool Alien::CollideBase(int baseY)
{
    if (live)
    {
        if (y + boundy >= baseY)
        {
            live = false;
            return true;
        }
    }
    return false;
}