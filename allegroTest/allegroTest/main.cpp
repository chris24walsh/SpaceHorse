#include <enet\enet.h> //Must be included before the allegro library to prevent conflicts
#include <allegro5/allegro5.h>
#include "allegro5/allegro_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <random>
#include <cmath>

using namespace std;

////////////////////////////////////
/////     GLOBAL VARIABLES     /////

//Allegro objects
bool done;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *backgroundSprite = NULL;
/*ALLEGRO_BITMAP *shipSprite = NULL;
ALLEGRO_BITMAP *shipSprite1 = NULL;
ALLEGRO_BITMAP *shipSprite2 = NULL;
ALLEGRO_BITMAP *shipSpriteCurrent = NULL;*/
const int maxFireballs = 15;
//ALLEGRO_BITMAP *fireball[maxFireballs];
ALLEGRO_BITMAP *dockingStation[3];
ALLEGRO_BITMAP *dockingText = NULL;
ALLEGRO_BITMAP *upgradedText = NULL;
ALLEGRO_BITMAP *buffer = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_DISPLAY_MODE disp_data;

//Screen vars
int gridX = 0;
int gridY = 0;
int numberGrids = 2; //Number of grids along one side of the square map actually...:)
int windowWidth, windowHeight, maxX, maxY, screenWidth, screenHeight;
int backgroundWidth = 1920;
int backgroundHeight = 1080;
int scaledWidth, scaledHeight, scaledOffsetX, scaledOffsetY;
bool leftPressed, rightPressed, upPressed, downPressed;

//Ship vars
/*int player[0]->x = 200;
int player[0]->y = 200;
int player[0]->height = 100;
int player[0]->width = 100;
float player[0]->angle = 0;
float player[0]->speed = 0;
int maxplayer[0]->speed = 7;
int flipflop = 0; */

//Fireball vars
/*int fireballNumber = 0;
int player[0]->fireWidth = 30;
int player[0]->fireHeight = 20;
int player[0]->fireX[maxFireballs];
int player[0]->fireY[maxFireballs];
float player[0]->fireAngle[maxFireballs];
float fireballSpeed = 17;*/

//Docking station vars
bool canDock, dockingScr;
const int maxDockingStations = 3;
int dockingX[maxDockingStations];
int dockingY[maxDockingStations];
int dockingWidth[maxDockingStations] = {250, 525, 190};
int dockingHeight[maxDockingStations] = {250, 525, 190};
bool safe = true;
bool dispUpgradeText = false;

//Network vars
bool useServer = true;
ENetHost * host; //This machine
ENetPeer * peer; //Remote machine


//////////////////////////////////
/////     Global Objects     /////

class Ship {
public:
	int x, y, height, width, speed, speedX, speedY, maxSpeed, flipflop, fireballNumber, fireX[maxFireballs], fireY[maxFireballs], fireHeight, fireWidth, fireSpeed;
	float angle, fireAngle[maxFireballs];
	ALLEGRO_BITMAP *shipSprite, *shipSprite1, *shipSprite2, *shipSpriteCurrent;
	ALLEGRO_BITMAP *fireSprite[maxFireballs];

	Ship() {
		x = y = 200;
		height = width = 100;
		speed = speedX = speedY = 0;
		maxSpeed = 7;
		angle = 0;
		flipflop = 0;
		shipSprite = shipSprite1 = shipSprite2 = shipSpriteCurrent = NULL;
		fireWidth = 30;
		fireHeight = 20;
		fireSpeed = 17;
		fireballNumber = 0;
		for (int i=0;i<maxFireballs;i++) {
			fireSprite[i] = NULL;
			fireX[i] = 2000;
			fireY[i] = 2000;
			fireAngle[i] = 0;
		}
	};
};


/////////////////////////////////////////
/////     Function Declarations     /////

//Program control functions declaration
void abort_game(const char*);
void init(Ship*[]);
void shutdown(Ship*[]);

//Game Functions declaration
void fire(Ship*[]);
void dock(Ship*[]);
void upgrade_weapon(Ship*[]);
void press_key(ALLEGRO_EVENT, Ship*[]);
void release_key(ALLEGRO_EVENT, Ship*[]);
void update_logic(Ship*[]);
void update_graphics(Ship*[]);
void game_loop(Ship*[]);

//Network Functions declaration
void connectServer();
void sendData();
int receiveData();

/////////////////////////////////
/////     Main function     /////

int main(int argc, char* argv[])
{
	Ship p1, p2;
	Ship *player1 = &p1;
	Ship *player2 = &p2;
	Ship *player[2] = {player1, player2};

    init(player);
    game_loop(player);
    shutdown(player);
}


//////////////////////////////////////////////
/////      Program control functions     /////

void abort_game(const char* message)
{
    printf("%s \n", message);
	_getch();
    exit(1);
}
 
void init(Ship *player[])
{
	//Allegro initialisation
    if (!al_init())
        abort_game("Failed to initialize allegro");
    if (!al_install_keyboard())
        abort_game("Failed to install keyboard");
    timer = al_create_timer(1.0 / 60);
    if (!timer)
        abort_game("Failed to create timer");
	
	//Setup Display
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	windowWidth = 640*2;
	windowHeight = 480*2;
	//windowWidth = disp_data.width;
	//windowHeight = disp_data.height;
	maxX = windowWidth*numberGrids;
	maxY = windowHeight*numberGrids;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(windowWidth, windowHeight);
    if (!display)
        abort_game("Failed to create display");

	//Calculate scaling factor
	float sx = float(windowWidth)/backgroundWidth;
	float sy = float(windowHeight)/backgroundHeight;
	float scale = sx<sy?sx:sy;
	scaledWidth = windowWidth * scale;
	scaledHeight = windowHeight * scale;
	scaledOffsetX = (windowWidth - scaledWidth) / 2;
	scaledOffsetY = (windowHeight - scaledHeight) / 2;

	//Initialise image addon
	if(!al_init_image_addon())
		abort_game("Failed to initialize al_init_image_addon");
	//Load bitmap files
	//buffer = al_load_bitmap("c:/dev/allegro/images/backgroundSprite.png");
	buffer = al_create_bitmap(windowWidth, windowHeight);
	backgroundSprite = al_load_bitmap("c:/dev/allegro/images/backgroundSprite.png"); //Load background image
	for (int p=0;p<2;p++) {
		player[p]->shipSprite = al_load_bitmap("c:/dev/allegro/images/shipSprite.png"); //Stationary ship sprite
		player[p]->shipSprite1 = al_load_bitmap("c:/dev/allegro/images/shipSprite1.png"); //Moving ship sprite 1
		player[p]->shipSprite2 = al_load_bitmap("c:/dev/allegro/images/shipSprite2.png"); // Moving ship sprite 2
		player[p]->shipSpriteCurrent = player[p]->shipSprite;
		//for (int i=0; i<maxFireballs; i++) fireball[i] = al_load_bitmap("c:/dev/allegro/images/fireball.png");
		for (int i=0; i<maxFireballs; i++) player[p]->fireSprite[i]= al_load_bitmap("c:/dev/allegro/images/fireball.png");
	}
	dockingStation[0] = al_load_bitmap("c:/dev/allegro/images/dockingStation1.png");
	dockingStation[1] = al_load_bitmap("c:/dev/allegro/images/dockingStation2.png");
	dockingStation[2] = al_load_bitmap("c:/dev/allegro/images/dockingStation3.png");
	dockingText = al_load_bitmap("c:/dev/allegro/images/dockingText.png");
	upgradedText = al_load_bitmap("c:/dev/allegro/images/upgradedText.png");
	
	//Check if bitmaps loaded properly
	if(!backgroundSprite) abort_game("Failed to load the background image");
	for (int p=0;p<2;p++) {
		if(!player[p]->shipSprite) abort_game("Failed to load the shipSprite image");
		if(!player[p]->shipSprite1) abort_game("Failed to load the shipSprite1 image");
		if(!player[p]->shipSprite2) abort_game("Failed to load the shipSprite2 image");
		//if(!fireball[0]) abort_game("Failed to load the fireball image");
		if(!player[p]->fireSprite) abort_game("Failed to load the fireball image");
	}
	if(!dockingStation[0]) abort_game("Failed to load the dockingStation1 image");
	if(!dockingStation[1]) abort_game("Failed to load the dockingStation2 image");
	if(!dockingStation[2]) abort_game("Failed to load the dockingStation3 image");
	if(!dockingText) abort_game("Failed to load the dockingText image");
	if(!upgradedText) abort_game("Failed to load the upgradedText image");
	
	//Initisatise the event queue
	event_queue = al_create_event_queue();
    if (!event_queue) abort_game("Failed to create event queue");
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
 
    done = false;

	//Set up fireballs outside map initially
	//for (int i=0; i<maxFireballs; i++) player[0]->fireX[i] = maxX + player[0]->fireWidth;
	//for (int i=0; i<maxFireballs; i++) player[0]->fireY[i] = maxY + player[0]->fireHeight;

	//Docking station initialisation
	canDock = dockingScr = false;
	//Random seed the dockingstations throughout the map
	srand(6); //srand(time(NULL));
	for (int i=0; i<3; i++) dockingX[i] = float(maxX)*(rand()%100)/100;
	for (int i=0; i<3; i++) dockingY[i] = float(maxY)*(rand()%100)/100;


	//Initialise the network library
	if (enet_initialize () != 0)
    {
        abort_game("An error occurred while initializing ENet.\n");
    }
    //atexit (enet_deinitialize); //deinitialise upon program exit

	//select server/client/singleplayer
	std::cout << "Do you want to set up server(1), client(2) or play singleplayer(3)?: ";
	int choice;
	std::cin >> choice;

	//Create server (or client as below, depending on whether useServer is true/false)
	if (choice==1) {
		ENetAddress address;
		//Bind server to localhost
		address.host = ENET_HOST_ANY;
		// Bind server to port 1234
		address.port = 1234;
		host = enet_host_create (& address /* the address to bind the server host to */, 
									 32      /* allow up to 32 clients and/or outgoing connections */,
									  2      /* allow up to 2 channels to be used, 0 and 1 */,
									  0      /* assume any amount of incoming bandwidth */,
									  0      /* assume any amount of outgoing bandwidth */);
		if (host == NULL) {
			abort_game("An error occurred while trying to create an ENet server host.\n");
		}
	}

	//Create client
	else if (choice==2) {
		host = enet_host_create (NULL /* create a client host */,
					1 /* only allow 1 outgoing connection */,
					2 /* allow up 2 channels to be used, 0 and 1 */,
					0	/* 57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
					0	/* 14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
		if (host == NULL) {
			abort_game("An error occurred while trying to create an ENet client host.\n");
		}
		//Begin connection to server machine
		connectServer();
	}
	else ; //Single player mode (just don't setup server or client!)
}
 
void shutdown(Ship *player[])
{
	//Allegro shutdown
    if (timer) al_destroy_timer(timer);
    if (display) al_destroy_display(display);
	if (backgroundSprite)	al_destroy_bitmap(backgroundSprite);
	for (int p=0;p<2;p++) {
		if (player[p]->shipSprite) al_destroy_bitmap(player[p]->shipSprite);
		if (player[p]->shipSprite1) al_destroy_bitmap(player[p]->shipSprite1);
		if (player[p]->shipSprite2) al_destroy_bitmap(player[p]->shipSprite2);
		//if (player[0]->shipSpriteCurrent) al_destroy_bitmap(player[0]->shipSpriteCurrent);
		for (int i=0; i<maxFireballs; i++)  if (player[p]->fireSprite) al_destroy_bitmap(player[p]->fireSprite[i]);
	}
    if (event_queue) al_destroy_event_queue(event_queue);

	//Networking shutdown
	enet_deinitialize();
	enet_host_destroy(host);
}


//////////////////////////////////
/////     Game functions     /////

void fire(Ship *player[])
{
	int x = player[0]->fireballNumber++; //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
	player[0]->fireX[x] = player[0]->x%windowWidth;
	player[0]->fireY[x] = player[0]->y%windowHeight;
	player[0]->fireAngle[x] = player[0]->angle;
	if (player[0]->fireballNumber>=maxFireballs){
		player[0]->fireballNumber = 0;
	}
}

void dock(Ship *player[])
{
	if (dockingScr) dockingScr = false; //Undock, if already docked and docking activated
	else if (canDock) { //Else, if able to dock, do so, stop ship and set it to safe mode
		dockingScr = true;
		player[0]->speed = 0;
		safe = true;
	}
}

void upgrade_weapon(Ship *player[])
{
	if (dockingScr) {
		dispUpgradeText = true;
		//for (int i=0; i<maxFireballs; i++) fireball[i] = al_load_bitmap("c:/dev/allegro/images/fireball2.png");
		for (int i=0; i<maxFireballs; i++) player[0]->fireSprite[i] = al_load_bitmap("c:/dev/allegro/images/fireball2.png");
		player[0]->fireHeight = 40;
	}
}

void press_key(ALLEGRO_EVENT e, Ship *player[])
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
		fire(player);
	}
	if (e.keyboard.keycode == ALLEGRO_KEY_D) {
		dock(player);
	}
	if (e.keyboard.keycode == ALLEGRO_KEY_W) {
		upgrade_weapon(player);
	}
}

void release_key(ALLEGRO_EVENT e, Ship *player[])
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

void update_logic(Ship *player[])
{
	//Rotate clockwise or anti-clockwise
	if (leftPressed) {
		player[0]->angle-=0.05;
	}
	if (rightPressed) {
		player[0]->angle+=0.05;
	}

	//Cycle between different rocket sprites, to give effect of rocket blasting
	if (player[0]->speed>0) {
		if (player[0]->flipflop<5) player[0]->shipSpriteCurrent = player[0]->shipSprite1;
		else if (player[0]->flipflop>=5) player[0]->shipSpriteCurrent = player[0]->shipSprite2;
		player[0]->flipflop++;
		if (player[0]->flipflop==10) player[0]->flipflop=0;
	}
	else player[0]->shipSpriteCurrent = player[0]->shipSprite;

	//Increase or decrease player[0]->speed
	if (upPressed) {
		if (player[0]->speed<player[0]->maxSpeed) player[0]->speed += 1;
	}
	else if (downPressed) {
		if (player[0]->speed>0) player[0]->speed -= 1;
	}

	//Resolve translations for x and y axis
	player[0]->speedX = player[0]->speed * cos(player[0]->angle);
	player[0]->speedY = player[0]->speed * sin(player[0]->angle);

	//Apply translation to ship
	//Restrict map in X axis (can't travel below 0, above maxX)
	if ((player[0]->x<player[0]->width/2 && player[0]->speedX<0) || (player[0]->x>maxX-(player[0]->width/2) && player[0]->speedX>0)) ; //Can't travel
	else player[0]->x += player[0]->speedX;
	
	//Restrict map in Y axis (can't travel below 0, above maxY)
	if ((player[0]->y<player[0]->height/2 && player[0]->speedY<0) || (player[0]->y>maxY-(player[0]->height/2) && player[0]->speedY>0)) ; //Can't travel
	else player[0]->y += player[0]->speedY;

	//Set which grid the ship is currently in
	gridX = int(float(player[0]->x)/windowWidth)*windowWidth;
	gridY = int(float(player[0]->y)/windowHeight)*windowHeight;
	//gridX = (player[0]->x/windowWidth)*windowWidth;
	//gridY = (player[0]->y/windowHeight)*windowHeight;
	
	//Set fireball position
	for (int i=0; i<maxFireballs; i++) player[0]->fireX[i] += player[0]->fireSpeed * cos(player[0]->fireAngle[i]);
	for (int i=0; i<maxFireballs; i++) player[0]->fireY[i] += player[0]->fireSpeed * sin(player[0]->fireAngle[i]);
	//player[0]->fireX += player[0]->fireSpeed * cos(player[0]->fireAngle);
	//player[0]->fireY += player[0]->fireSpeed * sin(player[0]->fireAngle);

	//Check if near docking station, and if so allow docking
	int proxX, proxY;
	for (int i=0; i<3; i++) {
		proxX = abs(player[0]->x - dockingX[i]);
		//if (proxX<0) proxX *= -1; //Make the distance a positive value
		proxY = abs(player[0]->y - dockingY[i]);
		//if (proxY<0) proxY *= -1; //Make the distance a positive value
		if (proxX<dockingWidth[i]/2 && proxY<dockingHeight[i]/2) {
			canDock = true;
			break;
		}
		else canDock = false;
	}

}

void update_graphics(Ship *player[])
{
	//Space mode
	if (!dockingScr) {
		al_draw_bitmap(backgroundSprite,0,0,0); //Draw background first, unscaled
		for (int i=0; i<3; i++) //Draw planets next, only if ship's position is within current grid
			if (dockingX[i]>gridX && dockingX[i]<(gridX+windowWidth) && dockingY[i]>gridY && dockingY[i]<(gridY+windowHeight))
				al_draw_rotated_bitmap(dockingStation[i], dockingWidth[i]/2, dockingHeight[i]/2, dockingX[i]%windowWidth, dockingY[i]%windowHeight, 0, 0); //Draw planets only if their coordinates exist within current screen
		for (int p=0;p<2;p++) {
			//for (int i=0; i<maxFireballs; i++) al_draw_rotated_bitmap(fireball[i], player[0]->fireWidth/2, player[0]->fireHeight/2, player[0]->fireX[i], player[0]->fireY[i], player[0]->fireAngle[i], 0); //Draw fireballs
			for (int i=0; i<maxFireballs; i++)  al_draw_rotated_bitmap(player[p]->fireSprite[i], player[p]->fireWidth/2, player[p]->fireHeight/2, player[p]->fireX[i], player[p]->fireY[i], player[p]->fireAngle[i], 0); //Draw fireballs
			al_draw_rotated_bitmap(player[p]->shipSpriteCurrent, player[p]->width/2, player[p]->height/2, player[p]->x%windowWidth, player[p]->y%windowHeight, player[p]->angle, 0); //Draw ship (at a rotation)
		}
	}

	//Docking mode
	else {
		al_clear_to_color(al_map_rgb(25,0,25));
		al_draw_rotated_bitmap(player[0]->shipSprite, player[0]->width/2, player[0]->height/2, 250, 250, 0, 0);
		al_draw_rotated_bitmap(player[0]->fireSprite[0], player[0]->fireHeight/2, player[0]->fireWidth/2, 250, 250, 0, 0);
		al_draw_bitmap(dockingText, 0, 0, 0);
		if (dispUpgradeText) al_draw_bitmap(upgradedText, 0, windowHeight/2, 0);
	}
}
 
void game_loop(Ship *player[])
{
    bool redraw = true;
    al_start_timer(timer);
 
    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
 
        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            update_logic(player);
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            press_key(event, player);
        }
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			release_key(event, player);
		}

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
			al_set_target_bitmap(buffer);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            update_graphics(player); //Redraw background and sprites
			al_set_target_backbuffer(display);
            al_draw_scaled_bitmap(buffer, 0, 0, windowWidth, windowHeight, scaledOffsetX, scaledOffsetY, scaledWidth, scaledHeight, 0);
            al_flip_display();
        }
    }
}
 

/////////////////////////////////////
/////     Network functions     /////

//Connect to server
void connectServer() {
	ENetAddress address;
	ENetEvent event;
	//ENetPeer *peer;
	/* Connect to some.server.net:1234. */
	enet_address_set_host (& address, "192.168.8.102");
	address.port = 1234;
	/* Initiate the connection, allocating the two channels 0 and 1. */
	peer = enet_host_connect (host, & address, 2, 0);    
	if (peer == NULL)
	{
	   fprintf (stderr, 
				"No available peers for initiating an ENet connection.\n");
	   exit (EXIT_FAILURE);
	}
	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service (host, & event, 7000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		puts("Connection to some.server.net:1234 succeeded.");
	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset (peer);
		puts ("Connection to some.server.net:1234 failed.");
	}
}

//Sending data
void sendData() {
	/* Create a reliable packet of size 7 containing "packet\0" */
	ENetPacket * packet = enet_packet_create ("packet", strlen ("packet") + 1, ENET_PACKET_FLAG_RELIABLE);
	/* Extend the packet so and append the string "foo", so it now */
	/* contains "packetfoo\0"                                      */
	enet_packet_resize (packet, strlen ("packetfoo") + 1);
	//strcpy (& packet -> data [strlen ("packet")], "foo");
	/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */
	enet_peer_send (peer, 0, packet);
	/* One could just use enet_host_service() instead. */
	enet_host_flush (host);
}

//Receiving data
int receiveData() {
	ENetEvent event;
	// Non-blocking event poll
	while (enet_host_service (host, & event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf ("A new client connected from %x:%u.\n", 
					event.peer -> address.host,
					event.peer -> address.port);
			/* Store any relevant client information here. */
			event.peer -> data = "Client information";
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet -> dataLength,
					event.packet -> data,
					event.peer -> data,
					event.channelID);
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy (event.packet);
			return 1;
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			printf ("%s disconnected.\n", event.peer -> data);
			/* Reset the peer's client information. */
			event.peer -> data = NULL;
		}
	}
	return 0;
}