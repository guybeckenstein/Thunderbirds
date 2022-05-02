// The class is used to define the big ship object in the game
#pragma once

#include "Ship.h"

enum class bigShipConstants : char { FIGURE = '#', COLOR = 4, HEIGHT = 2, WIDTH = 2, MAX_WEIGHT = 6 };

class BigShip : public Ship
{
private:
	bool _hasReachedExit; // True only if it reaches 'E' sign
public:
	// Constructors
	BigShip(const vector<Point>& coord) : Ship(coord, true), _hasReachedExit(false) {}
	// Destructors
	~BigShip() override = default;
	// Getters
	bool getHasReachedExit() const { return _hasReachedExit; }
	// Setters
	void setHasReachedExit(bool hasReachedExit) { _hasReachedExit = hasReachedExit; }
	// Movement Methods
	bool Move(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, const vector<Point>& allGhosts, bool& validMove, bool hasReachedExit, bool colorAllowed) override;
	char getArrowKeys(const Board& board, const vector<Block>& allBlocks, bool colorAllowed, bool& exit) const;
	bool getNextDirection(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, const vector<Point>& allGhosts, char key, Direction& nextDirection, bool hasReachedExit);
	// Print Methods
	void Render(std::ostream& os) const override { os << static_cast<char>(bigShipConstants::FIGURE); }
};

