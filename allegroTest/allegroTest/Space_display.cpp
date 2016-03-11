#include "Space_display.h"

Space_display::Space_display(void)
{
}

void Space_display::update(Logic &logic, std::vector<Player> &players, Map &map)
{
	//Clear display first
		al_clear_to_color(al_map_rgb(0, 0, 0));
		
		//Remember that all sprites drawn to the display must be drawn relative to the the current background coordinates (the camera), bgX and bgY - so subtract their x and y coordinates from bgX and bgY
		
		//Draw all 9 of the background sprites, tiled around, and including, the current grid
		int backX = gridX-windowWidth; //X coordinate of top left corner tile of the 9 tiles
		int backY = gridY-windowHeight; //Y coordinate of top left corner tile of the 9 tiles
		for (int x=0; x<3; x++)
			for (int y=0; y<3; y++)
				al_draw_bitmap(backgroundSprite,backX + windowWidth*x - bgX, backY + windowHeight*y - bgY,0);
		
		//Draw planets
		for (int i=0; i<(map.getTotalPlanets()); i++)
			al_draw_scaled_rotated_bitmap((map.getPlanet(i)).planetSprite, (map.getPlanet(i)).width/2, (map.getPlanet(i)).height/2, 
			(map.getPlanet(i)).x-bgX, (map.getPlanet(i)).y-bgY, (map.getPlanet(i)).planetScale, (map.getPlanet(i)).planetScale, 0, 0); //Draw planets only if their coordinates exist within current screen
		//Draw sprites for each Ship object
		for (int p=players.size()-1;p>=0;p--) {
			//Draw fireballs
			for (int i=0; i<MAXFIREBALLS; i++)
				al_draw_tinted_rotated_bitmap(players.at(p).getShip().fireSprite[i], al_map_rgb(255,255,255), 
				players.at(p).getShip().fireWidth/2, players.at(p).getShip().fireHeight/2, 
				players.at(p).getShip().fireX[i]-bgX, players.at(p).getShip().fireY[i]-bgY, players.at(p).getShip().fireAngle[i], 0);
			//Draw ships
			al_draw_rotated_bitmap(players.at(p).getShip().shipSpriteCurrent, players.at(p).getShip().width/2, players.at(p).getShip().height/2, 
				players.at(p).getShip().x-bgX, players.at(p).getShip().y-bgY, players.at(p).getShip().angle, 0); 
		}

		//Draw bordering rectangle //Not necessary with current gigantic map, but useful to debug a small bounded area
		//al_draw_rectangle(0-bgX, 0-bgY, maxX-bgX, maxY-bgY, al_map_rgb(255, 255, 255), 10);
		
		//Draw stats to screen
		std::stringstream s1, s2;
		s1 << "Health: " << players.at(0).getShip().health;
		s2 << "Coordinates: " << players.at(0).getShip().x << " , " << players.at(0).y;
		std::string str1 = s1.str();
		std::string str2 = s2.str();
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.05, windowHeight*0.1, 0, str1.c_str());
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.05, windowHeight*0.9, 0, str2.c_str());

		//Draw radar
		al_draw_scaled_rotated_bitmap(radarSprite, radarScreenWidth/2, radarScreenHeight/2, windowWidth*0.85, windowHeight*0.15, 0.2, 0.2, 0, 0);
		//Planets on radar
		for (int i=0; i<(map.getTotalPlanets()); i++) {
			float rX = windowWidth*0.85 - (players.at(0).getShip().x - (map.getPlanet(i)).x) * radarScale;
			float rY = windowHeight*0.15 - (players.at(0).getShip().y - (map.getPlanet(i)).y) * radarScale;
			if ((rX > windowWidth*0.85 - windowWidth*0.1) && (rX < windowWidth*0.85 + windowWidth*0.1) && (rY > windowHeight*0.15 - windowHeight*0.1) && (rY < windowHeight*0.15 + windowHeight*0.1))
				al_draw_rotated_bitmap(radarDotSprite, 2.5, 2.5, rX, rY, 0, 0); //Draw planets only if their coordinates exist within current screen
		}
		//Ships on radar
		for (int p=players.size()-1;p>=0;p--) {
			float rX = windowWidth*0.85 - (players.at(0).getShip().x - players.at(p).getShip().x) * radarScale;
			float rY = windowHeight*0.15 - (players.at(0).getShip().y - players.at(p).getShip().y) * radarScale;
			if ((rX > windowWidth*0.85 - windowWidth*0.1) && (rX < windowWidth*0.85 + windowWidth*0.1) && (rY > windowHeight*0.15 - windowHeight*0.1) && (rY < windowHeight*0.15 + windowHeight*0.1))
				al_draw_rotated_bitmap(radarDotSprite, 2.5, 2.5, rX, rY, 0, 0);
		}

		//Game over
		if ((logic.getGameOver())) 
		{ 
			al_draw_text(font, al_map_rgb(250, 0, 20), windowWidth*0.5, windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, "GAME OVER"); 
		}
		
		//Hyper Drive input
		if ((logic.getHyperDrive()))
		{
			al_draw_text(font, al_map_rgb(255, 255, 255), windowWidth*0.5, windowHeight*0.35, ALLEGRO_ALIGN_CENTRE, "Enter coordinates: ");
			al_draw_text(font, al_map_rgb(255, 255, 255), windowWidth*0.5, windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, editText.c_str());
		}
}

bool Space_display::getSpaceLoaded() { return m_spaceLoaded; }
bool Space_display::getSpaceFail() { return m_spaceFail; }

Space_display::~Space_display(void)
{
}
