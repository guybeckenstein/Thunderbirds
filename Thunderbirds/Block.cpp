#include "Block.h"
#include "BigShip.h"
#include "SmallShip.h"

// Returns true if it is being pushed, false otherwise
bool Block::updatePushedBlock(const Board& board, const vector<Point>& shipCoord, Direction currDirection, vector<Block>& allBlocks, int shipMaxWeight, int currWeight)
{ 
	if (shipMaxWeight < currWeight || isShipClash(shipCoord, currDirection) == true) // A condition if block cannot be pushed due to ship barrier / maximum weight limit
		return false;
	bool canProgress = true;
	vector<Point> coordToUpdate = getCoord();
	for (size_t i = 0; i < coordToUpdate.size() && canProgress == true; i++)
	{
		int x = coordToUpdate[i].getX(), y = coordToUpdate[i].getY();
		if (currDirection == Direction::UP)
			if (y > 0 && board.isWall(x, y - 1) == false)
			{
				int clashedBlockIndex = -1;
				if (isBlockClash(allBlocks, currDirection, clashedBlockIndex) == true)
					canProgress = allBlocks[clashedBlockIndex].updatePushedBlock(board, getCoord(), currDirection, allBlocks, shipMaxWeight, currWeight + allBlocks[clashedBlockIndex].getWeight()); // Recursion
				if (canProgress == true)
					coordToUpdate[i].updateCoord(currDirection);
			}
			else
				canProgress = false;
		else if (currDirection == Direction::DOWN)
		{
			int clashedBlockIndex = -1; // Unused
			if (y < static_cast<int>(board.getHeight()) - 1 && board.isWall(x, y + 1) == false && isBlockClash(allBlocks, currDirection, clashedBlockIndex) == false)
				coordToUpdate[i].updateCoord(currDirection);
			else
				canProgress = false;
		}
		else if (currDirection == Direction::LEFT)
			if (x > 0 && board.isWall(x - 1, y) == false)
			{
				int clashedBlockIndex = -1;
				if (isBlockClash(allBlocks, currDirection, clashedBlockIndex) == true)
					canProgress = allBlocks[clashedBlockIndex].updatePushedBlock(board, getCoord(), currDirection, allBlocks, shipMaxWeight, currWeight + allBlocks[clashedBlockIndex].getWeight()); // Recursion
				if (canProgress == true && isShipClash(shipCoord, currDirection) == false)
					coordToUpdate[i].updateCoord(currDirection);
			}
			else
				canProgress = false;
		else if (currDirection == Direction::RIGHT)
			if (x < static_cast<int>(board.getWidth()) - 1 && board.isWall(x + 1, y) == false)
			{
				int clashedBlockIndex = -1;
				if (isBlockClash(allBlocks, currDirection, clashedBlockIndex) == true)
					canProgress = allBlocks[clashedBlockIndex].updatePushedBlock(board, getCoord(), currDirection, allBlocks, shipMaxWeight, currWeight + allBlocks[clashedBlockIndex].getWeight()); // Recursion
				if (canProgress == true)
					coordToUpdate[i].updateCoord(currDirection);
			}
			else
				canProgress = false;
	}
	// Coordinators are updated only if possible
	if (canProgress == true)
		setCoord(coordToUpdate);
	return canProgress;
}
// Returns true if the block is heavier than the maximum weight a ship can lift (and it falls on the ship), false otherwise
bool Block::updateBlockOnAir(const Board& board, const vector<Point>& bigShip, const vector<Point>& smallShip, const vector<Block>& allBlocks, int shipMaxWeight)
{
	bool isBigShipClash = false, isSmallShipClash = false, isCurrBlockClash = false;
	if (isWallUnderneath(board) == false)
	{
		int clashedBlockIndex = -1; // Unused
		isBigShipClash = isShipClash(bigShip, Direction::DOWN);
		isSmallShipClash = isShipClash(smallShip, Direction::DOWN);
		isCurrBlockClash = isBlockClash(allBlocks, Direction::DOWN, clashedBlockIndex);
		if (isBigShipClash == false && isSmallShipClash == false && isCurrBlockClash == false)
			updatePushedBlock(board, bigShip, Direction::DOWN, const_cast<vector<Block>&>(allBlocks), getWeight(), 0); // allBlocks coord won't change anyways
	}
	if ((isBigShipClash == true && getWeight() > static_cast<int>(bigShipConstants::MAX_WEIGHT)) || (isSmallShipClash == true && getWeight() > static_cast<int>(smallShipConstants::MAX_WEIGHT)))
		return true;
	return false;
}
//
bool Block::isWallUnderneath(const Board& board) const
{
	for (size_t i = 0; i < getCoord().size(); i++)
		if (board.isWall(getCoord()[i].getX(), getCoord()[i].getY() + 1) == true)
			return true;
	return false;
}
// Checks if a block hits a ship when it falls, either big or small
bool Block::isShipClash(const vector<Point>& ship, Direction currDirection) const
{
	for (size_t i = 0; i < getCoord().size(); i++)
		for (size_t j = 0; j < ship.size(); j++)
		{
			if (currDirection == Direction::UP && getCoord()[i].getX() == ship[j].getX() && getCoord()[i].getY() == ship[j].getY() + 1)
				return true;
			else if (currDirection == Direction::DOWN && getCoord()[i].getX() == ship[j].getX() && getCoord()[i].getY() == ship[j].getY() - 1)
				return true;
			else if (currDirection == Direction::LEFT && getCoord()[i].getX() == ship[j].getX() + 1 && getCoord()[i].getY() == ship[j].getY())
				return true;
			else if (currDirection == Direction::RIGHT && getCoord()[i].getX() == ship[j].getX() - 1 && getCoord()[i].getY() == ship[j].getY())
				return true;
		}
	return false;
}
// Function checks if there is a clash between two blocks, and if there is it returns true, and returns the index of the block through the output parameter.
bool Block::isBlockClash(const vector<Block>& allBlocks, Direction currDirection, int& clashedBlockIdx) const // Better use a vector of ints than just one int reference
{
	for (size_t blockIdx = 0; blockIdx < allBlocks.size(); blockIdx++) // Going over all other blocks
	{
		if (blockIdx == getIndex())
			continue;
		for (size_t i = 0; i < getCoord().size(); i++) // Going over all coordinators in the current falling block
			for (size_t j = 0; j < allBlocks[blockIdx].getCoord().size(); j++) // Going over all coordinators in the current other block
			{
				if (currDirection == Direction::UP && getCoord()[i].getX() == allBlocks[blockIdx].getCoord()[j].getX() && getCoord()[i].getY() == allBlocks[blockIdx].getCoord()[j].getY() + 1)
				{
					clashedBlockIdx = blockIdx;
					return true;
				}
				else if (currDirection == Direction::DOWN && getCoord()[i].getX() == allBlocks[blockIdx].getCoord()[j].getX() && getCoord()[i].getY() == allBlocks[blockIdx].getCoord()[j].getY() - 1)
				{
					clashedBlockIdx = blockIdx;
					return true;
				}
				else if (currDirection == Direction::LEFT && getCoord()[i].getX() == allBlocks[blockIdx].getCoord()[j].getX() + 1 && getCoord()[i].getY() == allBlocks[blockIdx].getCoord()[j].getY())
				{
					clashedBlockIdx = blockIdx;
					return true;
				}
				else if (currDirection == Direction::RIGHT && getCoord()[i].getX() == allBlocks[blockIdx].getCoord()[j].getX() - 1 && getCoord()[i].getY() == allBlocks[blockIdx].getCoord()[j].getY())
				{
					clashedBlockIdx = blockIdx;
					return true;
				}
			}
	}
	return false;
}