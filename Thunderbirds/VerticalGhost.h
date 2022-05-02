// The class is used to define the horizontal ghost object in the game
#pragma once

#include "Ghost.h"

enum class verticalGhostConstants : char { NUM_OF_DIRECTIONS = 2, FIGURE = '!' };

class VerticalGhost : public Ghost
{
private:
public:
	// Constructor
	VerticalGhost(const vector<Point>& coord, const vector<bool>& possibleDirections) : Ghost(coord, possibleDirections) {}
	// Destructor
	virtual ~VerticalGhost() = default;
	// Movement Methods
	bool getPossibleDirections(const Board& board, const vector<Block>& allBlocks, vector<bool>& newPossibleDirections) const override;
	void checkForWalls(const Board& board, vector<bool>& newPossibleDirections) const;
	void getNextDirection(Direction& newDirection) const override;
	// Print Methods
	void Render(std::ostream & os) const override { os << static_cast<char>(verticalGhostConstants::FIGURE); }
};

