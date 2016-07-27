#include "New_game_logic.h"
#include <ios>


New_game_logic::New_game_logic()
	:m_new_game(),
	m_textEntered(), //the text string that has been entered so far
	m_newChar()
{
}

void New_game_logic::load(New_game_display &New_game)
{
	New_game.load();
	m_new_game = &New_game;
}

void New_game_logic::unload()
{
	(*m_new_game).unload();
}

void New_game_logic::update() {}

GameMode New_game_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
	switch (keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_A: //digits a to z
	case ALLEGRO_KEY_B:
	case ALLEGRO_KEY_C:
	case ALLEGRO_KEY_D:
	case ALLEGRO_KEY_E:
	case ALLEGRO_KEY_F:
	case ALLEGRO_KEY_G:
	case ALLEGRO_KEY_H:
	case ALLEGRO_KEY_I:
	case ALLEGRO_KEY_J:
	case ALLEGRO_KEY_K:
	case ALLEGRO_KEY_L:
	case ALLEGRO_KEY_M:
	case ALLEGRO_KEY_N:
	case ALLEGRO_KEY_O:
	case ALLEGRO_KEY_P:
	case ALLEGRO_KEY_Q:
	case ALLEGRO_KEY_R:
	case ALLEGRO_KEY_S:
	case ALLEGRO_KEY_T:
	case ALLEGRO_KEY_U:
	case ALLEGRO_KEY_V:
	case ALLEGRO_KEY_W:
	case ALLEGRO_KEY_X:
	case ALLEGRO_KEY_Y:
	case ALLEGRO_KEY_Z:
		m_newChar = keyPressed.keyboard.keycode + 64; //convert keycode to intended ascii value (e.g. A = ascii 65)
		enterText(m_newChar);
		break;
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
		m_newChar = keyPressed.keyboard.keycode + 21; //convert keycode to intended ascii value (e.g. 0 = ascii 48)
		enterText(m_newChar);
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
		m_newChar = keyPressed.keyboard.keycode + 11; //convert keycode to intended ascii value (e.g. 0 = ascii 48)
		enterText(m_newChar);
		break;
	case ALLEGRO_KEY_BACKSPACE: //backspace
		deleteText();
		break;
	case ALLEGRO_KEY_ENTER:
	{
		if (m_textEntered.empty())
		{
			return GameMode::newGame; //stay in New_game, as there is no text entered yet
		}
		else
		{
			//Create a database with that name, and put that name as user.
			int rc;
			char *error;

			// Open Database
			std::cout << "Opening MyDb.db ..." << std::endl;
			sqlite3 *db;
			std::string saveName = "../../Saves/" + m_textEntered + ".db";
			rc = sqlite3_open(saveName.c_str(), &db);
			if (rc)
			{
				std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl << std::endl;
				sqlite3_close(db);
				fail("");
			}
			else
			{
				std::cout << "Opened MyDb.db." << std::endl << std::endl;
			}

			// Execute SQL
			std::cout << "Creating MyTable ..." << std::endl;
			const char *sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
			rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
			if (rc)
			{
				std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
				sqlite3_free(error);
			}
			else
			{
				std::cout << "Created MyTable." << std::endl << std::endl;
			}

			// Execute SQL
			std::cout << "Inserting a value into MyTable ..." << std::endl;
			const char *sqlInsert = "INSERT INTO MyTable VALUES(NULL, 'A Value');";
			rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
			if (rc)
			{
				std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
				sqlite3_free(error);
			}
			else
			{
				std::cout << "Inserted a value into MyTable." << std::endl << std::endl;
			}


			// Display MyTable
			std::cout << "Retrieving values in MyTable ..." << std::endl;
			const char *sqlSelect = "SELECT * FROM MyTable;";
			char **results = NULL;
			int rows, columns;
			sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
			if (rc)
			{
				std::cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << std::endl << std::endl;
				sqlite3_free(error);
			}
			else
			{
				// Display Table
				for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
				{
					for (int colCtr = 0; colCtr < columns; ++colCtr)
					{
						// Determine Cell Position
						int cellPosition = (rowCtr * columns) + colCtr;

						// Display Cell Value
						std::cout.width(12);
						std::cout.setf(std::ios::left);
						std::cout << results[cellPosition] << " ";
					}

					// End Line
					std::cout << std::endl;

					// Display Separator For Header
					if (0 == rowCtr)
					{
						for (int colCtr = 0; colCtr < columns; ++colCtr)
						{
							std::cout.width(12);
							std::cout.setf(std::ios::left);
							std::cout << "~~~~~~~~~~~~ ";
						}
						std::cout << std::endl;
					}
				}
			}
			sqlite3_free_table(results);

			// Close Database
			std::cout << "Closing MyDb.db ..." << std::endl;
			sqlite3_close(db);
			std::cout << "Closed MyDb.db" << std::endl << std::endl;

			return GameMode::space; //Start new game (go to space screen)
		}
	}
	}
	(*m_new_game).setTextEntered(m_textEntered); //update textEntered string in new_game_display object
	return GameMode::newGame; //stay in New_game
}

void New_game_logic::enterText(char newChar) {
	if (m_textEntered.length()<10) // limit to 10 characters
	{
		m_textEntered += newChar;
		m_new_game->setTextEntered(m_textEntered); //tell display what the new string is
	}
}

void New_game_logic::deleteText() {
	if (!m_textEntered.empty()) //make sure there is something to get rid of
	{
		m_textEntered.pop_back(); //back one digit
		m_new_game->setTextEntered(m_textEntered); //tell display what the new string is
	}
}

void New_game_logic::fail(std::string message) {
	std::cout << message << std::endl;
	_getch();
	exit(1);
}

New_game_logic::~New_game_logic(void)
{
}
