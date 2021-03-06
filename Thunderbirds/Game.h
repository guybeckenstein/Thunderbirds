// This class is the actual infrastructure of Thunderbirds game, using all the other classes
#pragma once

#include <fstream>
#include "BigShip.h"
#include "Block.h"
#include "Board.h"
#include "HorizontalGhost.h"
#include "Menu.h"
#include "SmallShip.h"
#include "VerticalGhost.h"
#include "WanderingGhost.h"

enum class gameConstants : char { GAME_SPEED = 100, ESC = 27, EXIT = '9', INIT_LIVES = 3 };
const int MAX_OBJECTS = static_cast<int>(boardConstants::TABLE_MAX_HEIGHT) * static_cast<int>(boardConstants::TABLE_MAX_WIDTH);

class Game
{
private:
	Board _board;
	vector<Point> _bigShipCoord, _smallShipCoord; // All starting coordinators of both of the ships
	vector<vector<Point>> _blocksCoord; // All starting coordinators of all of the blocks
	vector<Point> _horizontalGhostsCoord, _verticalGhostsCoord, _wanderingGhostsCoord; // All starting coordinators of all of the ghosts (each ghost is 1x1 sized, so no need to use vector of vectors)
	int _time;
	// vector<vector<Point>> _horizontalGhostsCoord, _verticalGhostsCoord, _wanderingGhostsCoord; // All starting coordinators of all of the ghosts
public:
	// Constructor and Game Construction
	Game(const string& screenName, bool colorAllowed, bool& isValid);
	bool isValidScreen(const string& screenName, vector<string>& currBoard, vector<Point>& exitCoord, Point& legendCoord);
	int setScreenWidth(const string& firstLine, int width, bool& res);
	bool isLegendOnFirstLine(const string& string, int& width) const;
	int readTextFileLine(vector<Point>& allBlockCoord, vector<Point>& allExitCoord, vector<string>& currBoard, int y, const string& currLine, int screenWidth, int& countBigShip, int& countSmallShip, int& countLegend, int& legendHeight, int& countExitCharacters, Point& legendCoord, bool& illegalCharacter);
	bool isValidAmountOfCharacters(int countBigShipCharacters, int countSmallShipCharacters, int countLegendCharacters) const { return countBigShipCharacters < 5 && countSmallShipCharacters < 3 && countLegendCharacters < 2; }
	bool createVectorsFromPoints(const vector<Point>& allBlockCoord);
	bool createBigShipVector();
	bool createSmallShipVector();
	void createBlocksVector(const vector<Point>& tmpBlocksCoord);
	void isInvalidScreen(const string& screenName, bool& res, bool illegalCharacter, int height, int width, int countBigShip, int countSmallShip, int countLegend, int countExitCharacters) const;
	void seperateEntitiesFromBoard(const vector<Point>& allBlockCoord, vector<string>& currBoard) const;
	// Getters
	const Board& getBoard() const { return _board; }
	const vector<Point>& getBigShipCoord() const { return _bigShipCoord; }
	const vector<Point>& getSmallShipCoord() const { return _smallShipCoord; }
	const vector<vector<Point>>& getBlocksCoord() const { return _blocksCoord; }
	const vector<Point>& getHorizontalGhostsCoord() const { return _horizontalGhostsCoord; }
	const vector<Point>& getVerticalGhostsCoord() const { return _verticalGhostsCoord; }
	const vector<Point>& getWanderingGhostsCoord() const { return _wanderingGhostsCoord; }
	int getTime() const { return _time; }
	// Setters
	void setTime(int time) { _time = time; }
	// Operators
	friend std::ostream& operator<<(std::ostream& out, Direction direction);
	// Game Methods
	bool Play(const string& screenName, BigShip& bigShip, SmallShip& smallShip, vector<Block>& blocks, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, int& lives, int& time, bool colorAllowed);
	vector<Point> createGhostsCoordVector(const vector<HorizontalGhost>& horizontalGhosts, const vector<VerticalGhost>& verticalGhosts, const vector<WanderingGhost>& wanderingGhosts) const;
	bool moveShipsAndBlocks(BigShip& bigShip, SmallShip& smallShip, vector<Block>& blocks, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, const vector<Point>& allGhosts, bool& exit, bool colorAllowed);
	bool moveGhosts(const vector<Point>& bigShipCoord, const vector<Point>& smallShipCoord, const vector<Block>& allBlocksCoord, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, bool colorAllowed);
	bool hasReachedExit(const Point& coord, const vector<Point> exit) const;
	void Outcome(int lives, bool isQuit, int legendX, int legendY, bool colorAllowed) const;
	// General Game Methods
	void Print(int lives, int time, bool isBigShipActive, bool colorAllowed) const;
	void Reset(BigShip& bigShip, SmallShip& smallShip, vector<Block>& blocks, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, int& time, int& lives);
};