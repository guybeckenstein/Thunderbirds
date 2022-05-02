// The class inherits the common single coordinator with the random moving ability to "HorizontalGhost", "VerticalGhost" and "WanderingGhost"
#pragma once

#include "Block.h"
#include "Entity.h"

enum class ghostConstants : char { COLOR = 8 };

class Ghost : public Entity
{
protected:
	Direction _direction; // Current direction
	vector<bool> _possibleDirections; // Next move directions
public:
	// Constructor
	Ghost(const vector<Point>& coord, const vector<bool>& possibleDirections) : Entity(coord), _direction(Direction::UNDEFINED), _possibleDirections(possibleDirections) { srand(static_cast<unsigned int>(std::time(NULL))); }
	// Destructor
	virtual ~Ghost() = default;
	// Getters
	Direction getDirection() const { return _direction; }
	const vector<bool>& getPossibleDirections() const { return _possibleDirections; }
	// Setters
	void setDirection(Direction direction) { _direction = direction; }
	void setPossibleDirections(const vector<bool>& possibleDirections) { _possibleDirections = possibleDirections; }
	// Movement Methods
	void Move(const Board& board, const vector<Block>& allBlocks);
	virtual bool getPossibleDirections(const Board& board, const vector<Block>& allBlocks, vector<bool>& newPossibleDirections) const = 0;
	virtual void getNextDirection(Direction& newDirection) const = 0;
	// Clash Methods
	bool isBlockClash(const Block& block, Direction currDirection) const;
	bool isShipClash(const vector<Point>& ship) const;
	// Reset / Reinitialize Methods
	void Reset(const vector<Point>& coord) override;
};

