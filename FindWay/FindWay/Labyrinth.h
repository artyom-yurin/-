#pragma once

const int MAX_WIDTH_LABYRINTH_PROTECTION = 102;
const int MAX_HEIGHT_LABYRINTH_PROTECTION = 102;
const int MAX_WIDTH_LABYRINTH = 100;
const int MAX_HEIGHT_LABYRINTH = 100;

struct Cell
{
	Cell(bool isWall = false)
		: close(isWall)
	{}
	sf::RectangleShape rect;
	size_t x;
	size_t y;
	bool watch = false;
	bool close;
	size_t F = 0;
	size_t G = 0;
	std::shared_ptr<Cell> parent = nullptr;
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

struct Heroes
{
	sf::RectangleShape fox;
	sf::Texture texture;
	sf::CircleShape kolobok;
};

typedef std::array<std::array<std::shared_ptr<Cell>, MAX_HEIGHT_LABYRINTH_PROTECTION>, MAX_WIDTH_LABYRINTH_PROTECTION> Labyrinth;

void InitLabyrinth(Labyrinth & labyrinth);

bool ReadLabyrinth(std::istream & input, Labyrinth & labyrinth, Size & size, Points & points, sf::RenderWindow & window, Heroes & heroes);

bool FindWay(Labyrinth & labyrinth, const Size & size, Points & points, sf::RenderWindow & window, Heroes & heroes);

void DrawLabyrinth(Labyrinth & labyrinth, const Size & size, sf::RenderWindow & window, Heroes & heroes);
