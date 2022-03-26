#include "Entity.h"
#include "Game.h"
#include "Menu.h"

// Operators
std::ostream& operator<<(std::ostream& os, const Entity& object)
{
	object.Render(os);
	return os;
}
// Print Methods
void Entity::eraseRender(bool colorAllowed, const vector<Point>& tmpCoord) const // Prints the original figure of the current coordinator
{
	for (size_t i = 0; i < tmpCoord.size(); i++)
	{
		Menu::gotoxy(tmpCoord[i].getX(), tmpCoord[i].getY());
		std::cout << static_cast<char>(boardConstants::BLANK) << std::endl;
	}
}
void Entity::Print(bool colorAllowed, int figureColor) const // Prints the entity's render - in it's new coordinator
{
	if (colorAllowed == true)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), figureColor);
	vector<Point> currCoord = getCoord();
	for (size_t i = 0; i < currCoord.size(); i++)
	{
		Menu::gotoxy(currCoord[i].getX(), currCoord[i].getY());
		std::cout << *this << std::endl;
	}
}