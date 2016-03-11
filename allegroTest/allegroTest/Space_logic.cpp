#include "Space_logic.h"


Space_logic::Space_logic(void)
{
}

void Space_logic::load(Space_display &space)
{
	space.load();
	m_space = &space;
}

void Space_logic::unload()
{
	(*m_space).unload();
}

void fire()
{
	firePressed = true;
}

int Space_logic::dock()
{
	if (players.at(0).canDock) //Else, if able to dock, do so, stop ship and set it to safe mode
	{
		players.at(0).speed = 0;
		safe = true;
		return 2;
	}
	else return 1;
}

void triggerCollision() { players.at(0).health--; }

void hyperdrive()
{
	players.at(0).speed = 0;
	//paused = true;
	hyperDrive = true;
	enterCoordinates.str("");
	editText = "";
	oldAngle = players.at(0).angle;
	distanceTravelX = abs(abs(x2) - abs(players.at(0).x));
	distanceTravelY = abs(abs(y2) - abs(players.at(0).y));
}

void Space_logic::update()
{
	if (!paused) {
		//Rotate clockwise or anti-clockwise
		if (leftPressed) {
			players.at(0).angle-=0.05;
		}
		if (rightPressed) {
			players.at(0).angle+=0.05;
		}
		if (players.at(0).angle > 6.25) players.at(0).angle = 0;
		if (players.at(0).angle < 0) players.at(0).angle = 6.25;

		//Cycle between different rocket sprites, to give effect of rocket blasting
		for (int p=0;p<players.size();p++) {
			if (players.at(p).speed>0) {
				if (players.at(p).flipflop<5) players.at(p).shipSpriteCurrent = players.at(p).shipSprite1;
				else if (players.at(p).flipflop>=5) players.at(p).shipSpriteCurrent = players.at(p).shipSprite2;
				players.at(p).flipflop++;
				if (players.at(p).flipflop==10) players.at(p).flipflop=0;
			}
			else players.at(p).shipSpriteCurrent = players.at(p).shipSprite;
		}

		//Increase or decrease players.at(0).speed
		if (upPressed) {
			if (players.at(0).speed<players.at(0).maxSpeed) players.at(0).speed += 1;
		}
		else if (downPressed) {
			if (players.at(0).speed>0) players.at(0).speed -= 1;
		}

		//Resolve translations for x and y axis
		players.at(0).speedX = players.at(0).speed * cos(players.at(0).angle);
		players.at(0).speedY = players.at(0).speed * sin(players.at(0).angle);

		//Apply translation to ship
		//Restrict map in X axis (can't travel below 0, above maxX)
		if ((players.at(0).x<players.at(0).width/2 && players.at(0).speedX<0) || (players.at(0).x>maxX-(players.at(0).width/2) && players.at(0).speedX>0)) ; //Can't travel
		else players.at(0).x += players.at(0).speedX;
	
		//Restrict map in Y axis (can't travel below 0, above maxY)
		if ((players.at(0).y<players.at(0).height/2 && players.at(0).speedY<0) || (players.at(0).y>maxY-(players.at(0).height/2) && players.at(0).speedY>0)) ; //Can't travel
		else players.at(0).y += players.at(0).speedY;

		//Set which grid the ship is currently in
		gridX = int(float(players.at(0).x)/windowWidth)*windowWidth;
		gridY = int(float(players.at(0).y)/windowHeight)*windowHeight;

		//Set background coordinates
		bgX = players.at(0).x - windowWidth/2;
		bgY = players.at(0).y - windowHeight/2;
	
		//Set fireball position
		for (int p=0;p<players.size();p++) {
			for (int i=0; i<MAXFIREBALLS; i++) players.at(p).fireX[i] += players.at(p).fireSpeed * cos(players.at(p).fireAngle[i]);
			for (int i=0; i<MAXFIREBALLS; i++) players.at(p).fireY[i] += players.at(p).fireSpeed * sin(players.at(p).fireAngle[i]);
		}

		//Set fire cycle
		if (firePressed) {
			if (players.at(0).fireCycle >= 10) {
				int x = players.at(0).fireballNumber++; //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
				players.at(0).fireX[x] = players.at(0).x;
				players.at(0).fireY[x] = players.at(0).y;
				players.at(0).fireAngle[x] = players.at(0).angle;
				if (players.at(0).fireballNumber>=MAXFIREBALLS){
					players.at(0).fireballNumber = 0;
				}
				players.at(0).fireCycle = 0;
			}
			players.at(0).fireCycle++;
		}
	
		//Check if near docking station, and if so allow docking
		int proxX, proxY;
		for (int i=0; i<planets.size(); i++) {
			proxX = abs(players.at(0).x - planets.at(i).x);
			proxY = abs(players.at(0).y - planets.at(i).y);
			if (planets.at(i).canDock)
				if (proxX<planets.at(i).width/2 && proxY<planets.at(i).height/2) {
					players.at(0).canDock = true;
					break;
				}
			else players.at(0).canDock = false;
		}
		//Check for gameover
		if (players.at(0).health <= 0) {
			gameOver = true;
			players.at(0).speed = 0;
			//paused = true;
		}
	}
	if (hyperDrive && textEntered) {
		if (!angleAligned) { //Get angle right first
			//Decide if better to rotate clockwise or anticlockwise
			if (newAngle > players.at(0).angle) {
				if (newAngle - players.at(0).angle < 3.14) players.at(0).angle += 0.05;
				else players.at(0).angle -= 0.05;
			}
			else {
				if (players.at(0).angle - newAngle > 3.14) players.at(0).angle += 0.05;
				else players.at(0).angle -= 0.05;
			}
			if (players.at(0).angle > 6.25) players.at(0).angle = 0; //Keep angle under 6.28
			if (players.at(0).angle < 0) players.at(0).angle = 6.25; //Keep angle above 0
			if (abs(abs(players.at(0).angle) - abs(newAngle)) < 0.05) { //Check for nearness then align angle exactly
				players.at(0).angle = newAngle;
				angleAligned = true;
			}
		}
		if (angleAligned) { //Then check distance
			players.at(0).x += players.at(0).maxSpeed * hyperSpeed * cos(players.at(0).angle);
			players.at(0).y += players.at(0).maxSpeed * hyperSpeed * sin(players.at(0).angle);
			distanceTravelX -= players.at(0).maxSpeed * hyperSpeed * cos(players.at(0).angle);
			distanceTravelY -= players.at(0).maxSpeed * hyperSpeed * sin(players.at(0).angle);
			//if ( abs(abs(players.at(0).x) - abs(x2)) < 1000 && abs(abs(players.at(0).y) - abs(y2)) < 1000) { //Check for nearness, then align distance exactly
			if (distanceTravelX <= 1000 || distanceTravelY <= 1000) {
				players.at(0).x = x2;
				players.at(0).y = y2;
				paused = hyperDrive = angleAligned = textEntered = false; //Reset flags
			}
		}
		//Set which grid the ship is currently in
		gridX = int(float(players.at(0).x)/windowWidth)*windowWidth;
		gridY = int(float(players.at(0).y)/windowHeight)*windowHeight;

		//Set background coordinates
		bgX = players.at(0).x - windowWidth/2;
		bgY = players.at(0).y - windowHeight/2;
	}
}

int Space_logic::keyPress(ALLEGRO_EVENT keyPressed)
{
	if (!hyperDrive && !gameOver) {
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
			return -1;
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_LEFT) {
			leftPressed = true;
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
			rightPressed = true;
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_UP) {
			upPressed = true;
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_DOWN) {
			downPressed = true;
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_F) {
			fire();
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_D) {
			return dock();
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_P) {
			if (paused && !gameOver) paused = false;
			else paused = true;
		}
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_H) {
			hyperdrive();
		}
//		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_F1) {
//			saveGame();
//		}
	}
	if (hyperDrive) {
		if (keyPressed.keyboard.keycode >= 27 && keyPressed.keyboard.keycode <= 36) {
			newKey = keyPressed.keyboard.keycode;
			newKey -= 27;
			enterCoordinates << newKey;
			editText = enterCoordinates.str();
		}
		if (keyPressed.keyboard.keycode == 75) {
			enterCoordinates << " ";
			editText = enterCoordinates.str();
		}
		if (keyPressed.keyboard.keycode == 63) {
			if (editText.length() > 0)
				editText.pop_back();
		}
		else if (keyPressed.keyboard.keycode == ALLEGRO_KEY_ENTER) {
			if (!editText.empty()) {
				textEntered = true;
				x2 = atoi(strtok((char*)editText.c_str(), " "));
				y2 = atoi(strtok(NULL, " "));
				newAngle = atan2( (y2 - players.at(0).y),  (x2 - players.at(0).x) );
				if (newAngle<0) newAngle += 3.14*2;
				distanceTravelX = abs(abs(x2) - abs(players.at(0).x));
				distanceTravelY = abs(abs(y2) - abs(players.at(0).y));
				if (x2==players.at(0).x && y2==players.at(0).y) hyperDrive = paused = false;
			}
			else hyperDrive = paused = false;
		}
	}
	if (gameOver) {
		if (keyPressed.keyboard.keycode == ALLEGRO_KEY_ENTER || keyPressed.keyboard.keycode == ALLEGRO_KEY_ESCAPE) return -1;
	}
	return 1;
}

void Space_logic::keyRelease(ALLEGRO_EVENT keyReleased)
{
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				leftPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				rightPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_UP) {
				upPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				downPressed = false;
			}
			if (keyReleased.keyboard.keycode == ALLEGRO_KEY_F) {
				firePressed = false;
				(*m_players).at(0).getShip().setFireCycle(10);
			}
}

Space_logic::~Space_logic(void)
{
}
