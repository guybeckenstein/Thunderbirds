// The class inherits the common coordinator with the moving ability feature to "BigShip", "SmallShip"
#pragma once

#include "Block.h"
#include "Entity.h"

class Ship : public Entity
{
protected:
	bool _active, _firstMove;
	Direction _direction;
public:
	// Constructor
	Ship(const vector<Point>& coord, bool movement) : Entity(coord), _active(movement), _firstMove(true), _direction(Direction::UNDEFINED) {}
	// Destructor
	virtual ~Ship() = default;
	// Getters
	bool getActive() const { return _active; }
	bool getFirstMove() const { return _firstMove; }
	Direction getDirection() const { return _direction; }
	// Setters
	void setActive(bool active) { _active = active; }
	void setFirstMove(bool firstMove) { _firstMove = firstMove; }
	void setDirection(Direction direction) { _direction = direction; }
	// Movement Methods
	virtual bool Move(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, const vector<Point>& allGhosts, bool& validMove, bool hasReachedExit, bool colorAllowed) = 0; // Function must return bool to deal with cases of a player that quits game
	bool areEntitiesClashing(const Point& coord, const vector<Point>& currEntity) const;
	bool isBlockClash(const Block& block, Direction currDirection) const;
	void removeEntityExistance();
	// Reset / Reinitialize Methods
	void Reset(const vector<Point>& coord) override;
};

