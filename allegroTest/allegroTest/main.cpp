#include <enet\enet.h>
#include <allegro5/allegro5.h>
#include "allegro5/allegro_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <random>
#include <cmath>


bool done;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *backgroundSprite = NULL;
ALLEGRO_BITMAP *shipSprite = NULL;
ALLEGRO_BITMAP *shipSprite1 = NULL;
ALLEGRO_BITMAP *shipSprite2 = NULL;
ALLEGRO_BITMAP *shipSpriteCurrent = NULL;
const int maxFireballs = 15;
ALLEGRO_BITMAP *fireball[maxFireballs];
ALLEGRO_BITMAP *dockingStation[3];
ALLEGRO_BITMAP *dockingText = NULL;
ALLEGRO_BITMAP *upgradedText = NULL;
ALLEGRO_BITMAP *buffer = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_DISPLAY_MODE disp_data;

//ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);

//Screen vars
int gridX = 0;
int gridY = 0;
int numberGrids = 2; //Number of grids along one side of the square map actually...:)
int windowWidth, windowHeight, maxX, maxY, screenWidth, screenHeight;
int backgroundWidth = 1920;
int backgroundHeight = 1080;
int scaleX, scaleY, scaleW, scaleH;
bool leftPressed, rightPressed, upPressed, downPressed;

//Ship vars
int shipX = 200;
int shipY = 200;
int shipHeight = 100;
int shipWidth = 100;
float shipAngle = 0;
float shipSpeed = 0;
int mashipSpeedX = 7;
int flipflop = 0;

//Fireball vars
int fireballNumber = 0;
int fireballWidth = 30;
int fireballHeight = 20;
int fireballX[maxFireballs];
int fireballY[maxFireballs];
float fireballAngle[maxFireballs];
float fireballSpeed = 17;

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

//Network Functions
void connectServer();
void sendData();
int receiveData();

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
	
	//Setup Display
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	windowWidth = disp_data.width;
	windowHeight = disp_data.height;
	maxX = windowWidth*numberGrids;
	maxY = windowHeight*numberGrids;
	
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(windowWidth, windowHeight);
    if (!display)
        abort_game("Failed to create display");

	if(!al_init_image_addon())
		abort_game("Failed to initialize al_init_image_addon");

	//Load bitmap files
	buffer = al_load_bitmap("c:/dev/allegro/images/backgroundSprite.png");
	backgroundSprite = al_load_bitmap("c:/dev/allegro/images/backgroundSprite.png"); //Load background image
	shipSprite = al_load_bitmap("c:/dev/allegro/images/shipSprite.png");
	shipSprite1 = al_load_bitmap("c:/dev/allegro/images/shipSprite1.png");
	shipSprite2 = al_load_bitmap("c:/dev/allegro/images/shipSprite2.png");
	shipSpriteCurrent = shipSprite;
	for (int i=0; i<maxFireballs; i++) fireball[i] = al_load_bitmap("c:/dev/allegro/images/fireball.png");
	dockingStation[0] = al_load_bitmap("c:/dev/allegro/images/dockingStation1.png");
	dockingStation[1] = al_load_bitmap("c:/dev/allegro/images/dockingStation2.png");
	dockingStation[2] = al_load_bitmap("c:/dev/allegro/images/dockingStation3.png");
	dockingText = al_load_bitmap("c:/dev/allegro/images/dockingText.png");
	upgradedText = al_load_bitmap("c:/dev/allegro/images/upgradedText.png");
	
	//al_append_path_component(path, "Images");
	//al_set_path_filename(path, "backgroundSprite.png");
	//image = al_load_bitmap(al_path_cstr(path, '/'));

	//Random seed the dockingstation
	srand(6); //srand(time(NULL));
	for (int i=0; i<3; i++) dockingX[i] = float(maxX)*(rand()%100)/100;
	for (int i=0; i<3; i++) dockingY[i] = float(maxY)*(rand()%100)/100;

	if(!backgroundSprite) abort_game("Failed to load the background image");
	if(!shipSprite) abort_game("Failed to load the ship image");
	
	event_queue = al_create_event_queue();
    if (!event_queue) abort_game("Failed to create event queue");
 
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
 
    done = false;

	//Set up fireballs outside map initially
	for (int i=0; i<maxFireballs; i++) fireballX[i] = maxX + fireballWidth;
	for (int i=0; i<maxFireballs; i++) fireballY[i] = maxY + fireballHeight;

	//Docking off
	canDock = dockingScr = false;

	//Initialise the network library
	if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        //return EXIT_FAILURE;
    }
    //atexit (enet_deinitialize); //deinitialise upon program exit

	//Create server
	if (useServer) {
		ENetAddress address;
		/* Bind the server to the default localhost.     */
		/* A specific host address can be specified by   */
		/* enet_address_set_host (& address, "x.x.x.x"); */
		address.host = ENET_HOST_ANY;
		/* Bind the server to port 1234. */
		address.port = 1234;
		host = enet_host_create (& address /* the address to bind the server host to */, 
									 32      /* allow up to 32 clients and/or outgoing connections */,
									  2      /* allow up to 2 channels to be used, 0 and 1 */,
									  0      /* assume any amount of incoming bandwidth */,
									  0      /* assume any amount of outgoing bandwidth */);
		if (host == NULL)
		{
			fprintf (stderr, 
					 "An error occurred while trying to create an ENet server host.\n");
			//exit (EXIT_FAILURE);
		}
	}

	//Create client
	else {
		host = enet_host_create (NULL /* create a client host */,
					1 /* only allow 1 outgoing connection */,
					2 /* allow up 2 channels to be used, 0 and 1 */,
					0	/* 57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
					0	/* 14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
		if (host == NULL)
		{
			fprintf (stderr, 
					 "An error occurred while trying to create an ENet client host.\n");
			//exit (EXIT_FAILURE);
		}

		//Connect to the server machine
		connectServer();
	}
}
 
void shutdown(void)
{
    if (timer) al_destroy_timer(timer);
    if (display) al_destroy_display(display);
	if (backgroundSprite)	al_destroy_bitmap(backgroundSprite);
	if (shipSprite) al_destroy_bitmap(shipSprite);	
    if (event_queue) al_destroy_event_queue(event_queue);
	//if (path) al_destroy_path(path);

	//Network shutdown
	enet_deinitialize();
	enet_host_destroy(host);
}

//Game functions

void fire()
{
	int x = fireballNumber++; //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
	fireballX[x] = shipX%windowWidth;
	fireballY[x] = shipY%windowHeight;
	fireballAngle[x] = shipAngle;
	if (fireballNumber>=maxFireballs){
		fireballNumber = 0;
	}
}

void dock()
{
	if (dockingScr) dockingScr = false; //Undock, if already docked and docking activated
	else if (canDock) { //Else, if able to dock, do so, stop ship and set it to safe mode
		dockingScr = true;
		shipSpeed = 0;
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
		//std::cout << gridX << " " << gridY << "    " << dockingX[0] << " " << dockingY[0];
		int y = 0;
	}
	if (e.keyboard.keycode == ALLEGRO_KEY_W) {
		if (dockingScr) {
			dispUpgradeText = true;
			for (int i=0; i<maxFireballs; i++) fireball[i] = al_load_bitmap("c:/dev/allegro/images/fireball2.png");
			fireballHeight = 40;
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
		shipAngle-=0.05;
	}
	if (rightPressed) {
		shipAngle+=0.05;
	}

	//Cycle between different rocket sprites, to give effect of rocket blasting
	if (shipSpeed>0) {
		if (flipflop<5) shipSpriteCurrent = shipSprite1;
		else if (flipflop>=5) shipSpriteCurrent = shipSprite2;
		flipflop++;
		if (flipflop==10) flipflop=0;
	}
	else shipSpriteCurrent = shipSprite;

	//Increase or decrease shipSpeed
	if (upPressed) {
		if (shipSpeed<mashipSpeedX) shipSpeed += 1;
	}
	else if (downPressed) {
		if (shipSpeed>0) shipSpeed -= 1;
	}

	//Resolve translations for x and y axis
	int shipSpeedX = shipSpeed * cos(shipAngle);
	int shipSpeedY = shipSpeed * sin(shipAngle);

	//Apply translation to ship
	//Restrict map in X axis (can't travel below 0, above maxX)
	if ((shipX<shipWidth/2 && shipSpeedX<0) || (shipX>maxX-(shipWidth/2) && shipSpeedX>0)) ; //Can't travel
	else shipX += shipSpeedX;
	
	//Restrict map in Y axis (can't travel below 0, above maxY)
	if ((shipY<shipHeight/2 && shipSpeedY<0) || (shipY>maxY-(shipHeight/2) && shipSpeedY>0)) ; //Can't travel
	else shipY += shipSpeedY;

	//Set which grid the ship is currently in
	gridX = int(float(shipX)/windowWidth)*windowWidth;
	gridY = int(float(shipY)/windowHeight)*windowHeight;
	//gridX = (shipX/windowWidth)*windowWidth;
	//gridY = (shipY/windowHeight)*windowHeight;
	
	//Set fireball position
	for (int i=0; i<maxFireballs; i++) fireballX[i] += fireballSpeed * cos(fireballAngle[i]);
	for (int i=0; i<maxFireballs; i++) fireballY[i] += fireballSpeed * sin(fireballAngle[i]);

	//Check if near docking station
	int proxX, proxY;
	for (int i=0; i<3; i++) {
		proxX = abs(shipX - dockingX[i]);
		//if (proxX<0) proxX *= -1; //Make the distance a positive value
		proxY = abs(shipY - dockingY[i]);
		//if (proxY<0) proxY *= -1; //Make the distance a positive value
		if (proxX<dockingWidth[i]/2 && proxY<dockingHeight[i]/2) {
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
		al_draw_bitmap(backgroundSprite,0,0,0); //Draw background first, unscaled
		for (int i=0; i<3; i++) //Draw planets next, only if ship's position is within current grid
			if (dockingX[i]>gridX && dockingX[i]<(gridX+windowWidth) && dockingY[i]>gridY && dockingY[i]<(gridY+windowHeight))
				al_draw_rotated_bitmap(dockingStation[i], dockingWidth[i]/2, dockingHeight[i]/2, dockingX[i]%windowWidth, dockingY[i]%windowHeight, 0, 0); //Draw planets only if their coordinates exist within current screen
		for (int i=0; i<maxFireballs; i++) al_draw_rotated_bitmap(fireball[i], fireballWidth/2, fireballHeight/2, fireballX[i], fireballY[i], fireballAngle[i], 0); //Draw fireballs
		al_draw_rotated_bitmap(shipSpriteCurrent, shipWidth/2, shipHeight/2, shipX%windowWidth, shipY%windowHeight, shipAngle, 0); //Draw ship (at a rotation)
	}

	//Docking mode
	else {
		al_clear_to_color(al_map_rgb(25,0,25));
		al_draw_rotated_bitmap(shipSprite, shipWidth/2, shipHeight/2, 250, 250, 0, 0);
		al_draw_rotated_bitmap(fireball[0], fireballHeight/2, fireballWidth/2, 250, 250, 0, 0);
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
			//al_set_target_bitmap(buffer);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            update_graphics(); //Redraw background and sprites
			//al_set_target_backbuffer(display);
			//al_draw_bitmap(buffer, 0, 0, 0);
			//al_draw_scaled_bitmap(buffer, 0, 0, backgroundWidth, backgroundHeight, scaleX, scaleY, scaleW, scaleH, 0);
            //al_draw_scaled_bitmap(buffer, 0, 0, windowWidth, windowHeight, scaleX, scaleY, scaleW, scaleH, 0);
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

//Network functions
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
		puts ("Connection to some.server.net:1234 succeeded.");
		//
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
			return 0;
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
			return 0;
		}
	}
}
