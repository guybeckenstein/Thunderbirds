// The class is used to define the block object in the game
#pragma once

#include "Board.h"
#include "Entity.h"

enum class blockConstants : char { COLOR = 6 };

class Block : public Entity
{
private:
	int _index, _weight;
public:
	// Constructors
	Block(const vector<Point>& coord, int index) : Entity(coord), _index(index), _weight(static_cast<int>(coord.size())) {}
	// Destructors
	~Block() override = default;
	// Getters
	int getIndex() const { return _index; }
	int getWeight() const { return _weight; }
	// Setters
	void setWeight(int weight) { _weight = weight; }
	// Movement Methods
	bool updatePushedBlock(const Board& board, const vector<Point>& shipCoord, Direction currDirection, vector<Block>& allBlocks, const vector<Point>& ghostsChoord, int shipMaxWeight, int currWeight);
	bool updateBlockOnAir(const Board& board, const vector<Point>& bigShip, const vector<Point>& smallShip, const vector<Block>& allBlocks, const vector<Point>& ghostsCoord, int shipMaxWeight);
	bool isWallUnderneath(const Board& board) const;
	bool isShipClash(const vector<Point>& ship, Direction currDirection) const;
	bool isBlockClash(const vector<Block>& allBlocks, Direction currDirection, int& clashedBlockIdx) const;
	bool isGhostClash(const vector<Point>& allGhostsCoord, Direction currDirection) const;
	// Print Methods
	void Render(std::ostream & os) const override { os << static_cast<char>(boardConstants::BLOCK); }
	// Reset / Reinitialize Methods
	void Reset(const vector<Point>& coord) override { _coord = coord; }
};