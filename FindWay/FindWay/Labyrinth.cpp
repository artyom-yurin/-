#include "stdafx.h"
#include "Labyrinth.h"

void DrawLabyrinth(Labyrinth & labyrinth, const Size & size, sf::RenderWindow & window)
{
	window.clear();
	for (size_t i = 1; i <= size.width; i++)
	{
		for (size_t j = 1; j <= size.height; j++)
		{
			window.draw(labyrinth[i][j]->rect);
		}
	}
	window.display();
}

void InitLabyrinth(Labyrinth & labyrinth)
{
	for (size_t i = 1; i <= MAX_HEIGHT_LABYRINTH; ++i)
	{
		for (size_t j = 1; j <= MAX_WIDTH_LABYRINTH; ++j)
		{
			labyrinth[i][j] = std::make_shared<Cell>();
		}
	}
	for (size_t j = 0; j < MAX_WIDTH_LABYRINTH_PROTECTION; ++j)
	{
		labyrinth[0][j] = std::make_shared<Cell>(true);
		labyrinth[101][j] = std::make_shared<Cell>(true);
	}
	for (size_t i = 0; i < MAX_HEIGHT_LABYRINTH_PROTECTION; ++i)
	{
		labyrinth[i][0] = std::make_shared<Cell>(true);
		labyrinth[i][101] = std::make_shared<Cell>(true);
	}
}

bool ReadLabyrinth(std::istream & input, Labyrinth & labyrinth, Size & size, Points & points, sf::RenderWindow & window)
{
	std::string line;
	size_t i = 0;
	int countPoint = 0;
	while (getline(input, line))
	{
		if (i > MAX_HEIGHT_LABYRINTH)
		{
			return false;
		}
		if (size.width < line.size())
		{
			if (line.size() > MAX_WIDTH_LABYRINTH)
			{
				return false;
			}
			size.width = line.size();
		}
		for (size_t j = 0; j < line.size(); ++j)
		{
			switch (line[j])
			{
				case '#':
				{
					labyrinth[i + 1][j + 1] = std::make_shared<Cell>(true);
					labyrinth[i + 1][j + 1]->rect.setFillColor(sf::Color::Black);
					break;
				}
				case ' ':
				{
					(labyrinth[i + 1][j + 1])->rect.setFillColor(sf::Color::White);
					break;
				}
				case 'C':
				{
					if (countPoint < 2)
					{
						++countPoint;
					}
					else
					{
						return false;
					}
					points.startPoint = labyrinth[i + 1][j + 1];
					labyrinth[i + 1][j + 1]->rect.setFillColor(sf::Color::Blue);
					break;
				}
				case 'F':
				{
					if (countPoint < 2)
					{
						++countPoint;
					}
					else
					{
						return false;
					}
					points.finishPoint = labyrinth[i + 1][j + 1];
					labyrinth[i + 1][j + 1]->rect.setFillColor(sf::Color::Red);
					break;
				}
				default:
				{
					return false;
				}
			}
		}
		++i;
	}
	if (countPoint != 2)
	{
		return false;
	}
	size.height = i;
	float cellSize = std::max(600.0f / size.height, 600.0f / size.width);
	for (size_t k = 1; k <= size.height; k++)
	{
		for (size_t j = 1; j <= size.width; j++)
		{
			labyrinth[k][j]->rect.setSize({cellSize , cellSize});
			labyrinth[k][j]->rect.setPosition(cellSize * (j-1), cellSize * (k-1));
		}
	}
	DrawLabyrinth(labyrinth, size, window);
	return true;
}

bool FindWay(Labyrinth & labyrinth, const Size & size, Points & points, sf::RenderWindow & window)
{
	std::vector<Cell> openList;
	while (!openList.empty() && points.finishPoint->parent == nullptr)
	{
		DrawLabyrinth(labyrinth, size, window);
	}
	return false;
}
