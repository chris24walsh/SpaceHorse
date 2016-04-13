#include "Game.h"

Game::Game(int width, int height, ALLEGRO_TIMER &timer, ALLEGRO_EVENT_QUEUE &event_queue)
	:players(1, Player()),
	map(),
	display(width, height),
	logic()
{   
	//load map
	map.makeSolarSystem();

	//load logic
	logic.load(display, players, map);

	//finally, start the game loop
	gameLoop(timer, event_queue);
}

void Game::gameLoop(ALLEGRO_TIMER &timer, ALLEGRO_EVENT_QUEUE &event_queue)
{
	bool redraw = true;
	al_start_timer(&timer);

	while (!logic.getDone())
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(&event_queue, &event);

        switch(event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
            logic.update();
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			logic.pressKey(event, display);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			logic.releaseKey(event);
			break;
		}

        if (redraw && al_is_event_queue_empty(&event_queue))
		{
            redraw = false;
            display.update();
			al_flip_display();
        }
    }
}

Game::~Game(void)
{
	for(int i=0; i<players.size(); ++i)
	{ players.at(i).getShip().shutdown(); }
}