#include "Header.h"
#include "Game.h"

void abort(std::string message)
{
    printf("%s \n", message);
	_getch();
    exit(1);
}

int main(int argc, char* argv[])
{
	//Allegro initialisation
    if (!al_init())
	{ abort("Failed to initialize allegro"); }
	
	const int FPS = 60;
	ALLEGRO_TIMER *timer;
	timer = al_create_timer(1.0/FPS);
	if (!timer)
	{ abort("Failed to create timer"); }

    if (!al_install_keyboard())
	{ abort("Failed to install keyboard"); }

	//Initialise image addon
	if(!al_init_image_addon()) 
	{ abort("Failed to initialise al_init_image_addon"); }
	
	//Initialise the font addon
	al_init_font_addon();
	if (!al_init_ttf_addon()) 
	{ abort("Failed to initalise al_init_ttf_addon"); }

	//Initialise primitives addon
	if (!al_init_primitives_addon()) 
	{ abort("Failed to initialise al_init_primitives_addon"); }

	//setup display
	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	int windowWidth = 1400;//disp_data.width;
	int windowHeight = 800;// disp_data.height;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	ALLEGRO_DISPLAY *display;
    display = al_create_display(windowWidth, windowHeight);
	if(!display) { abort("Failed to create display"); }
		
	//Initisatise the event queue
	ALLEGRO_EVENT_QUEUE *event_queue;
	event_queue = al_create_event_queue();
	if (!event_queue) { abort("Failed to create event queue"); }
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	//seed random
	srand(static_cast<unsigned int>(time(0))); //this is in effect globally, so start it here
	rand(); //junk first result because it is always about the same in Visual Studio

	//Initialise the network library
	/*if (enet_initialize () != 0)
    {
        abort_game("An error occurred while initializing ENet.\n");
    }*/

	Game game(windowWidth, windowHeight, *timer, *event_queue); //Create a game object which will start a game loop

	
	if (event_queue) al_destroy_event_queue(event_queue);
    if(display) al_destroy_display(display);
	if (timer) al_destroy_timer(timer);
	return 0;
}