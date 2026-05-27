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
    dx = 0;
    dy = 0;
    live = false;
    speed = BALL_SPEED;
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

// fire from the cannon tip position, using the cannon's draw angle for direction
void Projectile::Fire(int tipX, int tipY, float drawAngle)
{
    if (!live)
    {
        x = tipX;
        y = tipY;
        // projectile travels in the direction the barrel is pointing
        dx = speed * sin(drawAngle);
        dy = -speed * cos(drawAngle);
        live = true;
    }
}

// move projectile along its trajectory each frame
void Projectile::Update()
{
    if (live)
    {
        x += (int)dx;
        y += (int)dy;
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