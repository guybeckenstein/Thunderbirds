#include "Point.h"

// Operators
const Point& Point::operator=(const Point& coord)
{
	if (this != &coord)
	{
		setX(coord.getX());
		setY(coord.getY());
	}
	return *this;
}
const Point& Point::operator+(const Point& coord)
{
	setX(getX() + coord.getX());
	setY(getY() + coord.getY());
	return *this;
}
// Methods
void Point::updateCoord(Direction direction) // Updates the coordinator of the object or part of it, changing it by one.
{
	if (direction == Direction::UP)
		setY(_y - 1);
	else if (direction == Direction::LEFT)
		setX(_x - 1);
	else if (direction == Direction::DOWN)
		setY(_y + 1);
	else if (direction == Direction::RIGHT)
		setX(_x + 1);
}