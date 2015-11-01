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
ALLEGRO_BITMAP *ship = NULL;
ALLEGRO_BITMAP *ship1 = NULL;
ALLEGRO_BITMAP *ship2 = NULL;
ALLEGRO_BITMAP *shipCurrent = NULL;
ALLEGRO_DISPLAY *display = NULL;
//ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
int shipX = 200;
int shipY = 200;
int shipH = 100;
int shipW = 100;
float angle = 0;
float speed = 0;
int maxSpeed = 7;
int bgX = 0;
int bgY = 0;
int maxX = 3840;
int maxY = 2160;
int screenX = 1920;
int screenY = 1080;
bool setLeft, setRight, setUp, setDown;
int flipflop = 0;

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
 
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(screenX, screenY);
    if (!display)
        abort_game("Failed to create display");

	if(!al_init_image_addon())
		abort_game("Failed to initialize al_init_image_addon");

	bg = al_load_bitmap("c:/dev/allegro/images/bg2.jpg"); //Load background image
	ship = al_load_bitmap("c:/dev/allegro/images/ship.png");
	ship1 = al_load_bitmap("c:/dev/allegro/images/ship1.png");
	ship2 = al_load_bitmap("c:/dev/allegro/images/ship2.png");
	shipCurrent = ship;
	//al_append_path_component(path, "Images");
	//al_set_path_filename(path, "bg.png");
	//image = al_load_bitmap(al_path_cstr(path, '/'));

	if(!bg) abort_game("Failed to load the background image");
	if(!ship) abort_game("Failed to load the ship image");
	
	event_queue = al_create_event_queue();
    if (!event_queue) abort_game("Failed to create event queue");
 
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
 
    done = false;
}
 
void shutdown(void)
{
    if (timer) al_destroy_timer(timer);
    if (display) al_destroy_display(display);
	if (bg)	al_destroy_bitmap(bg);
	if (ship) al_destroy_bitmap(ship);	
    if (event_queue) al_destroy_event_queue(event_queue);
	//if (path) al_destroy_path(path);
}

void press_key(ALLEGRO_EVENT e)
{
	if (e.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		done = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
		setLeft = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        setRight = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
        setUp = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        setDown = true;
    }
}

void release_key(ALLEGRO_EVENT e)
{
	if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        setLeft = false;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        setRight = false;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
        setUp = false;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        setDown = false;
    }
}

void update_logic()
{
	//Rotate clockwise or anti-clockwise
	if (setLeft) {
		angle-=0.05;
	}
	if (setRight) {
		angle+=0.05;
	}

	//Cycle between different rocket sprites, to give effect of rocket blasting
	if (speed>0) {
		if (flipflop<5) shipCurrent = ship1;
		else if (flipflop>=5) shipCurrent = ship2;
		flipflop++;
		if (flipflop==10) flipflop=0;
	}

	//Increase or decrease speed
	if (setUp) {
		if (speed<maxSpeed) speed += 1;
	}
	if (setDown) {
		flipflop = 0;
		shipCurrent = ship;
		if (speed>0) speed -= 1;
	}

	//Apply translation to ship, if it veers off screen translate it to other side of screen
	if (shipX>60) shipX += speed * cos(angle);
	if (shipY>60) shipY += speed * sin(angle);
	//if (shipX>(shipW/2) && shipX<(maxX-(shipW/2))) shipX += speed * cos(angle);
	//if (shipY>(shipH/2) && shipY<(maxY-(shipH/2))) shipY += speed * sin(angle);
	
	//Set which screen the ship is currently in
	bgX = -(int(float(shipX)/screenX)*screenX);
	bgY = -(int(float(shipY)/screenY)*screenY);
}

void update_graphics()
{
	al_draw_bitmap(bg,bgX,bgY,0); //Draw background first
	al_draw_rotated_bitmap(shipCurrent, 50, 50, shipX%screenX, shipY%screenY, angle, 0); //Draw ship (at a rotation)
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
            press_key(event);
        }
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			release_key(event);
		}
 
        if (redraw && al_is_event_queue_empty(event_queue)) {
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