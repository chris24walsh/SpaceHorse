#include "Header.h"
#include "Game.h"
#include <ios>

using namespace std;

void abort(std::string message)
{
	std::cout << message << std::endl;
	_getch();
    exit(1);
}

int main(int argc, char* argv[])
{
	//Allegro initialisation
    if (!al_init())
	{ abort("Failed to initialize allegro"); }
	
	const int FPS = 60;
	ALLEGRO_TIMER *timer;
	timer = al_create_timer(1.0/FPS);
	if (!timer)
	{ abort("Failed to create timer"); }

    if (!al_install_keyboard())
	{ abort("Failed to install keyboard"); }

	//Initialise image addon
	if(!al_init_image_addon()) 
	{ abort("Failed to initialise al_init_image_addon"); }
	
	//Initialise the font addon
	al_init_font_addon();
	if (!al_init_ttf_addon()) 
	{ abort("Failed to initalise al_init_ttf_addon"); }

	//Initialise primitives addon
	if (!al_init_primitives_addon()) 
	{ abort("Failed to initialise al_init_primitives_addon"); }

	//setup display
	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	int windowWidth = 1400;//disp_data.width;
	int windowHeight = 800;// disp_data.height;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	ALLEGRO_DISPLAY *display;
    display = al_create_display(windowWidth, windowHeight);
	if(!display) { abort("Failed to create display"); }
		
	//Initisatise the event queue
	ALLEGRO_EVENT_QUEUE *event_queue;
	event_queue = al_create_event_queue();
	if (!event_queue) { abort("Failed to create event queue"); }
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	//Initialise the sound addons
	if(!al_install_audio())
	{ abort("Failed to initialize audio!\n"); }
 
	if(!al_init_acodec_addon())
	{ abort("Failed to initialize audio codecs!\n"); }
 
	if(!al_reserve_samples(20))
	{ abort("Failed to reserve samples!\n"); }

	//seed random
	srand(static_cast<unsigned int>(time(0))); //this is in effect globally, so start it here
	rand(); //junk first result because it is always about the same in Visual Studio

	//Initialise the network library
	/*if (enet_initialize () != 0)
    {
        abort_game("An error occurred while initializing ENet.\n");
    }*/

	/*//Test sqlite*/
	int rc;
	char *error;

	// Open Database
	cout << "Opening MyDb.db ..." << endl;
	sqlite3 *db;
	rc = sqlite3_open("../../Saves/MyDb.db", &db);
	if (rc)
	{
		cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_close(db);
		return 1;
	}
	else
	{
		cout << "Opened MyDb.db." << endl << endl;
	}

	// Execute SQL
	cout << "Creating MyTable ..." << endl;
	const char *sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
	rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Created MyTable." << endl << endl;
	}

	// Execute SQL
	cout << "Inserting a value into MyTable ..." << endl;
	const char *sqlInsert = "INSERT INTO MyTable VALUES(NULL, 'A Value');";
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(error);
	}
	else
	{
		cout << "Inserted a value into MyTable." << endl << endl;
	}

	// Display MyTable
	cout << "Retrieving values in MyTable ..." << endl;
	const char *sqlSelect = "SELECT * FROM MyTable;";
	char **results = NULL;
	int rows, columns;
	sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
	if (rc)
	{
		cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
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
				cout.width(12);
				cout.setf(ios::left);
				cout << results[cellPosition] << " ";
			}

			// End Line
			cout << endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					cout.width(12);
					cout.setf(ios::left);
					cout << "~~~~~~~~~~~~ ";
				}
				cout << endl;
			}
		}
	}
	sqlite3_free_table(results);

	// Close Database
	cout << "Closing MyDb.db ..." << endl;
	sqlite3_close(db);
	cout << "Closed MyDb.db" << endl << endl;

	// Wait For User To Close Program
	cout << "Please press any key to exit the program ..." << endl;
	//_getch();*/

	//End test sqlite

	Game game(windowWidth, windowHeight, *timer, *event_queue); //Create a game object which will start a game loop

	
	if (event_queue) al_destroy_event_queue(event_queue);
    if(display) al_destroy_display(display);
	if (timer) al_destroy_timer(timer);
	return 0;
}