#include "Space_display.h"

Space_display::Space_display(int windowWidth, int windowHeight)
	:m_gameOver(0),
	m_hyperDrive(0),
	m_spaceLoad(0),
	m_gridX(0),
	m_gridY(0),
	m_bgX(0),
	m_bgY(0),
	m_screenWidth(0),
	m_screenHeight(0),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_shipSpriteCurrents(1, 0),
	m_editText(""),
	m_backgroundSprite(nullptr),
	m_radarSprite(nullptr),
	m_radarDotSprite(nullptr),
	m_radarBuffer(nullptr),
	m_players(),
	m_map(),
	font(nullptr),
	m_numberGrids(1000000), 
	m_radarScreenWidth(1920),
	m_radarScreenHeight(1080),
	m_radarScale(0.005),
	m_shipSprites(0),
	m_shipSprite1s(0),
	m_shipSprite2s(0),
	m_fireSprites(0),
	m_planetSprites(0),
	music(nullptr),
	fireball(nullptr)
{
}

void Space_display::load(std::vector<Player> &players, Map &map)
{
	if(!m_spaceLoad)
	{
		m_players = &players;
		m_map = &map;
		
		//load background
		m_backgroundSprite = al_load_bitmap("../../images/backgroundSprite3.png");
		if(!m_backgroundSprite) { spaceFail("Failed to load the background image"); }
		
		//load font
		font = al_load_ttf_font("../../Font/pirulen.ttf",25,0);
		if(!font) { spaceFail("Could not load 'pirulen.ttf'.\n"); }
		
		//load ships
		m_shipSpriteCurrents.reserve(players.size());
		m_shipSprites.reserve(players.size());
		m_shipSprite1s.reserve(players.size());
		m_shipSprite2s.reserve(players.size());
		m_fireSprites.reserve(players.size());
	
		for(int i=0;i<players.size();++i)
		{
		m_shipSprites.push_back(al_load_bitmap(players.at(i).getShip().getShipSprite().c_str()));
		if(!m_shipSprites.at(i)) { spaceFail("Failed to load the shipSprite image"); }
		m_shipSprite1s.push_back(al_load_bitmap(players.at(i).getShip().getShipSprite1().c_str()));
		if(!m_shipSprite1s.at(i)) { spaceFail("Failed to load the shipSprite1 image"); }
		m_shipSprite2s.push_back(al_load_bitmap(players.at(i).getShip().getShipSprite2().c_str()));
		if(!m_shipSprite2s.at(i)) { spaceFail("Failed to load the shipSprite2 image"); }
		m_fireSprites.push_back(al_load_bitmap(players.at(i).getShip().getFireSprite().c_str()));
		if(!m_fireSprites.at(i)) { spaceFail("Failed to load the fireball image"); }
		}

		//load planets
		m_planetSprites.reserve(map.getPlanets().size());
		for(int i=0;i<map.getPlanets().size();++i)
		{
			m_planetSprites.push_back(al_load_bitmap(map.getPlanets().at(i).getPlanetSprite().c_str()));
			if(!m_planetSprites.at(i)) 
			{ 
				std::stringstream failMessage;
				failMessage << "Failed to load the planet" << (i+1) << " image";
				spaceFail(failMessage.str());
			}
		}

		//load radar
		m_radarBuffer = al_create_bitmap(m_windowWidth*3, m_windowHeight*3);
		m_radarSprite = al_load_bitmap("../../images/radar1.png");
		if(!m_radarSprite) { spaceFail("Failed to load the radar image"); }
		m_radarDotSprite = al_load_bitmap("../../images/radarDot.png");
		if(!m_radarDotSprite) { spaceFail("Failed to load the radar dot image"); }

		//load music
		music = al_load_sample("../../sounds/space-ambient.wav");
		if(!music) { spaceFail("Could not load 'space-ambient.wav'.\n"); }
		al_play_sample(music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

		m_spaceLoad = true;
	}
}

void Space_display::unload()
{
	if(m_spaceLoad)
	{
		if(m_backgroundSprite) { al_destroy_bitmap(m_backgroundSprite); } 
		for(int i=(*m_players).size()-1;i>-1;--i) 
		{
			if(m_shipSprites.at(i)) { al_destroy_bitmap(m_shipSprites.at(i)); m_shipSprites.pop_back(); }
			if(m_shipSprite1s.at(i)) { al_destroy_bitmap(m_shipSprite1s.at(i)); m_shipSprite1s.pop_back(); }
			if(m_shipSprite2s.at(i)) { al_destroy_bitmap(m_shipSprite2s.at(i)); m_shipSprite2s.pop_back(); }
			if(m_fireSprites.at(i)) { al_destroy_bitmap(m_fireSprites.at(i)); m_fireSprites.pop_back(); }
		}
		for(int i=(*m_map).getPlanets().size()-1;i>-1;--i)
		{
			if(m_planetSprites.at(i)) { al_destroy_bitmap(m_planetSprites.at(i)); m_planetSprites.pop_back(); }
		}
		if(m_radarSprite) { al_destroy_bitmap(m_radarSprite); }
		if(m_radarDotSprite) { al_destroy_bitmap(m_radarDotSprite); }
		if(font) { al_destroy_font(font); font = nullptr; }
		if (music) { al_destroy_sample(music); music = nullptr; }
		
		m_spaceLoad = false;
	}
}

void Space_display::update()
{
	if(!m_spaceLoad) { load((*m_players), (*m_map)); }
	//Clear display first
	al_clear_to_color(al_map_rgb(0, 0, 0));
	
/*Remember that all sprites drawn to the display must be drawn relative to the current background 
coordinates (the camera), bgX and bgY - so subtract their x and y coordinates from bgX and bgY*/
	
	//Draw all 9 of the background sprites, tiled around, and including, the current grid
	int backX = m_gridX-m_windowWidth; //X coordinate of top left corner tile of the 9 tiles
	int backY = m_gridY-m_windowHeight; //Y coordinate of top left corner tile of the 9 tiles
	for (int x=0; x<3; x++)
	{
		for (int y=0; y<3; y++)
		{ al_draw_bitmap(m_backgroundSprite, backX + m_windowWidth*x - m_bgX, backY + m_windowHeight*y - m_bgY,0); }
	}

	//Draw planets
	for (int i=0; i<(*m_map).getPlanets().size(); i++)
	{
		al_draw_tinted_scaled_rotated_bitmap(m_planetSprites.at(i),
			al_map_rgb((*m_map).getPlanets().at(i).getColor1(), (*m_map).getPlanets().at(i).getColor2(), (*m_map).getPlanets().at(i).getColor3()), 
			getPlanetSpriteWidth(i)/2, getPlanetSpriteHeight(i)/2,
			(*m_map).getPlanets().at(i).getX()-m_bgX, (*m_map).getPlanets().at(i).getY()-m_bgY, 
			(*m_map).getPlanets().at(i).getPlanetScale(), (*m_map).getPlanets().at(i).getPlanetScale(), 0, 0);
	} //Draw planets only if their coordinates exist within current screen
	
	//Draw sprites for each Ship object
	for (int i=0; i<(*m_players).size(); ++i)
	{
		//Draw fireballs
		for (int f=0; f<(*m_players).at(i).getShip().getMaxFireBalls(); ++f)
		{
			al_draw_tinted_rotated_bitmap(m_fireSprites.at(i), al_map_rgb(255,255,255),
				getFireSpriteWidth(i)/2, getFireSpriteHeight(i)/2,
				(*m_players).at(i).getShip().getFireX(f)-m_bgX, (*m_players).at(i).getShip().getFireY(f)-m_bgY,
				(*m_players).at(i).getShip().getFireAngle(f), 0);
		}
		//Draw ships
		switch(m_shipSpriteCurrents.at(i))
		{
		case 0:
			{
				al_draw_rotated_bitmap(m_shipSprites.at(i), getShipSpriteWidth(i)/2, getShipSpriteHeight(i)/2,
					(*m_players).at(i).getShip().getX()-m_bgX, (*m_players).at(i).getShip().getY()-m_bgY,
					(*m_players).at(i).getShip().getAngle(), 0);
			}
			break;
		case 1:
			{
				al_draw_rotated_bitmap(m_shipSprite1s.at(i), getShipSpriteWidth(i)/2, getShipSpriteHeight(i)/2,
					(*m_players).at(i).getShip().getX()-m_bgX, (*m_players).at(i).getShip().getY()-m_bgY,
					(*m_players).at(i).getShip().getAngle(), 0);
			}
			break;
		case 2:
			{
				al_draw_rotated_bitmap(m_shipSprite2s.at(i), getShipSpriteWidth(i)/2, getShipSpriteHeight(i)/2,
					(*m_players).at(i).getShip().getX()-m_bgX, (*m_players).at(i).getShip().getY()-m_bgY,
					(*m_players).at(i).getShip().getAngle(), 0);
			}
			break;
		}
	}
	
	//Draw bordering rectangle //Not necessary with current gigantic map, but useful to debug a small bounded area
	//al_draw_rectangle(0-bgX, 0-bgY, maxX-bgX, maxY-bgY, al_map_rgb(255, 255, 255), 10);

	//Draw stats to screen
	std::stringstream ss_health;
	ss_health << "Health: " << (*m_players).at(0).getShip().getHealth();
	std::string health = ss_health.str(); //good, don't try mixing stringstream with c-style strings - char* is just pointer to array!
	std::stringstream ss_coordinates;
	ss_coordinates << "Coordinates: " << (*m_players).at(0).getShip().getX() << " , " << (*m_players).at(0).getShip().getY();
	std::string coordinates = ss_coordinates.str();
	al_draw_text(font, al_map_rgb(255,255,255), m_windowWidth*0.05, m_windowHeight*0.1, 0, health.c_str());
	al_draw_text(font, al_map_rgb(255,255,255), m_windowWidth*0.05, m_windowHeight*0.9, 0, coordinates.c_str());

	//Draw radar
	al_draw_scaled_rotated_bitmap(m_radarSprite, m_radarScreenWidth/2, m_radarScreenHeight/2,
		m_windowWidth*0.85, m_windowHeight*0.15, 0.2, 0.2, 0, 0);

	//Planets on radar
	for (int i=0; i<((*m_map).getPlanets().size()); ++i)
	{
		double rX = m_windowWidth*0.85 - ((*m_players).at(0).getShip().getX() - ((*m_map).getPlanets().at(i)).getX()) * m_radarScale;
		double rY = m_windowHeight*0.15 - ((*m_players).at(0).getShip().getY() - ((*m_map).getPlanets().at(i)).getY()) * m_radarScale;
		if ((rX > m_windowWidth*0.85 - m_windowWidth*0.1) && (rX < m_windowWidth*0.85 + m_windowWidth*0.1) &&
			(rY > m_windowHeight*0.15 - m_windowHeight*0.1) && (rY < m_windowHeight*0.15 + m_windowHeight*0.1))
		{ al_draw_rotated_bitmap(m_radarDotSprite, 2.5, 2.5, rX, rY, 0, 0); } //Draw planets only if their coordinates exist within current screen
	}

	//Ships on radar
	if((*m_players).size()>1)
	{
		for (int i=1; i<(*m_players).size(); ++i)
		{
			double rX = m_windowWidth*0.85 - ((*m_players).at(0).getShip().getX() - (*m_players).at(i).getShip().getX()) * m_radarScale;
			double rY = m_windowHeight*0.15 - ((*m_players).at(0).getShip().getY() - (*m_players).at(i).getShip().getY()) * m_radarScale;
			if ((rX > m_windowWidth*0.85 - m_windowWidth*0.1) && (rX < m_windowWidth*0.85 + m_windowWidth*0.1) &&
				(rY > m_windowHeight*0.15 - m_windowHeight*0.1) && (rY < m_windowHeight*0.15 + m_windowHeight*0.1))
				al_draw_tinted_rotated_bitmap(m_radarDotSprite, al_map_rgb(255,120,120), 2.5, 2.5, rX, rY, 0, 0);
		}
	}
	//draw player1's ship tinted green rather than red
	al_draw_tinted_rotated_bitmap(m_radarDotSprite, al_map_rgb(100,255,100), 2.5, 2.5, m_windowWidth*0.85, m_windowHeight*0.15, 0, 0);

	//Game over
	if (m_gameOver)
	{ 
		al_draw_text(font, al_map_rgb(250, 0, 20), m_windowWidth*0.5, m_windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, "GAME OVER"); 
	}

	//Hyper Drive input
	if (m_hyperDrive)
	{
		al_draw_text(font, al_map_rgb(255, 255, 255), m_windowWidth*0.5, m_windowHeight*0.35, ALLEGRO_ALIGN_CENTRE, "Enter coordinates: ");
		al_draw_text(font, al_map_rgb(255, 255, 255), m_windowWidth*0.5, m_windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, m_editText.c_str());
	}
}

int Space_display::getPlanetSpriteWidth(int index) { return al_get_bitmap_width(m_planetSprites.at(index)); }
int Space_display::getPlanetSpriteHeight(int index) { return al_get_bitmap_height(m_planetSprites.at(index)); }
int Space_display::getShipSpriteWidth(int index) { return al_get_bitmap_width(m_shipSprites.at(index)); }
int Space_display::getShipSpriteHeight(int index) { return al_get_bitmap_height(m_shipSprites.at(index)); }
int Space_display::getFireSpriteWidth(int index) { return al_get_bitmap_width(m_fireSprites.at(index)); }
int Space_display::getFireSpriteHeight(int index) { return al_get_bitmap_height(m_fireSprites.at(index)); }
int Space_display::getNumberGrids() { return m_numberGrids; }
void Space_display::setGridX() { m_gridX = int(float((*m_players).at(0).getShip().getX())/m_windowWidth)*m_windowWidth; }
void Space_display::setGridY() { m_gridY = int(float((*m_players).at(0).getShip().getY())/m_windowHeight)*m_windowHeight; }
void Space_display::setBgX() { m_bgX = (*m_players).at(0).getShip().getX() - m_windowWidth/2; }
void Space_display::setBgY() { m_bgY = (*m_players).at(0).getShip().getY() - m_windowHeight/2; }
void Space_display::setEditText(std::string editText) { m_editText = editText;}
void Space_display::setGameOver() { m_gameOver = true; }
void Space_display::setHyperDrive(bool hyperDrive) { m_hyperDrive = hyperDrive; }
void Space_display::setShipSpriteCurrents(int index, int shipSpriteCurrent)
{
	if(shipSpriteCurrent<0 || shipSpriteCurrent>2) { spaceFail("Invalid shipSpriteCurrent number"); }
	else { m_shipSpriteCurrents.at(index) = shipSpriteCurrent; }
}

void Space_display::spaceFail(std::string failMessage)
{
	std::cout << failMessage << std::endl;
	_getch();
    exit(1);
}

Space_display::~Space_display(void)
{
	if(m_spaceLoad)
	{
		if(m_backgroundSprite) { al_destroy_bitmap(m_backgroundSprite); } 
		for(int i=(*m_players).size()-1;i>-1;--i) 
		{
			if(m_shipSprites.at(i)) { al_destroy_bitmap(m_shipSprites.at(i)); m_shipSprites.pop_back(); }
			if(m_shipSprite1s.at(i)) { al_destroy_bitmap(m_shipSprite1s.at(i)); m_shipSprite1s.pop_back(); }
			if(m_shipSprite2s.at(i)) { al_destroy_bitmap(m_shipSprite2s.at(i)); m_shipSprite2s.pop_back(); }
			if(m_fireSprites.at(i)) { al_destroy_bitmap(m_fireSprites.at(i)); m_fireSprites.pop_back(); }
		}
		for(int i=(*m_map).getPlanets().size()-1;i>-1;--i)
		{
			if(m_planetSprites.at(i)) { al_destroy_bitmap(m_planetSprites.at(i)); m_planetSprites.pop_back(); }
		}
		if(m_radarSprite) { al_destroy_bitmap(m_radarSprite); }
		if(m_radarDotSprite) { al_destroy_bitmap(m_radarDotSprite); }
		if(font) { al_destroy_font(font); }
		m_spaceLoad = false;
	}
}
