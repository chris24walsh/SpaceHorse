#include "Space_logic.h"


Space_logic::Space_logic()
	:m_space(),
	m_players(),
	m_map(),
	m_leftPressed(0),
	m_rightPressed(0),
	m_upPressed(0),
	m_downPressed(0),
	m_firePressed(0),
	m_gameOver(0),
	m_hyperDrive(0),
	m_paused(0),
	m_collided(0),
	m_textEntered(0),
	m_newKey(0),
	m_distance(0),
	m_maxX(0),
	m_maxY(0),
	m_editText("")
{
}

void Space_logic::load(Space_display &space, int windowWidth, int windowHeight, std::vector<Player> &players, Map &map)
{
	space.load(players, map);
	m_space = &space;
	m_players = &players;
	m_map = &map;
	m_maxX = windowWidth*space.getNumberGrids();
	m_maxY = windowHeight*space.getNumberGrids();

	//set size of planets, ships and fire according to loaded bitmaps
	for(int i=0;i<map.getPlanets().size();++i) 
	{ map.getPlanets().at(i).setPlanetSize(m_space->getPlanetSpriteWidth(i), m_space->getPlanetSpriteHeight(i)); }
	for(int i=0;i<players.size();++i)
	{
		players.at(i).getShip().setFireSize(m_space->getFireSpriteWidth(i), m_space->getFireSpriteHeight(i));
	}
}

void Space_logic::unload()
{
	m_space->unload();
}

void Space_logic::fire()
{
	m_firePressed = true;
}

bool Space_logic::nearPlanet(int index)
{
	int proxX, proxY;
	proxX = abs(m_players->at(0).getShip().getCoordinates().x - m_map->getPlanets().at(index).getX());
	proxY = abs(m_players->at(0).getShip().getCoordinates().y - m_map->getPlanets().at(index).getY());
	if (proxX<m_map->getPlanets().at(index).getWidth() && proxY<m_map->getPlanets().at(index).getHeight())
	{
		return true;
	}
	else return false;
}

int Space_logic::dock()
{
	for (int i=0; i<(*m_map).getPlanets().size(); ++i)
	{
		if (m_map->getPlanets().at(i).getCanDock() && nearPlanet(i))
		{
			m_players->at(0).getShip().setDocked(true, i);
			return 2; //switch to dock screen }
		}
	}
	return 1; //stay in space screen
}

void Space_logic::collide()
{
	for (int i=0; i<m_map->getPlanets().size(); ++i)
	{
		if (m_map->getPlanets().at(i).getCanCollide() && nearPlanet(i) && !m_players->at(0).getShip().getDocked())
		{ m_players->at(0).getShip().damage(1); }
	}
}

void Space_logic::hyperDrive()
{
	m_players->at(0).getShip().setDocked(true, 0); //make it as if the ship were docked
	//m_paused = true;
	m_hyperDrive = true;
	m_editText = ("");
	m_space->setEditText(m_editText);
	m_space->setHyperDrive(true);
}

void Space_logic::changeVelocity()
{	
	if (m_leftPressed)
	{ m_players->at(0).getShip().turn(true); }
	if (m_rightPressed)
	{ m_players->at(0).getShip().turn(false); }
	if (m_upPressed)
	{ m_players->at(0).getShip().accelerate(true); }
	if (m_downPressed)
	{ m_players->at(0).getShip().accelerate(false); }
}

void Space_logic::moveShips()
{
	for(int i=0; i<m_players->size(); ++i)
	{
		//needs to be made more precise
		//Restrict map in X axis (can't travel below 0, above maxX)
		bool x = !((m_players->at(i).getShip().getCoordinates().x<m_players->at(i).getShip().getWidth()/2
			&& m_players->at(i).getShip().getSpeedX()<0)
			|| (m_players->at(i).getShip().getCoordinates().x>m_maxX-(m_players->at(i).getShip().getWidth()/2)
			&& m_players->at(i).getShip().getSpeedX()>0));
		
		//Restrict map in Y axis (can't travel below 0, above maxY)
		bool y = !((m_players->at(0).getShip().getCoordinates().y<m_players->at(0).getShip().getHeight()/2
			&& m_players->at(0).getShip().getSpeedY()<0)
			|| (m_players->at(0).getShip().getCoordinates().y>m_maxY-(m_players->at(0).getShip().getHeight()/2)
			&& m_players->at(0).getShip().getSpeedY()>0));

		m_players->at(0).getShip().move(x, y);
	}
}

void Space_logic::moveFireballs()
{
	for (int p=0;p<m_players->size();++p)
	{
		for (int i=0; i<m_players->at(p).getShip().getMaxFireBalls(); i++)
		{
			m_players->at(p).getShip().updateFireX(i);
			m_players->at(p).getShip().updateFireY(i);
		}
	}
}

void Space_logic::makeFireballs()
{
	if (m_firePressed)
	{
		if (m_players->at(0).getShip().getFireCycle() >= 10)
		{
			int x = m_players->at(0).getShip().getFireBallNumber(); //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
			m_players->at(0).getShip().setFireBallNumber((m_players->at(0).getShip().getFireBallNumber()+1));
			m_players->at(0).getShip().setFireX(x, m_players->at(0).getShip().getCoordinates().x);
			m_players->at(0).getShip().setFireY(x, m_players->at(0).getShip().getCoordinates().y);
			m_players->at(0).getShip().setFireAngle(x, m_players->at(0).getShip().getAngle());
			if (m_players->at(0).getShip().getFireBallNumber()>=m_players->at(0).getShip().getMaxFireBalls())
			{ m_players->at(0).getShip().setFireBallNumber(0); }
			m_players->at(0).getShip().setFireCycle(0);
		}
		m_players->at(0).getShip().setFireCycle((m_players->at(0).getShip().getFireCycle()+1));
	}
}

void Space_logic::checkGameOver()
{
	if (m_players->at(0).getShip().getHealth() <= 0)
	{
		m_gameOver = true;
		m_space->setGameOver(); //tell display that game is over
		m_players->at(0).getShip().stop();
		//paused = true;
	}
}

void Space_logic::hyperEnterDigit()
{
	if(m_editText.length()<14) // limit to (6(number of digits in grid coordinates)*2(X and Y))+2(separation characters) = 14
	{
		m_editText += std::to_string(m_newKey);
		if(m_editText.length()==6) //now add the separator characters to show the user it expects the y coordinate now
		{ m_editText += ", "; }
		m_space->setEditText(m_editText); //tell display what the new string is
	}
}

void Space_logic::hyperBackDigit()
{
	if(!m_editText.empty()) //make sure there is something to get rid of
	{
		if(m_editText.length()==8) //get rid of the two separator characters first ", "
		{ m_editText.pop_back(); m_editText.pop_back(); } 

		m_editText.pop_back(); //back one digit
		m_space->setEditText(m_editText); //tell display what the new string is
	}
}

void Space_logic::hyperTextEntered()
{
	if (m_editText.length()==14 && m_textEntered==false)
	{
		m_space->setHyperDrive(false); //switch off get coordinates overlay
		m_textEntered = true;
		
		//convert relevant parts of strings to integers and tell ship where to go
		int hyperX = std::stoi(m_editText.substr(0, 6).c_str());
		int hyperY = std::stoi(m_editText.substr(8, 6).c_str());
		if (hyperX==m_players->at(0).getShip().getCoordinates().x && hyperY==m_players->at(0).getShip().getCoordinates().y) //if already there!
		{ m_hyperDrive = m_paused = false; }
		else
		{ m_players->at(0).getShip().beginHyperDrive(hyperX, hyperY); }
	}
	else if(m_textEntered==false) //abort if not full coordinates
	{ abortHyperDrive(); }
}

void Space_logic::abortHyperDrive()
{
	m_space->setHyperDrive(false);
	m_players->at(0).getShip().setDocked(false, 0);
	m_hyperDrive = m_paused = false;
}

void Space_logic::update()
{
	if (!m_paused)
	{
		//transfer user input to ship velocity
		changeVelocity();

		//Apply translation to ship
		moveShips();
	
		//Set fireball position
		moveFireballs();

		//Set fire cycle
		makeFireballs();

		//check for collision with planets
		collide();

		//Check for gameover
		checkGameOver();
	}

	if (m_hyperDrive && m_textEntered)
	{
		m_players->at(0).getShip().hyperDrive();
		if(!(m_players->at(0).getShip().getDocked())) { m_paused = m_hyperDrive = m_textEntered = false; } //Reset flags if hyperdrive is done
	}
}

int Space_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
	if (!m_hyperDrive && !m_gameOver)
	{
		switch(keyPressed.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE: 
			return -1;
		case ALLEGRO_KEY_LEFT:
			m_leftPressed = true;
			break;
		case ALLEGRO_KEY_RIGHT:
			m_rightPressed = true;
			break;
		case ALLEGRO_KEY_UP:
			m_upPressed = true;
			break;
		case ALLEGRO_KEY_DOWN:
			m_downPressed = true;
			break;
		case ALLEGRO_KEY_F:
			fire();
			break;
		case ALLEGRO_KEY_D:
			return dock();
		case ALLEGRO_KEY_P:
			if (m_paused) {m_paused = false;}
			else {m_paused = true;}
			break;
		case ALLEGRO_KEY_H:
			hyperDrive();
			break;
//		case ALLEGRO_KEY_F1:
//			saveGame();
//			break;
		}
	}
	else if (m_hyperDrive)
	{
		switch(keyPressed.keyboard.keycode)
		{
		case ALLEGRO_KEY_0: //numbers 0 to 9
		case ALLEGRO_KEY_1:
		case ALLEGRO_KEY_2:
		case ALLEGRO_KEY_3:
		case ALLEGRO_KEY_4:
		case ALLEGRO_KEY_5:
		case ALLEGRO_KEY_6:
		case ALLEGRO_KEY_7:
		case ALLEGRO_KEY_8:
		case ALLEGRO_KEY_9:
			m_newKey = keyPressed.keyboard.keycode - 27; //convert keycode to intended number
			hyperEnterDigit();
			break;
		case ALLEGRO_KEY_PAD_0: //pad numbers 0 to 9 for different people
		case ALLEGRO_KEY_PAD_1:
		case ALLEGRO_KEY_PAD_2:
		case ALLEGRO_KEY_PAD_3:
		case ALLEGRO_KEY_PAD_4:
		case ALLEGRO_KEY_PAD_5:
		case ALLEGRO_KEY_PAD_6:
		case ALLEGRO_KEY_PAD_7:
		case ALLEGRO_KEY_PAD_8:
		case ALLEGRO_KEY_PAD_9:
			m_newKey = keyPressed.keyboard.keycode - 37; //convert keycode to intended number
			hyperEnterDigit();
			break;
		case ALLEGRO_KEY_BACKSPACE: //backspace
			hyperBackDigit();
			break;
		case ALLEGRO_KEY_ENTER:
			hyperTextEntered();
			break;
		case ALLEGRO_KEY_H:
			if (m_textEntered==false)
			{ abortHyperDrive(); }
			break;
		case ALLEGRO_KEY_ESCAPE:
			return -1;
		}
	}
	else if (m_gameOver)
	{
		switch(keyPressed.keyboard.keycode)
		{
		case ALLEGRO_KEY_ENTER:
		case ALLEGRO_KEY_ESCAPE:
			return -1;
		}
	}
	return 1;
}

void Space_logic::keyRelease(ALLEGRO_EVENT &keyReleased)
{
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				m_leftPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				m_rightPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_UP) {
				m_upPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				m_downPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_F) {
				m_firePressed = false;
				(*m_players).at(0).getShip().setFireCycle(10);
			}
}

Space_logic::~Space_logic(void)
{
}
