// Tyler Selwyn
// CPSC 440 - Game Programming
// Program 3 - Space Defense Game
// projectile.h - header for the projectile class

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Projectile
{
public:
    Projectile();
    ~Projectile();
    void Draw();
    void Fire(int tipX, int tipY, float drawAngle);
    void Update();
    bool IsOffScreen(int WIDTH, int HEIGHT);

    int getX() { return x; }
    int getY() { return y; }
    bool getLive() { return live; }
    void setLive(bool l) { live = l; }
    void setImage(ALLEGRO_BITMAP* img) { image = img; }

private:
    int x;
    int y;
    float dx;
    float dy;
    bool live;
    int speed;
    ALLEGRO_BITMAP* image;
};

#endif