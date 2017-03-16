#pragma once

const int MAX_WIDTH_LABYRINTH_PROTECTION = 102;
const int MAX_HEIGHT_LABYRINTH_PROTECTION = 102;
const int MAX_WIDTH_LABYRINTH = 100;
const int MAX_HEIGHT_LABYRINTH = 100;

struct Cell
{
	Cell(bool isWall = false)
		: wall(isWall)
	{}
	sf::RectangleShape rect;
	bool wall = false;
	bool close = false;
	int F;
	int G;
	int H;
	Cell * parent = nullptr;
};

struct Points
{
	std::shared_ptr<Cell> startPoint = nullptr;
	std::shared_ptr<Cell> finishPoint = nullptr;
};

struct Size
{
	size_t width = 0;
	size_t height = 0;
};

typedef std::array<std::array<std::shared_ptr<Cell>, MAX_HEIGHT_LABYRINTH_PROTECTION>, MAX_WIDTH_LABYRINTH_PROTECTION> Labyrinth;

void InitLabyrinth(Labyrinth & labyrinth);

bool ReadLabyrinth(std::istream & input, Labyrinth & labyrinth, Size & size, Points & points, sf::RenderWindow & window);

bool FindWay(Labyrinth & labyrinth, const Size & size, Points & points, sf::RenderWindow & window);