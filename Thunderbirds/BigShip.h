// The class is used to define the big ship object in the game
#pragma once

#include "MovingEntity.h"

enum class bigShipConstants : char { FIGURE = '#', COLOR = 4, HEIGHT = 2, WIDTH = 2, MAX_WEIGHT = 6 };

class BigShip : public MovingEntity
{
private:
	bool _hasReachedExit; // True only if it reaches 'E' sign
public:
	// Constructors
	BigShip(const vector<Point>& coord) : MovingEntity(coord, true), _hasReachedExit(false) {}
	// Destructors
	~BigShip() override = default;
	// Getters
	bool getHasReachedExit() const { return _hasReachedExit; }
	// Setters
	void setHasReachedExit(bool hasReachedExit) { _hasReachedExit = hasReachedExit; }
	// Movement Methods
	bool Move(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, bool& validMove, bool hasReachedExit, bool isFirstMove, bool colorAllowed) override;
	char getArrowKeys(const Board& board, bool colorAllowed, bool isFirstMove, bool& exit) const;
	bool getNextDirection(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, char key, Direction& nextDirection, bool hasReachedExit);
	bool isBlockClash(const Block& block, Direction currDirection, int& coordIndex) const;
	// Print Methods
	void Render(std::ostream& os) const override { os << static_cast<char>(bigShipConstants::FIGURE); }
};

