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
#include "projectile.h"
#include "alien.h"

// screen size
const int WIDTH = 800;
const int HEIGHT = 600;
const float FPS = 60.0;

// how many of each object we can have at once
const int NUM_PROJECTILES = 10;
const int NUM_ALIENS = 5;

// tracks which keys are being held down
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
bool keys[5] = { false, false, false, false, false };

int main()
{
    // set up allegro
    if (!al_init())
    {
        fprintf(stderr, "failed to init allegro\n");
        return -1;
    }
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();

    // create the window, event queue, and timer
    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_FONT* font = al_create_builtin_font();

    if (!display || !event_queue || !timer)
    {
        fprintf(stderr, "failed to create display or event queue\n");
        return -1;
    }

    // register event sources so we can get input and timer events
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // load all the game images
    ALLEGRO_BITMAP* background = al_load_bitmap("background.png");
    ALLEGRO_BITMAP* cannonImg = al_load_bitmap("cannon.png");
    ALLEGRO_BITMAP* alienImg = al_load_bitmap("alien.png");
    ALLEGRO_BITMAP* projImg = al_load_bitmap("projectile.png");
    ALLEGRO_BITMAP* baseImg = al_load_bitmap("base.png");

    // make sure every image loaded correctly
    if (!background || !cannonImg || !alienImg || !projImg || !baseImg)
    {
        fprintf(stderr, "failed to load one or more images\n");
        if (!background) fprintf(stderr, "  missing: background.png\n");
        if (!cannonImg) fprintf(stderr, "  missing: cannon.png\n");
        if (!alienImg) fprintf(stderr, "  missing: alien.png\n");
        if (!projImg) fprintf(stderr, "  missing: projectile.png\n");
        if (!baseImg) fprintf(stderr, "  missing: base.png\n");
        return -1;
    }

    bool done = false;
    bool redraw = true;

    al_start_timer(timer);

    // game loop runs until player exits
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_UP:
                keys[UP] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = true;
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_UP:
                keys[UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = false;
                break;
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

        // drawing section
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            // draw background
            al_draw_bitmap(background, 0, 0, 0);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }

    // free everything before closing
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