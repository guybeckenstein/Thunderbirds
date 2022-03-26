// The class inherits the common coordinator with the moving ability feature to "BigShip", "SmallShip"
#pragma once

#include "Block.h"

enum class entityConstants : int { NUM_OF_ARROWKEYS = 4 };

class MovingEntity : public Entity
{
protected:
	// Movement
	bool _movement;
	// Direction and next directions
	Direction _direction;
public:
	// Constructor
	MovingEntity(const vector<Point>& coord, bool movement) : Entity(coord), _movement(movement), _direction(Direction::UNDEFINED) {}
	// Destructor
	virtual ~MovingEntity() = default;
	// Getters
	bool getMovement() const { return _movement; }
	Direction getDirection() const { return _direction; }
	// Setters
	void setMovement(bool movement) { _movement = movement; }
	void setDirection(Direction direction) { _direction = direction; }
	// Movement Methods
	virtual bool Move(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, bool& validMove, bool hasReachedExit, bool isFirstMove, bool colorAllowed) = 0; // Function must return bool to deal with cases of a player that quits game
	bool areEntitiesClashing(const Point& coord, const vector<Point>& currEntity) const;
	void removeEntityExistance();
	// Reset / Reinitialize Methods
	void Reset(const vector<Point>& coord) override;
};

