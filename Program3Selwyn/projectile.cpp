// Tyler Selwyn
// CPSC 440 - Game Programming
// Program 3 - Space Defense Game
// projectile.cpp - implementation for the projectile class

#include "projectile.h"
#include <cmath>

const float PI = 3.14159265f;
const int BALL_SPEED = 7;

Projectile::Projectile()
{
    x = 0;
    y = 0;
    live = false;
    speed = BALL_SPEED;
    radian_angle = 0;
    image = NULL;
}

Projectile::~Projectile()
{
    // image is shared across all projectiles, destroyed in main
}

// draw the projectile if alive
void Projectile::Draw()
{
    if (live && image)
    {
        al_draw_bitmap(image, x, y, 0);
    }
}

// fire from cannon position at the cannon's current angle
void Projectile::Fire(float cannonAngle, int cannonX, int cannonY)
{
    if (!live)
    {
        x = cannonX;
        y = cannonY;
        // convert cannon angle to radians for trajectory
        radian_angle = ((cannonAngle + 64.0f) / 0.711f) * ((2.0f * PI) / 360.0f);
        live = true;
    }
}

// move projectile along its trajectory each frame
void Projectile::Update()
{
    if (live)
    {
        x -= (int)(speed * cos(radian_angle));
        y -= (int)(speed * sin(radian_angle));
    }
}

// check if projectile went off screen so we can deactivate it
bool Projectile::IsOffScreen(int WIDTH, int HEIGHT)
{
    if (live)
    {
        if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        {
            live = false;
            return true;
        }
    }
    return false;
}