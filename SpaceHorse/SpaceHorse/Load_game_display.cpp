#include "Load_game_display.h"


Load_game_display::Load_game_display(int windowWidth, int windowHeight)
	:m_loadGameLoad(0),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_homeScreenOption(1), //'new game' option lit up by default
	m_font(nullptr),
	m_music(nullptr)
{
}

void Load_game_display::load() //load all the resources for this display screen
{
	if(!m_loadGameLoad)
	{
		//Load font
		m_font = al_load_ttf_font("../../Font/pirulen.ttf",35,0);
		if(!m_font) { loadGameFail("Could not load 'pirulen.ttf'.\n"); }

		//Load audio
		m_music = al_load_sample("../../sounds/intro-music.wav");
		if (!m_music) { loadGameFail("Could not load 'intro-music.wav'.\n"); }
		al_play_sample(m_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

		//Load vector with files in '../../Saves'
		std::string folder = "../../Saves";
		std::string search_path = folder + "/*.*";
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					savedGameName.push_back(fd.cFileName);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}

		savedGameName.erase(savedGameName.begin());
		savedGameName.push_back("BACK TO MENU");

		m_loadGameLoad = true;
	}
}

void Load_game_display::unload() //free up all the resources for this display screen
{
	if(m_loadGameLoad)
	{
		if(m_font) { al_destroy_font(m_font); m_font = nullptr; }
		if (m_music) { al_destroy_sample(m_music); m_music = nullptr;  }

		//Delete vector elements
		savedGameName.clear();
		
		m_loadGameLoad = false;
	}
}

void Load_game_display::update()
{
	if(!m_loadGameLoad) { load(); }
	al_clear_to_color(al_map_rgb(30,30,30));
	
	//draw menu options
	for (int i = 0; i < savedGameName.size(); i++) {
		if (i+1 == m_homeScreenOption) { //Highlight option
			al_draw_text(m_font, al_map_rgb(190, 190, 190), m_windowWidth * 0.5, (m_windowHeight * i/10) + (m_windowHeight * 0.3), ALLEGRO_ALIGN_CENTRE, savedGameName.at(i).c_str()); //Iterate through all the saved games, and display them as options
		}
		else { //Unhightlighted option
			al_draw_text(m_font, al_map_rgb(0, 0, 0), m_windowWidth * 0.5, (m_windowHeight * i / 10) + (m_windowHeight * 0.3), ALLEGRO_ALIGN_CENTRE, savedGameName.at(i).c_str()); //Iterate through all the saved games, and display them as options
		}
	}
}

void Load_game_display::loadGameFail(std::string failMessage)
{
	std::cout << failMessage << std::endl;
	_getch();
    exit(1);
}

void Load_game_display::setHomeScreenOption(int homeScreenOption) { m_homeScreenOption = homeScreenOption; }

int Load_game_display::getNumberSaveGames()
{
	return savedGameName.size();
}

Load_game_display::~Load_game_display(void)
{
	if(m_loadGameLoad)
	{
		if(m_font) { al_destroy_font(m_font); }
		m_font = nullptr;
		m_loadGameLoad = false;
	}
}
