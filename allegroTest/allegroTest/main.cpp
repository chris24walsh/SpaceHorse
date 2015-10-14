#include <allegro5/allegro5.h>
#include "allegro5/allegro_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>


bool done;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *bg = NULL;
ALLEGRO_BITMAP *shipCurrent = NULL;
ALLEGRO_BITMAP *shipUp = NULL;
ALLEGRO_BITMAP *shipDown = NULL;
ALLEGRO_BITMAP *shipLeft = NULL;
ALLEGRO_BITMAP *shipRight = NULL;
ALLEGRO_BITMAP *shipUpLeft = NULL;
ALLEGRO_BITMAP *shipDownLeft = NULL;
ALLEGRO_BITMAP *shipUpRight = NULL;
ALLEGRO_BITMAP *shipDownRight = NULL;
ALLEGRO_DISPLAY *display = NULL;
//ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
int shipX = 200;
int shipY = 200;
int bgX = 0;
int bgY = 0;
int strSpeed = 6;
int diagSpeed = 3;
bool setLeft, setRight, setUp, setDown;
bool slow = false;
int maxX = 3840;
int maxY = 3840;
int screenX = 640;
int screenY = 480;
 
void abort_game(const char* message)
{
    printf("%s \n", message);
	getch();
    exit(1);
}
 
void init(void)
{
    if (!al_init())
        abort_game("Failed to initialize allegro");
 
    if (!al_install_keyboard())
        abort_game("Failed to install keyboard");
 
    timer = al_create_timer(1.0 / 60);
    if (!timer)
        abort_game("Failed to create timer");
 
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(screenX, screenY);
    if (!display)
        abort_game("Failed to create display");

	if(!al_init_image_addon())
		abort_game("Failed to initialize al_init_image_addon");

	bg = al_load_bitmap("c:/dev/allegro/images/bg2.jpg"); //Load background image
	shipUp = al_load_bitmap("c:/dev/allegro/images/shipUp.png");
	shipDown = al_load_bitmap("c:/dev/allegro/images/shipDown.png");
	shipLeft = al_load_bitmap("c:/dev/allegro/images/shipLeft.png");
	shipRight = al_load_bitmap("c:/dev/allegro/images/shipRight.png");
	shipUpLeft = al_load_bitmap("c:/dev/allegro/images/shipUpLeft.png");
	shipDownLeft = al_load_bitmap("c:/dev/allegro/images/shipDownLeft.png");
	shipUpRight = al_load_bitmap("c:/dev/allegro/images/shipUpRight.png");
	shipDownRight = al_load_bitmap("c:/dev/allegro/images/shipDownRight.png");
	shipCurrent = shipRight;
	//al_append_path_component(path, "Images");
	//al_set_path_filename(path, "bg.png");
	//image = al_load_bitmap(al_path_cstr(path, '/'));

	if(!bg)
		abort_game("Failed to load the background image");

	if(!shipUp)
		abort_game("Failed to load the ship image");
	if(!shipDown)
		abort_game("Failed to load the ship image");
	if(!shipLeft)
		abort_game("Failed to load the ship image");
	if(!shipRight)
		abort_game("Failed to load the ship image");
	if(!shipUpLeft)
		abort_game("Failed to load the ship image");
	if(!shipDownLeft)
		abort_game("Failed to load the ship image");
	if(!shipUpRight)
		abort_game("Failed to load the ship image");
	if(!shipDownRight)
		abort_game("Failed to load the ship image");
 
    event_queue = al_create_event_queue();
    if (!event_queue)
        abort_game("Failed to create event queue");
 
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
 
    done = false;
}
 
void shutdown(void)
{
    if (timer)
        al_destroy_timer(timer);
 
    if (display)
        al_destroy_display(display);

	if (bg)
		al_destroy_bitmap(bg);

	if (shipUp)
		al_destroy_bitmap(shipUp);
	if (shipDown)
		al_destroy_bitmap(shipDown);
	if (shipLeft)
		al_destroy_bitmap(shipLeft);
	if (shipRight)
		al_destroy_bitmap(shipRight);
	if (shipUpLeft)
		al_destroy_bitmap(shipUpLeft);
	if (shipDownLeft)
		al_destroy_bitmap(shipDownLeft);
	if (shipUpRight)
		al_destroy_bitmap(shipUpRight);
	if (shipDownRight)
		al_destroy_bitmap(shipDownRight);
 
    if (event_queue)
        al_destroy_event_queue(event_queue);

	//if (path)
	//	al_destroy_path(path);
}

void update_logic()
{
	//Increment/Decrement the speed depending on what keys are currently pressed
	if (setUp)
		if (setLeft) {
			if (shipY>0) shipY -= diagSpeed;
			if (shipX>0) shipX -= diagSpeed;
		}
		else if (setRight) {
			if (shipY>0) shipY -= diagSpeed;
			if (shipX<maxX-42) shipX += diagSpeed;
		}
		else {
			if (shipY>0) shipY -= strSpeed;
		}
	if (setDown)
		if (setLeft) {
			if (shipY<maxY-42) shipY += diagSpeed;
			if (shipX>0) shipX -= diagSpeed;
		}
		else if (setRight) {
			if (shipY<maxY-42) shipY += diagSpeed;
			if (shipX<maxX-42) shipX += diagSpeed;
		}
		else {
			if (shipY<maxY-42) shipY += strSpeed;
		}
	if (setLeft) {
		if (!setUp && !setDown)
			if (shipX>0) shipX -= strSpeed;
	}
	if (setRight) {
		if (!setUp && !setDown)
			if (shipX<maxX-42) shipX += strSpeed;
	}

	//Set which screen the ship is currently in
	bgX = -(int(float(shipX)/screenX)*screenX);
	bgY = -(int(float(shipY)/screenY)*screenY);
}

void check_key(ALLEGRO_EVENT e)
{
	if (e.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		done = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
		setLeft = true;
		if (setUp) {
			shipCurrent = shipUpLeft;
		}
		else if (setDown) {
			shipCurrent = shipDownLeft;
		}
		else {
			shipCurrent = shipLeft;
		}
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        setRight = true;
		if (setUp) {
			shipCurrent = shipUpRight;
		}
		else if (setDown) {
			shipCurrent = shipDownRight;
		}
		else {
			shipCurrent = shipRight;
		}
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
        setUp = true;
		if (setLeft) {
			shipCurrent = shipUpLeft;
		}
		else if (setRight) {
			shipCurrent = shipUpRight;
		}
		else {
			shipCurrent = shipUp;
		}
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        setDown = true;
		if (setLeft) {
			shipCurrent = shipDownLeft;
		}
		else if (setRight) {
			shipCurrent = shipDownRight;
		}
		else {
			shipCurrent = shipDown;
		}
    }
}

void clear_key(ALLEGRO_EVENT e)
{
	if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        setLeft = false;
		if (setUp)
			shipCurrent = shipUp;
		else if (setDown)
			shipCurrent = shipDown;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        setRight = false;
		if (setUp)
			shipCurrent = shipUp;
		else if (setDown)
			shipCurrent = shipDown;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
        setUp = false;
		if (setLeft)
			shipCurrent = shipLeft;
		else if (setRight)
			shipCurrent = shipRight;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        setDown = false;
		if (setLeft)
			shipCurrent = shipLeft;
		else if (setRight)
			shipCurrent = shipRight;
    }
}

void update_graphics()
{
	al_draw_bitmap(bg,bgX,bgY,0); //Draw background first
	al_draw_bitmap(shipCurrent,shipX%screenX,shipY%screenY,0); //Draw ship
}
 
void game_loop(void)
{
    bool redraw = true;
    al_start_timer(timer);
 
    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
 
        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            update_logic();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			slow = false;
            check_key(event);
        }
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			slow = true;
			clear_key(event);
		}
 
        if (redraw) { //&& al_is_event_queue_empty(event_queue)
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            update_graphics(); //For now, just redraw the background and draw the ship at its current pos
            al_flip_display();
        }
    }
}
 
int main(int argc, char* argv[])
{
    init();
    game_loop();
    shutdown();
}