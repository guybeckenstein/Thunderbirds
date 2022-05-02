#include "Ship.h"
#include "BigShip.h"

// Movement Methods
bool Ship::areEntitiesClashing(const Point& coord, const vector<Point>& currEntity) const
{
	for (const auto& currItr : currEntity)
		if (coord == currItr)
			return true;
	return false;
}
// Checks if the ship is about to clash with a block
bool Ship::isBlockClash(const Block& block, Direction currDirection) const
{
	bool endLoop = false;
	for (size_t i = 0; i < getCoord().size() && endLoop == false; i++)
	{
		for (size_t j = 0; j < block.getCoord().size() && endLoop == false; j++)
		{
			int shipX = getCoord()[i].getX(), shipY = getCoord()[i].getY(), blockX = block.getCoord()[j].getX(), blockY = block.getCoord()[j].getY();
			if (currDirection == Direction::UP)
			{
				if (i > 1)
					endLoop = true;
				if (shipX == blockX && shipY - 1 == blockY)
					return true;
			}
			else if (currDirection == Direction::DOWN)
			{
				if (i < 2)
					endLoop = true;
				if (shipX == blockX && shipY + 1 == blockY)
					return true;
			}
			else if (currDirection == Direction::LEFT)
			{
				if (i == 1 || i == 3)
					endLoop = true;
				if (shipX - 1 == blockX && shipY == blockY)
					return true;
			}
			else if (currDirection == Direction::RIGHT)
			{
				if (i == 0 || i == 2)
					endLoop = true;
				if (shipX + 1 == blockX && shipY == blockY)
					return true;
			}
		}
		if ((currDirection == Direction::LEFT && i == 1) || (currDirection == Direction::DOWN && i < 2) || (currDirection == Direction::RIGHT && (i == 0 || i == 2)))
			endLoop = false;
	}
	return false;
}
void Ship::removeEntityExistance()
{
	for (auto& currItr : const_cast<vector<Point>&>(getCoord()))
		currItr = { static_cast<int>(boardConstants::TABLE_MAX_HEIGHT) + 3, static_cast<int>(boardConstants::TABLE_MAX_WIDTH) + 3 };
	setActive(false);
	setDirection(Direction::UNDEFINED);
}
// Reset / Reinitialize Methods
void Ship::Reset(const vector<Point>& coord) // Used when resetting the game (when time ends, or one of the ships in under a heavy block)
{
	(coord.size() == static_cast<int>(bigShipConstants::HEIGHT) * static_cast<int>(bigShipConstants::WIDTH)) ? setActive(true) : setActive(false);
	setFirstMove(true);
	setDirection(Direction::UNDEFINED);
	setCoord(coord);
}