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

	//draw menu options
	if (m_homeScreenOption == 1) al_draw_text(m_font, al_map_rgb(190, 190, 190), m_windowWidth*0.5, m_windowHeight*0.2, ALLEGRO_ALIGN_CENTRE, "NEW GAME");
	else al_draw_text(m_font, al_map_rgb(0, 0, 0), m_windowWidth*0.5, m_windowHeight*0.2, ALLEGRO_ALIGN_CENTRE, "NEW GAME");
	if (m_homeScreenOption == 2) al_draw_text(m_font, al_map_rgb(190, 190, 190), m_windowWidth*0.5, m_windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, "LOAD GAME");
	else al_draw_text(m_font, al_map_rgb(0, 0, 0), m_windowWidth*0.5, m_windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, "LOAD GAME");
	if (m_homeScreenOption == 3) al_draw_text(m_font, al_map_rgb(190, 190, 190), m_windowWidth*0.5, m_windowHeight*0.6, ALLEGRO_ALIGN_CENTRE, "SAVE GAME");
	else al_draw_text(m_font, al_map_rgb(0, 0, 0), m_windowWidth*0.5, m_windowHeight*0.6, ALLEGRO_ALIGN_CENTRE, "SAVE GAME");
	if (m_homeScreenOption == 4) al_draw_text(m_font, al_map_rgb(190, 190, 190), m_windowWidth*0.5, m_windowHeight*0.8, ALLEGRO_ALIGN_CENTRE, "QUIT");
	else al_draw_text(m_font, al_map_rgb(0, 0, 0), m_windowWidth*0.5, m_windowHeight*0.8, ALLEGRO_ALIGN_CENTRE, "QUIT");
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
