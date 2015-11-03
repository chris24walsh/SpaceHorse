#include <allegro5/allegro5.h>
#include "allegro5/allegro_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <random>


bool done;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *bg = NULL;
ALLEGRO_BITMAP *ship = NULL;
ALLEGRO_BITMAP *ship1 = NULL;
ALLEGRO_BITMAP *ship2 = NULL;
ALLEGRO_BITMAP *shipCurrent = NULL;
const int maxFireballs = 15;
ALLEGRO_BITMAP *fireball[maxFireballs];
ALLEGRO_BITMAP *dockingStation[3];
ALLEGRO_BITMAP *dockingText;
ALLEGRO_BITMAP *upgradedText;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_DISPLAY_MODE disp_data;

//ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);

//Screen vars
int bgX = 0;
int bgY = 0;
int maxX;// = 3840;
int maxY;// = 2160;
int screenX;// = 1920;
int screenY;// = 1080;
bool leftPressed, rightPressed, upPressed, downPressed;
int flipflop = 0;

//Ship vars
int shipX = 200;
int shipY = 200;
int shipH = 100;
int shipW = 100;
float shipA = 0;
float shipS = 0;
int maxShipS = 7;

//Fireball vars
int fireballNumber = 0;
int fireW = 30;
int fireH = 20;
int fireX[maxFireballs];
int fireY[maxFireballs];
float fireA[maxFireballs] = {0};
float fireS = 17;

//Docking station vars
bool canDock, dockingScr;
int dockingX[3];
int dockingY[3];
int dockingW[3] = {250, 525, 190};
int dockingH[3] = {250, 525, 190};
bool safe = true;
bool dispUpgradeText = false;

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
 
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	screenX = disp_data.width;
	screenY = disp_data.height;
	maxX = screenX*10;
	maxY = screenY*10;
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(screenX, screenY);
    if (!display)
        abort_game("Failed to create display");

	if(!al_init_image_addon())
		abort_game("Failed to initialize al_init_image_addon");

	//Load bitmap files
	bg = al_load_bitmap("c:/dev/allegro/images/bg.png"); //Load background image
	ship = al_load_bitmap("c:/dev/allegro/images/ship.png");
	ship1 = al_load_bitmap("c:/dev/allegro/images/ship1.png");
	ship2 = al_load_bitmap("c:/dev/allegro/images/ship2.png");
	shipCurrent = ship;
	for (int i=0; i<maxFireballs; i++) fireball[i] = al_load_bitmap("c:/dev/allegro/images/fireball.png");
	dockingStation[0] = al_load_bitmap("c:/dev/allegro/images/dockingStation1.png");
	dockingStation[1] = al_load_bitmap("c:/dev/allegro/images/dockingStation2.png");
	dockingStation[2] = al_load_bitmap("c:/dev/allegro/images/dockingStation3.png");
	dockingText = al_load_bitmap("c:/dev/allegro/images/dockingText.png");
	upgradedText = al_load_bitmap("c:/dev/allegro/images/upgradedText.png");
	
	//al_append_path_component(path, "Images");
	//al_set_path_filename(path, "bg.png");
	//image = al_load_bitmap(al_path_cstr(path, '/'));

	//Random seed the dockingstation
	srand(6); //srand(time(NULL));
	for (int i=0; i<3; i++) dockingX[i] = float(maxX)*(rand()%100)/100;
	for (int i=0; i<3; i++) dockingY[i] = float(maxY)*(rand()%100)/100;

	if(!bg) abort_game("Failed to load the background image");
	if(!ship) abort_game("Failed to load the ship image");
	
	event_queue = al_create_event_queue();
    if (!event_queue) abort_game("Failed to create event queue");
 
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
 
    done = false;

	//Set up fireballs outside map initially
	for (int i=0; i<maxFireballs; i++) fireX[i] = maxX + fireW;
	for (int i=0; i<maxFireballs; i++) fireY[i] = maxY + fireH;

	//Docking off
	canDock = dockingScr = false;
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

//Game functions

void fire()
{
	int x = fireballNumber++; //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
	fireX[x] = shipX%screenX;
	fireY[x] = shipY%screenY;
	fireA[x] = shipA;
	if (fireballNumber>=maxFireballs){
		fireballNumber = 0;
	}
}

void dock()
{
	if (dockingScr) dockingScr = false; //Undock, if already docked and docking activated
	else if (canDock) { //Else, if able to dock, do so, stop ship and set it to safe mode
		dockingScr = true;
		shipS = 0;
		safe = true;
	}
}

void press_key(ALLEGRO_EVENT e)
{
	if (e.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
		done = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
		leftPressed = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        rightPressed = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
        upPressed = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        downPressed = true;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_F) {
		fire();
	}
	if (e.keyboard.keycode == ALLEGRO_KEY_D) {
		dock();
	}
	if (e.keyboard.keycode == ALLEGRO_KEY_E) {
		std::cout << dockingX << " " << dockingY << "     ";
	}
	if (e.keyboard.keycode == ALLEGRO_KEY_W) {
		if (dockingScr) {
			dispUpgradeText = true;
			for (int i=0; i<maxFireballs; i++) fireball[i] = al_load_bitmap("c:/dev/allegro/images/fireball2.png");
			fireH = 40;
		}
	}
}

void release_key(ALLEGRO_EVENT e)
{
	if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
        leftPressed = false;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
        rightPressed = false;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
        upPressed = false;
    }
	if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
        downPressed = false;
    }
}

void update_logic()
{
	//Rotate clockwise or anti-clockwise
	if (leftPressed) {
		shipA-=0.05;
	}
	if (rightPressed) {
		shipA+=0.05;
	}

	//Cycle between different rocket sprites, to give effect of rocket blasting
	if (shipS>0) {
		if (flipflop<5) shipCurrent = ship1;
		else if (flipflop>=5) shipCurrent = ship2;
		flipflop++;
		if (flipflop==10) flipflop=0;
	}
	else shipCurrent = ship;

	//Increase or decrease shipS
	if (upPressed) {
		if (shipS<maxShipS) shipS += 1;
	}
	else if (downPressed) {
		if (shipS>0) shipS -= 1;
	}

	//Resolve translations for x and y axis
	int xShipS = shipS * cos(shipA);
	int yShipS = shipS * sin(shipA);

	//Apply translation to ship
	//Restrict map in X axis (can't travel below 0, above maxX)
	if ((shipX<shipW/2 && xShipS<0) || (shipX>maxX-(shipW/2) && xShipS>0)) ; //Can't travel
	else shipX += xShipS;
	
	//Restrict map in Y axis (can't travel below 0, above maxY)
	if ((shipY<shipH/2 && yShipS<0) || (shipY>maxY-(shipH/2) && yShipS>0)) ; //Can't travel
	else shipY += yShipS;

	//Set which screen the ship is currently in
	bgX = int(float(shipX)/screenX)*screenX;
	bgY = int(float(shipY)/screenY)*screenY;
	
	//Set fireball position
	for (int i=0; i<maxFireballs; i++) fireX[i] += fireS * cos(fireA[i]);
	for (int i=0; i<maxFireballs; i++) fireY[i] += fireS * sin(fireA[i]);

	//Check if near docking station
	int proxX, proxY;
	for (int i=0; i<3; i++) {
		proxX = (shipX - dockingX[i]);
		if (proxX<0) proxX *= -1; //Make the distance a positive value
		proxY = (shipY - dockingY[i]);
		if (proxY<0) proxY *= -1; //Make the distance a positive value
		if (proxX<dockingW[i]/2 && proxY<dockingH[i]/2) {
			canDock = true;
			break;
		}
		else canDock = false;
	}

}

void update_graphics()
{
	//Space mode
	if (!dockingScr) {
		//al_draw_bitmap(bg,-bgX,-bgY,0); //Draw background first
		al_draw_bitmap(bg,0,0,0); //Draw background first
		for (int i=0; i<3; i++)
			if (dockingX[i]>bgX && dockingX[i]<(bgX+screenX) && dockingY[i]>bgY && dockingY[i]<(bgY+screenY))
				al_draw_rotated_bitmap(dockingStation[i], 125, 125, dockingX[i]%screenX, dockingY[i]%screenY, 0, 0); //Draw planets only if their coordinates exist within current screen
		for (int i=0; i<maxFireballs; i++) al_draw_rotated_bitmap(fireball[i], fireW/2, fireH/2, fireX[i], fireY[i], fireA[i], 0); //Draw fireballs
		al_draw_rotated_bitmap(shipCurrent, shipW/2, shipH/2, shipX%screenX, shipY%screenY, shipA, 0); //Draw ship (at a rotation)
	}

	//Docking mode
	else {
		al_clear_to_color(al_map_rgb(25,0,25));
		al_draw_rotated_bitmap(ship, shipW/2, shipH/2, 250, 250, 0, 0);
		al_draw_rotated_bitmap(fireball[0], fireH/2, fireW/2, 250, 250, 0, 0);
		al_draw_bitmap(dockingText, 700, 400, 0);
		if (dispUpgradeText) al_draw_bitmap(upgradedText, 700, 700, 0);
	}
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
            update_graphics(); //Redraw background and sprites
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