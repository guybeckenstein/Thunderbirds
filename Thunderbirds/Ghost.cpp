#include "Ghost.h"

// Movement Methods
void Ghost::Move(const Board& board, const vector<Block>& allBlocks)
{
	Direction tmpDirection;
	vector<bool> newPossibleDirections;
	bool isNewDirections = getPossibleDirections(board, allBlocks, newPossibleDirections);
	if (isNewDirections == true)
	{
		setPossibleDirections(newPossibleDirections);
		tmpDirection = getDirection();
		getNextDirection(tmpDirection);
		setDirection(tmpDirection);
	}
	vector<Point> pointToUpdate = getCoord();
	pointToUpdate[0].updateCoord(getDirection());
	setCoord(pointToUpdate);
}
// Clash Methods
// Checks if a ghost hits a ship when it moves, either big or small
bool Ghost::isShipClash(const vector<Point>& ship) const
{
	for (size_t j = 0; j < ship.size(); j++)
		if (getCoord()[0] == ship[j])
			return true;
	return false;
}
// Checks if the ghost is about to clash with a block
bool Ghost::isBlockClash(const Block& block, Direction currDirection) const
{
	bool endLoop = false;
	for (size_t j = 0; j < block.getCoord().size() && endLoop == false; j++)
	{
		int ghostX = getCoord()[0].getX(), ghostY = getCoord()[0].getY(), blockX = block.getCoord()[j].getX(), blockY = block.getCoord()[j].getY();
		if (currDirection == Direction::UP && ghostX == blockX && ghostY - 1 == blockY)
			return true;
		else if (currDirection == Direction::DOWN && ghostX == blockX && ghostY + 1 == blockY)
			return true;
		else if (currDirection == Direction::LEFT && ghostX - 1 == blockX && ghostY == blockY)
			return true;
		else if (currDirection == Direction::RIGHT && ghostX + 1 == blockX && ghostY == blockY)
			return true;
		else if (ghostX == blockX && ghostY == blockY)
			return true;
	}
	return false;
}
// Reset / Reinitialize Methods
void Ghost::Reset(const vector<Point>& coord) // Used when resetting the game (when time ends, or one of the ships in under a heavy block)
{
	for (size_t i = 0; i < getPossibleDirections().size(); i++)
		_possibleDirections[i] = false;
	setDirection(Direction::UNDEFINED);
	setCoord(coord);
}