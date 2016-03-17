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
	m_angleAligned(0),
	m_distanceAligned(0),
	m_textEntered(0),
	m_newKey(0),
	m_distance(0),
	m_distanceTravelX(0),
	m_distanceTravelY(0),
	m_maxX(0),
	m_maxY(0),
	m_editText(""),
	m_oldAngle(0),
	m_newAngle(0),
	m_x2(3000),
	m_y2(3000),
	m_hyperSpeed(50),
	m_enterCoordinates("")
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
	{ map.getPlanets().at(i).setPlanetSize((*m_space).getPlanetSpriteWidth(i), (*m_space).getPlanetSpriteHeight(i)); }
	for(int i=0;i<players.size();++i)
	{
		players.at(i).getShip().setShipSize((*m_space).getShipSpriteWidth(i), (*m_space).getShipSpriteHeight(i));
		players.at(i).getShip().setFireSize((*m_space).getFireSpriteWidth(i), (*m_space).getFireSpriteHeight(i));
	}
}

void Space_logic::unload()
{
	(*m_space).unload();
}

void Space_logic::fire()
{
	m_firePressed = true;
}

int Space_logic::dock()
{
	if ((*m_players).at(0).getShip().getCanDock()) //Else, if able to dock, do so, stop ship and set it to safe mode
	{
		(*m_players).at(0).getShip().toggleDocked();
		return 2; //switch to dock screen
	}
	else return 1; //stay in space screen
}

void Space_logic::triggerCollision() { (*m_players).at(0).getShip().setHealth(((*m_players).at(0).getShip().getHealth()-1)); }

void Space_logic::hyperDrive() //still a little buggy need to fix the calculations, etc.
{
	(*m_players).at(0).getShip().toggleDocked(); //make it as if the ship were docked
	//m_paused = true;
	m_hyperDrive = true;
	m_enterCoordinates.str("");
	m_enterCoordinates.clear();
	m_editText = ("");
	(*m_space).setEditText(m_editText);
	(*m_space).toggleHyperDrive();
	m_oldAngle = (*m_players).at(0).getShip().getAngle();
	m_distanceTravelX = abs(abs(m_x2) - abs((*m_players).at(0).getShip().getX()));
	m_distanceTravelY = abs(abs(m_y2) - abs((*m_players).at(0).getShip().getY()));
}

void Space_logic::update()
{
	if (!m_paused)
	{
		//Rotate clockwise or anti-clockwise
		if (m_leftPressed)
		{ (*m_players).at(0).getShip().setAngle((*m_players).at(0).getShip().getAngle()-0.05); }
		if (m_rightPressed)
		{ (*m_players).at(0).getShip().setAngle((*m_players).at(0).getShip().getAngle()+0.05); }
		
		if ((*m_players).at(0).getShip().getAngle() > 6.25) { (*m_players).at(0).getShip().setAngle(0); }
		if ((*m_players).at(0).getShip().getAngle() < 0) { (*m_players).at(0).getShip().setAngle(6.25); }

		//Cycle between different rocket sprites, to give effect of rocket blasting -- but should this only happen when accelerating???
		for (int i=0;i<(*m_players).size();++i)
		{
			if ((*m_players).at(i).getShip().getSpeed()>0)
			{
				if((*m_players).at(i).getShip().getFlipflop()<5) { (*m_space).setShipSpriteCurrents(i, 1); }
				else { (*m_space).setShipSpriteCurrents(i, 2); }
				(*m_players).at(i).getShip().setFlipflop((*m_players).at(i).getShip().getFlipflop()+1); //increase by one
				if ((*m_players).at(i).getShip().getFlipflop()==10) { (*m_players).at(i).getShip().setFlipflop(0); } //reset when it reaches 10
			}
			else { (*m_space).setShipSpriteCurrents(i, 0); }
		}

		//Increase or decrease players.at(0).speed
		if (m_upPressed)
		{
			if (!(*m_players).at(0).getShip().goingMaxSpeed())
			{ (*m_players).at(0).getShip().setSpeed((*m_players).at(0).getShip().getSpeed()+1); }
		}
		if (m_downPressed)
		{
			if ((*m_players).at(0).getShip().getSpeed()>0) 
			{ (*m_players).at(0).getShip().setSpeed((*m_players).at(0).getShip().getSpeed()-1); }
		}

		//Resolve translations for x and y axis
		(*m_players).at(0).getShip().setSpeedX((*m_players).at(0).getShip().getSpeed() * cos((*m_players).at(0).getShip().getAngle()));
		(*m_players).at(0).getShip().setSpeedY((*m_players).at(0).getShip().getSpeed() * sin((*m_players).at(0).getShip().getAngle()));

		//Apply translation to ship
		//Restrict map in X axis (can't travel below 0, above maxX)
		if ((*m_players).at(0).getShip().getX()<(*m_players).at(0).getShip().getWidth()/2 &&
			((*m_players).at(0).getShip().getSpeedX()<0) ||
			((*m_players).at(0).getShip().getX()>m_maxX-((*m_players).at(0).getShip().getWidth()/2) &&
			(*m_players).at(0).getShip().getSpeedX()>0)) ; //Can't travel
		else (*m_players).at(0).getShip().setX((*m_players).at(0).getShip().getX()+(*m_players).at(0).getShip().getSpeedX());
	
		//Restrict map in Y axis (can't travel below 0, above maxY)
		if ((*m_players).at(0).getShip().getY()<(*m_players).at(0).getShip().getHeight()/2 &&
			((*m_players).at(0).getShip().getSpeedY()<0) ||
			((*m_players).at(0).getShip().getY()>m_maxY-((*m_players).at(0).getShip().getHeight()/2) &&
			(*m_players).at(0).getShip().getSpeedY()>0)) ; //Can't travel
		else (*m_players).at(0).getShip().setY((*m_players).at(0).getShip().getY()+(*m_players).at(0).getShip().getSpeedY());

		//Set which grid the ship is currently in
		(*m_space).setGridX();
		(*m_space).setGridY();

		//Set background coordinates
		(*m_space).setBgX();
		(*m_space).setBgY();
	
		//Set fireball position
		for (int p=0;p<(*m_players).size();++p)
		{
			for (int i=0; i<(*m_players).at(p).getShip().getMaxFireBalls(); i++)
			{ (*m_players).at(p).getShip().updateFireX(i); }
			for (int i=0; i<(*m_players).at(p).getShip().getMaxFireBalls(); i++)
			{ (*m_players).at(p).getShip().updateFireY(i); }
		}

		//Set fire cycle
		if (m_firePressed) {
			if ((*m_players).at(0).getShip().getFireCycle() >= 10)
			{
				int x = (*m_players).at(0).getShip().getFireBallNumber(); //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
				(*m_players).at(0).getShip().setFireBallNumber(((*m_players).at(0).getShip().getFireBallNumber()+1));
				(*m_players).at(0).getShip().setFireX(x, (*m_players).at(0).getShip().getX());
				(*m_players).at(0).getShip().setFireY(x, (*m_players).at(0).getShip().getY());
				(*m_players).at(0).getShip().setFireAngle(x, (*m_players).at(0).getShip().getAngle());
				if ((*m_players).at(0).getShip().getFireBallNumber()>=(*m_players).at(0).getShip().getMaxFireBalls())
				{ (*m_players).at(0).getShip().setFireBallNumber(0); }
				(*m_players).at(0).getShip().setFireCycle(0);
			}
			(*m_players).at(0).getShip().setFireCycle(((*m_players).at(0).getShip().getFireCycle()+1));
		}
	
		//Check if near docking station, and if so allow docking
		int proxX, proxY;
		for (int i=0; i<(*m_map).getPlanets().size(); ++i)
		{
			proxX = abs((*m_players).at(0).getShip().getX() - (*m_map).getPlanets().at(i).getX());
			proxY = abs((*m_players).at(0).getShip().getY() - (*m_map).getPlanets().at(i).getY());
			if (proxX<(*m_map).getPlanets().at(i).getWidth() && proxY<(*m_map).getPlanets().at(i).getHeight())
			{
				if((*m_map).getPlanets().at(i).getCanDock())
				{ (*m_players).at(0).getShip().setCanDock(true, i); break; }
				else if((*m_map).getPlanets().at(i).getCanCollide())
				{ triggerCollision(); }
			}
			else { (*m_players).at(0).getShip().setCanDock(false, 0); }
		}

		//Check for gameover
		if ((*m_players).at(0).getShip().getHealth() <= 0)
		{
			m_gameOver = true;
			(*m_space).setGameOver();
			(*m_players).at(0).getShip().setSpeed(0);
			//paused = true;
		}
	}

	if (m_hyperDrive && m_textEntered)
	{
		if (!m_angleAligned) //Get angle right first
		{
			//Decide if better to rotate clockwise or anticlockwise
			if (m_newAngle > (*m_players).at(0).getShip().getAngle())
			{
				if (m_newAngle - (*m_players).at(0).getShip().getAngle() < 3.1416) 
				{ (*m_players).at(0).getShip().setAngle((*m_players).at(0).getShip().getAngle()+0.05); }
				else (*m_players).at(0).getShip().setAngle((*m_players).at(0).getShip().getAngle()-0.05);
			}
			else
			{
				if ((*m_players).at(0).getShip().getAngle() - m_newAngle > 3.1416) 
				{(*m_players).at(0).getShip().setAngle((*m_players).at(0).getShip().getAngle()+0.05);}
				else (*m_players).at(0).getShip().setAngle((*m_players).at(0).getShip().getAngle()-0.05);
			}
			if ((*m_players).at(0).getShip().getAngle() > 6.25) { (*m_players).at(0).getShip().setAngle(0); } //Keep angle under 6.28
			if ((*m_players).at(0).getShip().getAngle() < 0) { (*m_players).at(0).getShip().setAngle(6.25); } //Keep angle above 0
			if (abs(abs((*m_players).at(0).getShip().getAngle()) - abs(m_newAngle)) < 0.05) //Check for nearness then align angle exactly
			{
				(*m_players).at(0).getShip().setAngle(m_newAngle);
				m_angleAligned = true;
			}
		}
		if (m_angleAligned) //Then check distance
		{
			(*m_players).at(0).getShip().setX((*m_players).at(0).getShip().getX() +
				(*m_players).at(0).getShip().getMaxSpeed() * m_hyperSpeed * cos((*m_players).at(0).getShip().getAngle()));
			(*m_players).at(0).getShip().setY((*m_players).at(0).getShip().getY() +
				(*m_players).at(0).getShip().getMaxSpeed() * m_hyperSpeed * sin((*m_players).at(0).getShip().getAngle()));

			m_distanceTravelX -= abs((*m_players).at(0).getShip().getMaxSpeed() * m_hyperSpeed * cos((*m_players).at(0).getShip().getAngle()));
			m_distanceTravelY -= abs((*m_players).at(0).getShip().getMaxSpeed() * m_hyperSpeed * sin((*m_players).at(0).getShip().getAngle()));
			//if ( abs(abs(players.at(0).x) - abs(x2)) < 1000 && abs(abs(players.at(0).y) - abs(y2)) < 1000) { //Check for nearness, then align distance exactly
			if (m_distanceTravelX <= 1000 && m_distanceTravelY <= 1000)
			{
				(*m_players).at(0).getShip().setX(m_x2);
				(*m_players).at(0).getShip().setY(m_y2);
				(*m_players).at(0).getShip().toggleDocked();
				m_paused = m_hyperDrive = m_angleAligned = m_textEntered = false; //Reset flags
			}
		}
		//Set which grid the ship is currently in
		(*m_space).setGridX();
		(*m_space).setGridY();

		//Set background coordinates
		(*m_space).setBgX();
		(*m_space).setBgY();
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
		if (keyPressed.keyboard.keycode >= 27 && keyPressed.keyboard.keycode <= 36) //numbers 0 to 9
		{
			if(m_editText.length()<14) // 6 = number of digits in grid coordinate + 2 for ", " separation characters
			{
				m_newKey = keyPressed.keyboard.keycode;
				m_newKey -= 27;
				m_enterCoordinates << m_newKey;
				m_editText = m_enterCoordinates.str();
				if(m_editText.length()==6)
				{
					m_enterCoordinates << ", ";
					m_editText = m_enterCoordinates.str();
				}
				(*m_space).setEditText(m_editText);
			}
		}
		else if (keyPressed.keyboard.keycode == 63) //backspace
		{
			if(m_editText.length()>0)
			{
				if(m_editText.length()==8)
				{ m_editText.pop_back(); m_editText.pop_back(); } //get rid of the two separator characters first ", "
				m_editText.pop_back();
				m_enterCoordinates.str("");
				m_enterCoordinates.clear();
				m_enterCoordinates << m_editText;
			}
			(*m_space).setEditText(m_editText);
		}
		else if (keyPressed.keyboard.keycode == ALLEGRO_KEY_ENTER)
		{
			if (m_editText.length()==14 && m_textEntered==false)
			{
				(*m_space).toggleHyperDrive();
				m_textEntered = true;
//				m_x2 = atoi(strtok((char*)m_editText.c_str(), ", "));
//				m_y2 = atoi(strtok(NULL, ", "));
				m_x2 = atoi(m_editText.substr(0, 6).c_str()); //better I think.
				m_y2 = atoi(m_editText.substr(8, 6).c_str());
				m_newAngle = atan2( (m_y2 - (*m_players).at(0).getShip().getY()),  (m_x2 - (*m_players).at(0).getShip().getX()) );

				if (m_newAngle<0) { m_newAngle += 3.1416*2; }
				m_distanceTravelX = abs(abs(m_x2) - abs((*m_players).at(0).getShip().getX()));
				m_distanceTravelY = abs(abs(m_y2) - abs((*m_players).at(0).getShip().getY()));

				if (m_x2==(*m_players).at(0).getShip().getX() && m_y2==(*m_players).at(0).getShip().getY())
				{ m_hyperDrive = m_paused = false; }
			}
			else if(m_textEntered==false)
			{
				(*m_space).toggleHyperDrive();
				(*m_players).at(0).getShip().toggleDocked();
				m_hyperDrive = m_paused = false;
			}
		}
		else if (keyPressed.keyboard.keycode == ALLEGRO_KEY_H)
		{
			if (m_textEntered==false)
			{
				(*m_space).toggleHyperDrive();
				(*m_players).at(0).getShip().toggleDocked();
				m_hyperDrive = m_paused = false;
			}
		}
		else if (keyPressed.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { return -1; }
	}
	else if (m_gameOver)
	{
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_ENTER || keyPressed.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		{ return -1; }
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
