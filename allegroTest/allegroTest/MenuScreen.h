#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "Screen.h"
#include <vector>
#include <string>

class MenuScreen: public Screen {
public:
	//Members
	std::vector<std::string> menuOptions;
	int currentOption; //index of currently selected menu option

	//Methods
	MenuScreen() {}
	void add_option(std::string option) {
		menuOptions.push_back(option);
	}
	void previous_option() {
		if (currentOption>0) currentOption--;
		else if (currentOption==0) currentOption = menuOptions.size() - 1;
	}
	void next_option() {
		if (currentOption<menuOptions.size() - 1) currentOption++;
		else if (currentOption==(menuOptions.size() -1)) currentOption = 0;
	}
	int select_option() {
		return currentOption;
	}
};

#endif