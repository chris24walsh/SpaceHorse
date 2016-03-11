#include "Game.h"

Game::Game(void)
	:FPS(60), timer(al_create_timer(1.0/FPS)), event_queue(al_create_event_queue())
{
	//Allegro initialisation
    if (!al_init())
	{ abortGame("Failed to initialize allegro"); }
    if (!al_install_keyboard())
	{ abortGame("Failed to install keyboard"); }
    if (!timer)
	{ abortGame("Failed to create timer"); }

	//Initialise image addon
	if(!al_init_image_addon()) 
	{ abortGame("Failed to initialise al_init_image_addon"); }
	
	//Initialise the font addon
	al_init_font_addon();
	if (!al_init_ttf_addon()) 
	{ abortGame("Failed to initalise al_init_ttf_addon"); }

	//Initialise primitives addon
	if (!al_init_primitives_addon()) 
	{ abortGame("Failed to initialise al_init_primitives_addon"); }
	
	//Initisatise the event queue
	if (!event_queue) { abortGame("Failed to create event queue"); }
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display.getDisplay()));
	
	//Initialise the network library
	/*if (enet_initialize () != 0)
    {
        abort_game("An error occurred while initializing ENet.\n");
    }*/

	//Initialise logic
	logic.load(display, players, map);

	//finally, start the game loop
	gameLoop();
}

void Game::abortGame(std::string message)
{
    printf("%s \n", message);
	_getch();
    exit(1);
}

void Game::gameLoop()
{
	bool redraw = true;
	while (!logic.getDone())
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
            logic.update();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) 
		{
            logic.pressKey(event);
        }
		else if (event.type == ALLEGRO_EVENT_KEY_UP) 
		{
			logic.releaseKey(event);
		}

        if (redraw && al_is_event_queue_empty(event_queue)) 
		{
            redraw = false;
            display.update(logic, players, map); //Should the logic object be doing this???
			al_flip_display();
        }
    }
}

Game::~Game(void)
{
	if (timer) al_destroy_timer(timer);
    if (event_queue) al_destroy_event_queue(event_queue);
}