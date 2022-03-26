// The class inherits the common coordinator feature to "BigShip", "SmallShip" and "Block" classes
#pragma once

#include <map>
#include "Board.h"
#include "Point.h"

using std::map;

class Entity
{
protected:
	// Position
	vector<Point> _coord;
public:
	// Constructor
	Entity(const vector<Point>& coord) { _coord = coord; }
	// Destructor
	virtual ~Entity() = default;
	// Getters
	const vector<Point>& getCoord() const { return _coord; }
	// Setters
	void setCoord(const vector<Point>& coord) { _coord = coord; }
	// Operators
	friend std::ostream& operator<<(std::ostream& os, const Entity& object);
	// Print Methods
	void eraseRender(bool colorAllowed, const vector<Point>& tmpCoord) const;
	void Print(bool colorAllowed, int figureColor) const;
	virtual void Render(std::ostream& os) const = 0;
	// Reset / Reinitialize Methods
	virtual void Reset(const vector<Point>& coord) = 0;
};