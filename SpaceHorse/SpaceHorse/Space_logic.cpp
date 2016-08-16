#include "Space_logic.h"


Space_logic::Space_logic()
	:m_space_display(),
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
	fireball(nullptr)
{
}

void Space_logic::load(Space_display &space, int windowWidth, int windowHeight, std::vector<Player> &players, Map &map)
{
	space.load(players, map);
	m_space_display = &space;
	m_players = &players;
	m_map = &map;
	m_maxX = windowWidth*space.getNumberGrids();
	m_maxY = windowHeight*space.getNumberGrids();

	//set size of planets, ships and fire according to loaded bitmaps
	for(int i=0; i<map.getPlanets().size(); ++i) 
	{ 
		map.getPlanets().at(i).setPlanetSize(m_space_display->getPlanetSpriteWidth(i), m_space_display->getPlanetSpriteHeight(i)); 
	}

	for(int i=0;i<players.size();++i)
	{
		players.at(i).getShip().setShipSize(m_space_display->getShipSpriteWidth(i), m_space_display->getShipSpriteHeight(i));
		players.at(i).getShip().setFireSize(m_space_display->getFireSpriteWidth(i), m_space_display->getFireSpriteHeight(i));
	}
}

void Space_logic::unload()
{
	m_space_display->unload();
}

void Space_logic::fire()
{
	m_firePressed = true;
}

bool Space_logic::nearPlanet(int index)
{
	int proxX, proxY;
	proxX = abs(m_players->at(0).getShip().getX() - m_map->getPlanets().at(index).getX());
	proxY = abs(m_players->at(0).getShip().getY() - m_map->getPlanets().at(index).getY());
	if (proxX<m_map->getPlanets().at(index).getWidth() && proxY<m_map->getPlanets().at(index).getHeight())
	{
		return true;
	}
	else return false;
}

void Space_logic::collectSpaceJunk()
{
	for (int i = 0; i < m_map->getSpaceJunk().size(); i++)
	{
		if (nearSpaceJunk(i)) {
			m_map->collectSpaceJunk(i);
			m_players->at(0).getShip().incrementSpaceJunkCounter();
		}
	}
}

bool Space_logic::nearSpaceJunk(int index)
{
	int proxX, proxY;
	proxX = abs(m_players->at(0).getShip().getX() - m_map->getSpaceJunk().at(index)->getX());
	proxY = abs(m_players->at(0).getShip().getY() - m_map->getSpaceJunk().at(index)->getY());
	int sj_width = m_map->getSpaceJunk().at(index)->getWidth();
	int sj_height = m_map->getSpaceJunk().at(index)->getHeight();
	if (proxX < (sj_width / 2.0) && proxY < (sj_height / 2.0))
		return true;
	else 
		return false;
}

GameMode Space_logic::dock()
{
	for (int i=0; i<(*m_map).getPlanets().size(); ++i)
	{
		if (m_map->getPlanets().at(i).getCanDock() && nearPlanet(i))
		{
			m_players->at(0).getShip().setDocked(true, i);
			return GameMode::dock;
		}
	}
	return GameMode::space;
}

void Space_logic::collide()
{
	for (int i=0; i<m_map->getPlanets().size(); ++i)
	{
		if (m_map->getPlanets().at(i).getCanCollide() && nearPlanet(i) && !m_players->at(0).getShip().getDocked())
		{ 
			m_players->at(0).getShip().setHealth((m_players->at(0).getShip().getHealth()-1)); 
		}
	}
}

void Space_logic::hyperDrive()
{
	m_players->at(0).getShip().setDocked(true, 0); //make it as if the ship were docked
	//m_paused = true;
	m_hyperDrive = true;
	m_editText = ("");
	m_space_display->setEditText(m_editText);
	m_space_display->setHyperDrive(true);
	m_oldAngle = m_players->at(0).getShip().getAngle();
	m_distanceTravelX = abs(abs(m_x2) - abs(m_players->at(0).getShip().getX()));
	m_distanceTravelY = abs(abs(m_y2) - abs(m_players->at(0).getShip().getY()));
}

void Space_logic::rotate()
{
	if (m_leftPressed)
	{ m_players->at(0).getShip().setAngle(m_players->at(0).getShip().getAngle()-0.05); }
	if (m_rightPressed)
	{ m_players->at(0).getShip().setAngle(m_players->at(0).getShip().getAngle()+0.05); }

	if (m_players->at(0).getShip().getAngle() > 6.25) { m_players->at(0).getShip().setAngle(0); } //Keep angle under 6.28
	if (m_players->at(0).getShip().getAngle() < 0) { m_players->at(0).getShip().setAngle(6.25); } //Keep angle above 0
}

void Space_logic::animateShips()
{
	for (int i=0;i<m_players->size();++i)
	{
		if (m_players->at(i).getShip().getSpeed()>0)
		{
			if(m_players->at(i).getShip().getFlipflop()<5)
			{ m_space_display->setShipSpriteCurrents(i, 1); }
			else { m_space_display->setShipSpriteCurrents(i, 2); }

			m_players->at(i).getShip().setFlipflop(m_players->at(i).getShip().getFlipflop()+1); //increase by one
			if (m_players->at(i).getShip().getFlipflop()==10) { m_players->at(i).getShip().setFlipflop(0); } //reset when it reaches 10
		}
		else { m_space_display->setShipSpriteCurrents(i, 0); }
	}
}

void Space_logic::changeSpeed()
{
	if (m_upPressed && (!m_players->at(0).getShip().goingMaxSpeed()))
	{ (*m_players).at(0).getShip().setSpeed((*m_players).at(0).getShip().getSpeed()+1); }

	if (m_downPressed && (m_players->at(0).getShip().getSpeed()>0))
	{ (*m_players).at(0).getShip().setSpeed((*m_players).at(0).getShip().getSpeed()-1); }

	//Resolve translations for x and y axis
	(*m_players).at(0).getShip().setSpeedX((*m_players).at(0).getShip().getSpeed() * cos((*m_players).at(0).getShip().getAngle()));
	(*m_players).at(0).getShip().setSpeedY((*m_players).at(0).getShip().getSpeed() * sin((*m_players).at(0).getShip().getAngle()));
}

void Space_logic::moveShips()
{
	for(int i=0; i<m_players->size(); ++i)
	{
		//Restrict map in X axis (can't travel below 0, above maxX)
		if ((m_players->at(i).getShip().getX()<m_players->at(i).getShip().getWidth()/2
			&& m_players->at(i).getShip().getSpeedX()<0)
			|| (m_players->at(i).getShip().getX()>m_maxX-(m_players->at(i).getShip().getWidth()/2)
			&& m_players->at(i).getShip().getSpeedX()>0))
		{ ; } //Can't travel
		else
		{ m_players->at(0).getShip().setX(m_players->at(0).getShip().getX()+m_players->at(0).getShip().getSpeedX()); } //maybe get ship to add its own speed.
	
		//Restrict map in Y axis (can't travel below 0, above maxY)
		if ((m_players->at(0).getShip().getY()<m_players->at(0).getShip().getHeight()/2
			&& m_players->at(0).getShip().getSpeedY()<0)
			|| (m_players->at(0).getShip().getY()>m_maxY-(m_players->at(0).getShip().getHeight()/2)
			&& m_players->at(0).getShip().getSpeedY()>0))
		{ ; } //Can't travel
		else
		{ m_players->at(0).getShip().setY(m_players->at(0).getShip().getY()+m_players->at(0).getShip().getSpeedY()); }
	}
}

void Space_logic::setShipsPositions()
{
	//Set which grid the ship is currently in
	m_space_display->setGridX();
	m_space_display->setGridY();

	//Set background coordinates
	m_space_display->setBgX();
	m_space_display->setBgY();
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
			//load fireball sound
			fireball = al_load_sample("../../sounds/fireball.wav");
			if (!fireball) { printf("Could not load 'fireball.wav'.\n"); }
			al_play_sample(fireball, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			int x = m_players->at(0).getShip().getFireBallNumber(); //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
			m_players->at(0).getShip().setFireBallNumber((m_players->at(0).getShip().getFireBallNumber()+1));
			m_players->at(0).getShip().setFireX(x, m_players->at(0).getShip().getX());
			m_players->at(0).getShip().setFireY(x, m_players->at(0).getShip().getY());
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
		m_space_display->setGameOver(); //tell display that game is over
		m_players->at(0).getShip().setSpeed(0); //stop ship
		//paused = true;
	}
}

void Space_logic::hyperAlignAngle()
{
	//Decide if better to rotate clockwise or anticlockwise
	if (m_newAngle > m_players->at(0).getShip().getAngle())
	{
		if (m_newAngle - m_players->at(0).getShip().getAngle() < 3.14)
		{ m_players->at(0).getShip().setAngle(m_players->at(0).getShip().getAngle()+0.05); }
		else
		{ m_players->at(0).getShip().setAngle(m_players->at(0).getShip().getAngle()-0.05); }
	}
	else
	{
		if (m_players->at(0).getShip().getAngle() - m_newAngle > 3.14)
		{ m_players->at(0).getShip().setAngle(m_players->at(0).getShip().getAngle()+0.05); }
		else
		{ m_players->at(0).getShip().setAngle(m_players->at(0).getShip().getAngle()-0.05); }
	}

	rotate(); //keep angle between 0 and 6.28

	//Check for nearness then align angle exactly
	if (abs(abs(m_players->at(0).getShip().getAngle()) - abs(m_newAngle)) < 0.05)
	{
		m_players->at(0).getShip().setAngle(m_newAngle);
		m_angleAligned = true;
	}
}

void Space_logic::hyperMove()
{
	m_players->at(0).getShip().setX(m_players->at(0).getShip().getX()
		+ m_players->at(0).getShip().getMaxSpeed() * m_hyperSpeed * cos(m_players->at(0).getShip().getAngle()));
	m_players->at(0).getShip().setY(m_players->at(0).getShip().getY()
		+ m_players->at(0).getShip().getMaxSpeed() * m_hyperSpeed * sin(m_players->at(0).getShip().getAngle()));
	m_distanceTravelX -= abs(m_players->at(0).getShip().getMaxSpeed() * m_hyperSpeed * cos(m_players->at(0).getShip().getAngle()));
	m_distanceTravelY -= abs(m_players->at(0).getShip().getMaxSpeed() * m_hyperSpeed * sin(m_players->at(0).getShip().getAngle()));

	//if ( abs(abs(players.at(0).x) - abs(x2)) < 1000 && abs(abs(players.at(0).y) - abs(y2)) < 1000) { //Check for nearness, then align distance exactly
	if (m_distanceTravelX <= 1000 && m_distanceTravelY <= 1000)
	{
		m_players->at(0).getShip().setX(m_x2);
		m_players->at(0).getShip().setY(m_y2);
		m_players->at(0).getShip().setDocked(false, 0);
		m_paused = m_hyperDrive = m_angleAligned = m_textEntered = false; //Reset flags
	}
	setShipsPositions();
}

void Space_logic::hyperEnterDigit()
{
	if(m_editText.length()<14) // limit to (6(number of digits in grid coordinates)*2(X and Y))+2(separation characters) = 14
	{
		m_editText += std::to_string(m_newKey);
		if(m_editText.length()==6) //now add the separator characters to show the user it expects the y coordinate now
		{ m_editText += ", "; }
		m_space_display->setEditText(m_editText); //tell display what the new string is
	}
}

void Space_logic::hyperBackDigit()
{
	if(!m_editText.empty()) //make sure there is something to get rid of
	{
		if(m_editText.length()==8) //get rid of the two separator characters first ", "
		{ m_editText.pop_back(); m_editText.pop_back(); } 

		m_editText.pop_back(); //back one digit
		m_space_display->setEditText(m_editText); //tell display what the new string is
	}
}

void Space_logic::hyperTextEntered()
{
	if (m_editText.length()==14 && m_textEntered==false)
	{
		m_space_display->setHyperDrive(false); //switch off get coordinates overlay
		m_textEntered = true;
		
		m_x2 = std::stoi(m_editText.substr(0, 6).c_str()); //better I think to convert relevant parts of string to integers
		m_y2 = std::stoi(m_editText.substr(8, 6).c_str());
		
		m_newAngle = atan2( (m_y2 - m_players->at(0).getShip().getY()),  (m_x2 - m_players->at(0).getShip().getX()) );
		if (m_newAngle<0) { m_newAngle += 3.14*2; }
		
		m_distanceTravelX = abs(abs(m_x2) - abs(m_players->at(0).getShip().getX()));
		m_distanceTravelY = abs(abs(m_y2) - abs(m_players->at(0).getShip().getY()));

		if (m_x2==m_players->at(0).getShip().getX() && m_y2==m_players->at(0).getShip().getY()) //if already there!
		{ m_hyperDrive = m_paused = false; }
	}
	else if(m_textEntered==false) //abort if not full coordinates
	{ abortHyperDrive(); }
}

void Space_logic::abortHyperDrive()
{
	m_space_display->setHyperDrive(false);
	m_players->at(0).getShip().setDocked(false, 0);
	m_hyperDrive = m_paused = false;
}

void Space_logic::update()
{
	if (!m_paused)
	{
		//Rotate clockwise or anti-clockwise
		rotate();

		//Cycle between different rocket sprites, to give effect of rocket blasting -- but should this only happen when accelerating???
		animateShips();

		//Increase or decrease players.at(0).speed
		changeSpeed();

		//Apply translation to ship
		moveShips();

		//tell display to update grid and coordinates
		setShipsPositions();

		//Check if we have collected Space Junk
		collectSpaceJunk();

		//Set fireball position
		moveFireballs();

		//Set fire cycle
		makeFireballs();

		//check for collision with planets
		collide();

		//Check for gameover
		checkGameOver();

		//Check for animations
		checkAnimations();
	}

	if (m_hyperDrive && m_textEntered)
	{
		if (!m_angleAligned) { hyperAlignAngle(); } //Get angle right first
		if (m_angleAligned) { hyperMove(); } //Then check distance
	}
}

GameMode Space_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
	if (!m_hyperDrive && !m_gameOver)
	{
		switch(keyPressed.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE: 
			return GameMode::menu;
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
		case ALLEGRO_KEY_A:
			triggerAnimation1();
			break;
		case ALLEGRO_KEY_Q:
			triggerAnimation2();
			break;
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
			return GameMode::quit;
		}
	}
	else if (m_gameOver)
	{
		switch(keyPressed.keyboard.keycode)
		{
		case ALLEGRO_KEY_ENTER:
		case ALLEGRO_KEY_ESCAPE:
			return GameMode::quit;
		}
	}
	return GameMode::space;
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

void Space_logic::triggerAnimation1()
{
	Ship& ship = m_players->at(0).getShip(); //Get a reference to player 1 ship object
	Animation a1(ship.getX(), ship.getY(), ship.getAngle());
	a1.setAction(10, 10, 0);
	ship.setAnimation(a1);
	std::cout << "# Animations: " << ship.getAnimations().size() << std::endl;
}

void Space_logic::triggerAnimation2()
{
	Ship& ship = m_players->at(0).getShip(); //Get a reference to player 1 ship object
	Animation a1(ship.getX(), ship.getY(), ship.getAngle());
	ship.setAnimation(a1);
	std::cout << "# Animations: " << ship.getAnimations().size() << std::endl;
}

void Space_logic::checkAnimations()
{
	
	bool done = false; //A check of whether the animation is done
	Ship& ship = m_players->at(0).getShip(); //A reference to player 1 ship object

	//If ship has animations then..
	if (m_players->at(0).getShip().getAnimations().size() > 0) {
		//Cycle through all ships animations
		for (int i=0; i<ship.getAnimations().size(); i++) {
			Animation &animation = ship.getAnimations().at(i); //A reference to current animation being processed
			//Trigger the animations per-frame action
			done = animation.actionPerFrame();
			//Adjust the ships coordinates accordingly
			ship.setX(animation.getX());
			ship.setY(animation.getY());
			ship.setAngle(animation.getAngle());
			//If animation complete, delete from animations array
			if (done) ship.getAnimations().erase(ship.getAnimations().begin() + i);
		}
	}
}

Space_logic::~Space_logic(void)
{
}
