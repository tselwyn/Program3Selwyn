// Tyler Selwyn
// CPSC 440 - Game Programming
// Program 3 - Space Defense Game
// Program3Selwyn.cpp - main game loop, cannon control, drawing

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "projectile.h"
#include "alien.h"

// screen size
const int WIDTH = 800;
const int HEIGHT = 600;
const float FPS = 60.0;

// game object counts
const int NUM_PROJECTILES = 10;
const int NUM_ALIENS = 5;

// cannon barrel length from pivot to tip (measured from image)
const int BARREL_LENGTH = 95;

// keyboard tracking
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
bool keys[5] = { false, false, false, false, false };

int main()
{
    // --- allegro init ---
    if (!al_init())
    {
        fprintf(stderr, "failed to init allegro\n");
        return -1;
    }
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();

    // create display and event stuff
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_FONT* font = al_create_builtin_font();

    if (!display || !event_queue || !timer)
    {
        fprintf(stderr, "failed to create display or event queue\n");
        return -1;
    }

    // register event sources - display source needed for keyboard
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // load game images
    ALLEGRO_BITMAP* background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP* cannonImg = al_load_bitmap("cannon.png");
    ALLEGRO_BITMAP* alienImg = al_load_bitmap("alien.png");
    ALLEGRO_BITMAP* projImg = al_load_bitmap("projectile.png");
    ALLEGRO_BITMAP* baseImg = al_load_bitmap("base.png");

    if (!background || !cannonImg || !alienImg || !projImg || !baseImg)
    {
        fprintf(stderr, "failed to load one or more images\n");
        return -1;
    }

    bool redraw = true;
    bool done = false;

    // cannon sits at bottom center of screen
    int cannonX = WIDTH / 2;
    int cannonY = HEIGHT - 120;
    float cannonAngle = 128.0f;  // starts pointing straight up in 0-256 range

    // create arrays of game objects
    Projectile projectiles[NUM_PROJECTILES];
    Alien aliens[NUM_ALIENS];

    // give each object its sprite
    for (int i = 0; i < NUM_ALIENS; i++)
        aliens[i].setImage(alienImg);
    for (int i = 0; i < NUM_PROJECTILES; i++)
        projectiles[i].setImage(projImg);

    srand((unsigned)time(NULL));

    al_start_timer(timer);

    // --- main game loop ---
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        // calculate cannon draw angle — used for rendering and firing
        float drawAngle = -((cannonAngle - 128.0f) / 128.0f) * (ALLEGRO_PI / 2.0f);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;

            // rotate cannon left
            if (keys[LEFT])
            {
                cannonAngle += 2.5f;
                if (cannonAngle > 256) cannonAngle = 256;
                if (cannonAngle < 171 && cannonAngle > 85)
                    cannonAngle = 171;
            }

            // rotate cannon right
            if (keys[RIGHT])
            {
                cannonAngle -= 2.5f;
                if (cannonAngle < 0) cannonAngle = 256;
                if (cannonAngle < 171 && cannonAngle > 85)
                    cannonAngle = 85;
            }

            // move all live projectiles and check if off screen
            for (int i = 0; i < NUM_PROJECTILES; i++)
            {
                projectiles[i].Update();
                projectiles[i].IsOffScreen(WIDTH, HEIGHT);
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = true;
                // calculate where the tip of the barrel is right now
                {
                    int tipX = cannonX + (int)(BARREL_LENGTH * sin(drawAngle));
                    int tipY = cannonY - (int)(BARREL_LENGTH * cos(drawAngle));
                    // find an inactive projectile and fire it from the tip
                    for (int i = 0; i < NUM_PROJECTILES; i++)
                    {
                        if (!projectiles[i].getLive())
                        {
                            projectiles[i].Fire(tipX, tipY, drawAngle);
                            break;
                        }
                    }
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = false;
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }

        // --- drawing ---
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            // draw background
            al_draw_bitmap(background, 0, 0, 0);

            // draw the base at bottom center
            al_draw_bitmap(baseImg, (WIDTH / 2) - 150, cannonY + 20, 0);

            // draw cannon rotated to current angle
            al_draw_rotated_bitmap(cannonImg, 60, 100, cannonX, cannonY,
                drawAngle, 0);

            // draw all live projectiles
            for (int i = 0; i < NUM_PROJECTILES; i++)
            {
                projectiles[i].Draw();
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }

    // --- cleanup ---
    al_destroy_bitmap(background);
    al_destroy_bitmap(cannonImg);
    al_destroy_bitmap(alienImg);
    al_destroy_bitmap(projImg);
    al_destroy_bitmap(baseImg);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(font);

    return 0;
}