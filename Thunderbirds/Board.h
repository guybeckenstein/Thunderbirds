// The class is used to print the table of the screen, all entities including ships and blocks - when initializing the game
// Also being used to avoid game-objects from touching game walls
#pragma once

#include "Point.h"

enum class boardConstants : char { TABLE_MAX_HEIGHT = 25, TABLE_MAX_WIDTH = 80, LEGEND = '&', LEGEND_MAX_HEIGHT = 3, LEGEND_MAX_WIDTH = 25, EMPTY_SPACE = '^', 
	BLANK = ' ', WALL = 'W', BLOCK = '*', EXIT = 'E', EXIT_COLOR = 8,
	INITIAL_HORIZONTAL_GHOST_POSITION = '$', INITIAL_VERTICAL_GHOST_POSITION = '!', INITIAL_WANDERING_GHOST_POSITION = '%' };

class Board
{
private:
	vector<string> _table;
	vector<Point> _exit;
	size_t _height, _width;
	Point _legend;
public:
	// Constructors and Board Construction
	Board() : _height(0), _width(0), _legend(static_cast<int>(Direction::UNDEFINED), static_cast<int>(Direction::UNDEFINED)) {}
	void initializeBoard(const vector<string>& table, const vector<Point>& exitCoord, const Point& legendCoord);
	// Getters
	const vector<Point>& getExit() const { return _exit; }
	size_t getHeight() const { return _height; }
	size_t getWidth() const { return _width; }
	const Point& getLegend() const { return _legend; }
	// Setters
	void setTable(const vector<string>& table) { _table = table; }
	void setHeight(size_t height) { _height = height; }
	void setWidth(size_t width) { _width = width; }
	void setLegend(const Point& legend) { _legend = legend; }
	// Methods
	void Print(bool colorAllowed, const string& screenName) const;
	bool isWall(int x, int y) const { return (_table[y][x] == static_cast<char>(boardConstants::WALL)); }
	bool isBlock(int x, int y) const { return (_table[y][x] == static_cast<char>(boardConstants::BLOCK)); }
	bool isEmptySpace(int x, int y) const { return (_table[y][x] == static_cast<char>(boardConstants::EMPTY_SPACE)); }
};