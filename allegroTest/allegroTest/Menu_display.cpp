#include "Menu_display.h"


Menu_display::Menu_display(void)
{
}

void Menu_display::load() //load all the resources for this display screen
{ 
	font = al_load_ttf_font("C:/Dev/allegro/Font/pirulen.ttf",25,0);
	if(!font) { menuFail("Could not load 'pirulen.ttf'.\n"); }
}

void Menu_display::unload() //free up all the resources for this display screen
{
	if(font) { al_destroy_font(font); }
}

void Menu_display::update(int homeScreenOption, int windowWidth, int windowHeight)
{
	al_clear_to_color(al_map_rgb(30,30,30));
	int c1R, c1G, c1B, c2R, c2G, c2B, c3R, c3G, c3B, c4R, c4G, c4B;
	c1R = c1G = c1B = c2R = c2G = c2B = c3R = c3G = c3B = c4R = c4G = c4B = 256;
	if (homeScreenOption == 1) c1R = c1G = c1B = 190;
	if (homeScreenOption == 2) c2R = c2G = c2B = 190;
	if (homeScreenOption == 3) c3R = c3G = c3B = 190;
	if (homeScreenOption == 4) c4R = c4G = c4B = 190;
	al_draw_text(font, al_map_rgb(c1R,c1G,c1B), windowWidth*0.5, windowHeight*0.2, ALLEGRO_ALIGN_CENTRE, "HOST MULTIPLAYER GAME");
	al_draw_text(font, al_map_rgb(c2R,c2G,c2B), windowWidth*0.5, windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, "JOIN MULTIPLAYER GAME");
	al_draw_text(font, al_map_rgb(c3R,c3G,c3B), windowWidth*0.5, windowHeight*0.6, ALLEGRO_ALIGN_CENTRE, "SINGLE PLAYER");
	al_draw_text(font, al_map_rgb(c4R,c4G,c4B), windowWidth*0.5, windowHeight*0.8, ALLEGRO_ALIGN_CENTRE, "QUIT");
}

void Menu_display::menuFail(std::string failMessage)
{
    printf("%s \n", failMessage);
	_getch();
    exit(1);
}

Menu_display::~Menu_display(void)
{
	if(font) { al_destroy_font(font); }
}
