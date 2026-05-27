// Tyler Selwyn
// CPSC 440 - Game Programming
// Program 3 - Space Defense Game
// alien.h - header for the dropping alien enemy class

#ifndef ALIEN_H
#define ALIEN_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Alien
{
public:
    Alien();
    ~Alien();
    void Draw();
    void Start(int WIDTH, int HEIGHT);
    void Update();
    bool CollideBullet(int bx, int by);
    bool CollideBase(int baseY);

    int getX() { return x; }
    int getY() { return y; }
    bool getLive() { return live; }
    void setLive(bool l) { live = l; }
    void setImage(ALLEGRO_BITMAP* img) { image = img; }

private:
    int x;
    int y;
    bool live;
    int speed;
    int boundx;
    int boundy;
    ALLEGRO_BITMAP* image;
};

#endif