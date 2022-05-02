// The class is used to give every entity or part of it a starting coordinator, and to update its coordinator during gameplay
#pragma once

#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

enum class Direction : int { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3, SMALL_SHIP_OR_STAY = 4, BIG_SHIP_OR_STAY = 5, UNDEFINED = -1 };

using std::pair, std::string, std::vector;

class Point
{
private:
	int _x, _y;
public:
	// Constructors
	Point() = default;
	Point(int x, int y) : _x(x), _y(y) {}
	Point(const Point& point) { *this = point; }
	Point(Point&& point) noexcept : _x(point.getX()), _y(point.getY()) {}
	// Destructor
	~Point() = default;
	// Getters
	int getX() const { return _x; }
	int getY() const { return _y; }
	// Setters
	void setX(const int x) { _x = x; }
	void setY(const int y) { _y = y; }
	// Operators
	const Point& operator=(const Point& coord);
	const Point& operator+(const Point& coord);
	bool operator==(const Point& coord) const { return (getX() == coord.getX() && getY() == coord.getY()); }
	bool operator!=(const Point& coord) const { return (getX() != coord.getX() || getY() != coord.getY()); };
	// Methods
	void updateCoord(Direction direction);
};