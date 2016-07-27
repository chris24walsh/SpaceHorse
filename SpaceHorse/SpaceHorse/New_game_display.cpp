#include "NEW_GAME_DISPLAY.h"


New_game_display::New_game_display(int windowWidth, int windowHeight)
	:m_newGameLoad(0),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_textEntered(), //'new game' option lit up by default
	m_font(nullptr),
	m_music(nullptr)
{
}

void New_game_display::load() //load all the resources for this display screen
{
	if (!m_newGameLoad)
	{
		//Load font
		m_font = al_load_ttf_font("../../Font/pirulen.ttf", 35, 0);
		if (!m_font) { menuFail("Could not load 'pirulen.ttf'.\n"); }

		//Load audio
		m_music = al_load_sample("../../sounds/intro-music.wav");
		if (!m_music) { menuFail("Could not load 'intro-music.wav'.\n"); }
		al_play_sample(m_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

		m_newGameLoad = true;
	}
}

void New_game_display::unload() //free up all the resources for this display screen
{
	if (m_newGameLoad)
	{
		if (m_font) { al_destroy_font(m_font); m_font = nullptr; }
		if (m_music) { al_destroy_sample(m_music); m_music = nullptr; }

		m_newGameLoad = false;
	}
}

void New_game_display::update()
{
	if (!m_newGameLoad) { load(); }
	al_clear_to_color(al_map_rgb(30, 30, 30));
	int c1R, c1G, c1B,
		c2R, c2G, c2B;
	c1R = c1G = c1B = c2R = c2G = c2B = 190;

	//draw new game options
	al_draw_text(m_font, al_map_rgb(c1R, c1G, c1B), m_windowWidth*0.5, m_windowHeight*0.3, ALLEGRO_ALIGN_CENTRE, "ENTER A NAME FOR THE NEW GAME:");
	al_draw_text(m_font, al_map_rgb(c2R, c2G, c2B), m_windowWidth*0.5, m_windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, m_textEntered.c_str());
}

void New_game_display::menuFail(std::string failMessage)
{
	std::cout << failMessage << std::endl;
	_getch();
	exit(1);
}

void New_game_display::setTextEntered(std::string textEntered) { m_textEntered = textEntered; }

New_game_display::~New_game_display(void)
{
	if (m_newGameLoad)
	{
		if (m_font) { al_destroy_font(m_font); }
		m_font = nullptr;
		m_newGameLoad = false;
	}
}
