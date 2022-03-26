#include "MovingEntity.h"
#include "BigShip.h"

// Movement Methods
bool MovingEntity::areEntitiesClashing(const Point& coord, const vector<Point>& currEntity) const
{
	for (const auto& currItr : currEntity)
		if (coord == currItr)
			return true;
	return false;
}
void MovingEntity::removeEntityExistance()
{
	for (auto& currItr : const_cast<vector<Point>&>(getCoord()))
		currItr = { static_cast<int>(boardConstants::TABLE_MAX_HEIGHT) + 3, static_cast<int>(boardConstants::TABLE_MAX_WIDTH) + 3 };
	setMovement(false);
	setDirection(Direction::UNDEFINED);
}
// Reset / Reinitialize Methods
void MovingEntity::Reset(const vector<Point>& coord) // Used when resetting the game (when time ends, or one of the ships in under a heavy block)
{
	(coord.size() == static_cast<int>(bigShipConstants::HEIGHT) * static_cast<int>(bigShipConstants::WIDTH)) ? setMovement(true) : setMovement(false);
	setDirection(Direction::UNDEFINED);
	_coord = coord;
}