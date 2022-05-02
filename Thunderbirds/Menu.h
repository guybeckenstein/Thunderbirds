// This class is for printing main menu and other outputs
#pragma once

#include <iostream>
#include "Point.h"

enum class menuConstants : int { NON_COLOR = 7, COLOR = 15 };

using std::string, std::vector;

class Menu
{
private:
	vector<string> _screenNames;
	bool _colorAllowed;
public:
	Menu(const vector<string>& screenNames) : _screenNames(screenNames), _colorAllowed(false) {}
	// Used Functions
	static bool validScreenName(const string& name);
	void Run();
	char mainMenu();
	void newGameMenu() const;
	int getScreenIndexByName() const;
	void loadScreens(int singleScreenIndex) const;
	void Results(bool isWin, bool isQuit) const;
	void Instructions();
	static void pauseGame(const Point& legend, bool colorAllowed, bool& exit);
	static void clearLegendArea(const Point& legend);
	// Getters
	const vector<string>& getScreenNames() const { return _screenNames; }
	bool getColorSettings() const { return _colorAllowed; } // Bonus
	// Setters
	void setColorSettings(bool value) { _colorAllowed = value; } // Bonus
	// Operators
	const Menu& operator=(const Menu& menu);
	// We are expected to use these Methods
	static void clearScreen() { system("cls"); }
	static void pauseScreen() { system("pause"); }
	static void gotoxy(int x, int y);
	// Bonus Methods
	bool colorScreen(bool isColorAllowed);
};