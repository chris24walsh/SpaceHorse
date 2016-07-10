#include "Menu_display.h"


Menu_display::Menu_display(int windowWidth, int windowHeight)
	:m_menuLoad(0),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_homeScreenOption(1), //'new game' option lit up by default
	m_font(nullptr),
	m_music(nullptr)
{
}

void Menu_display::load() //load all the resources for this display screen
{
	if(!m_menuLoad)
	{
		//Load font
		m_font = al_load_ttf_font("../../Font/pirulen.ttf",35,0);
		if(!m_font) { menuFail("Could not load 'pirulen.ttf'.\n"); }

		//Load audio
		m_music = al_load_sample("../../sounds/intro-music.wav");
		if (!m_music) { menuFail("Could not load 'intro-music.wav'.\n"); }
		al_play_sample(m_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

		m_menuLoad = true;
	}
}

void Menu_display::unload() //free up all the resources for this display screen
{
	if(m_menuLoad)
	{
		if(m_font) { al_destroy_font(m_font); m_font = nullptr; }
		if (m_music) { al_destroy_sample(m_music); m_music = nullptr;  }
		
		m_menuLoad = false;
	}
}

void Menu_display::update()
{
	if(!m_menuLoad) { load(); }
	al_clear_to_color(al_map_rgb(30,30,30));
	int c1R, c1G, c1B,
		c2R, c2G, c2B,
		c3R, c3G, c3B;
	c1R = c1G = c1B = c2R = c2G = c2B = c3R = c3G = c3B = 0;

	switch(m_homeScreenOption) //light up the selected option
	{
	case 1:
		c1R = c1G = c1B = 190;
		break;
	case 2:
		c2R = c2G = c2B = 190;
		break;
	case 3:
		c3R = c3G = c3B = 190;
		break;
	}

	//draw menu options
	al_draw_text(m_font, al_map_rgb(c1R,c1G,c1B), m_windowWidth*0.5, m_windowHeight*0.3, ALLEGRO_ALIGN_CENTRE, "NEW GAME");
	al_draw_text(m_font, al_map_rgb(c2R,c2G,c2B), m_windowWidth*0.5, m_windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, "LOAD GAME");
	al_draw_text(m_font, al_map_rgb(c3R,c3G,c3B), m_windowWidth*0.5, m_windowHeight*0.7, ALLEGRO_ALIGN_CENTRE, "QUIT");
}

void Menu_display::menuFail(std::string failMessage)
{
	std::cout << failMessage << std::endl;
	_getch();
    exit(1);
}

void Menu_display::setHomeScreenOption(int homeScreenOption) { m_homeScreenOption = homeScreenOption; }

Menu_display::~Menu_display(void)
{
	if(m_menuLoad)
	{
		if(m_font) { al_destroy_font(m_font); }
		m_font = nullptr;
		m_menuLoad = false;
	}
}
