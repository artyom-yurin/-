#include "stdafx.h"
#include "Labyrinth.h"

void BuildPath(Labyrinth & labyrinth, const Size & size, Points & points, sf::RenderWindow & window)
{
	auto currCell = points.finishPoint;
	while (currCell->parent != nullptr)
	{
		currCell->rect.setFillColor(sf::Color::Cyan);
		currCell = currCell->parent;
	}
	currCell->rect.setFillColor(sf::Color::Cyan);
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

bool ReadLabyrinth(std::istream & input, Labyrinth & labyrinth, Size & size, Points & points)
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
			labyrinth[j + 1][i + 1]->rect.setFillColor(sf::Color::White);
			switch (line[j])
			{
				case '#':
				{
					labyrinth[j + 1][i + 1] = std::make_shared<Cell>(true);
					labyrinth[j + 1][i + 1]->rect.setFillColor(sf::Color::Black);
					break;
				}
				case ' ':
				{
					labyrinth[j + 1][i + 1]->x = j + 1;
					labyrinth[j + 1][i + 1]->y = i + 1;
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
					points.startPoint = labyrinth[j + 1][i + 1];
					(labyrinth[j + 1][i + 1])->x = j + 1;
					(labyrinth[j + 1][i + 1])->y = i + 1;
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
					points.finishPoint = labyrinth[j + 1][i + 1];
					labyrinth[j + 1][i + 1]->x = j + 1;
					labyrinth[j + 1][i + 1]->y = i + 1;
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
	return true;
}

size_t h(size_t y, size_t x, Points & points)
{
	return abs((int)(points.finishPoint->x - x)) + abs((int)(points.finishPoint->y - y));
}

bool FindWay(Labyrinth & labyrinth, const Size & size, Points & points, sf::RenderWindow & window)
{
	std::vector<std::shared_ptr<Cell>> openList;
	points.startPoint->watch = true;
	openList.push_back(points.startPoint);
	while (!openList.empty() && points.finishPoint->parent == nullptr)
	{
		auto currCell = openList.back();
		currCell->close = true;
		currCell->rect.setFillColor(sf::Color::Blue);
		openList.pop_back();
		if (!labyrinth[currCell->y - 1][currCell->x]->close)
		{
			size_t newG = currCell->G + 10;
			size_t newH = h(currCell->y - 1, currCell->x, points) * 10;
			if (!labyrinth[currCell->y - 1][currCell->x]->watch)
			{
				labyrinth[currCell->y - 1][currCell->x]->F = newG + newH;
				labyrinth[currCell->y - 1][currCell->x]->G = newG;
				labyrinth[currCell->y - 1][currCell->x]->watch = true;
				labyrinth[currCell->y - 1][currCell->x]->parent = currCell;
				labyrinth[currCell->y - 1][currCell->x]->rect.setFillColor(sf::Color::Green);
				openList.push_back(labyrinth[currCell->y - 1][currCell->x]);
			}
			else if (labyrinth[currCell->y - 1][currCell->x]->F > newH + newG)
			{
				labyrinth[currCell->y - 1][currCell->x]->F = newG + newH;
			}
		}
		if (!labyrinth[currCell->y + 1][currCell->x]->close)
		{
			size_t newG = currCell->G + 10;
			size_t newH = h(currCell->y + 1, currCell->x, points) * 10;
			if (!labyrinth[currCell->y + 1][currCell->x]->watch)
			{
				labyrinth[currCell->y + 1][currCell->x]->F = newG + newH;
				labyrinth[currCell->y + 1][currCell->x]->G = newG;
				labyrinth[currCell->y + 1][currCell->x]->parent = currCell;
				labyrinth[currCell->y + 1][currCell->x]->rect.setFillColor(sf::Color::Green);
				labyrinth[currCell->y + 1][currCell->x]->watch = true;
				openList.push_back(labyrinth[currCell->y + 1][currCell->x]);
			}
			else if (labyrinth[currCell->y + 1][currCell->x]->F > newH + newG)
			{
				labyrinth[currCell->y + 1][currCell->x]->F = newG + newH;
			}
		}
		if (!labyrinth[currCell->y][currCell->x - 1]->close)
		{
			size_t newG = currCell->G + 10;
			size_t newH = h(currCell->y, currCell->x - 1, points) * 10;
			if (!labyrinth[currCell->y][currCell->x - 1]->watch)
			{
				labyrinth[currCell->y][currCell->x - 1]->F = newG + newH;
				labyrinth[currCell->y][currCell->x - 1]->G = newG;
				labyrinth[currCell->y][currCell->x - 1]->watch = true;
				labyrinth[currCell->y][currCell->x - 1]->rect.setFillColor(sf::Color::Green);
				labyrinth[currCell->y][currCell->x - 1]->parent = currCell;
				openList.push_back(labyrinth[currCell->y][currCell->x - 1]);
			}
			else if (labyrinth[currCell->y][currCell->x - 1]->F > newH + newG)
			{
				labyrinth[currCell->y][currCell->x - 1]->F = newG + newH;
			}
		}
		if (!labyrinth[currCell->y][currCell->x + 1]->close)
		{
			size_t newG = currCell->G + 10;
			size_t newH = h(currCell->y, currCell->x + 1, points) * 10;
			if (!labyrinth[currCell->y][currCell->x + 1]->watch)
			{
				labyrinth[currCell->y][currCell->x + 1]->F = newG + newH;
				labyrinth[currCell->y][currCell->x + 1]->G = newG;
				labyrinth[currCell->y][currCell->x + 1]->watch = true;
				labyrinth[currCell->y][currCell->x + 1]->parent = currCell;
				labyrinth[currCell->y][currCell->x + 1]->rect.setFillColor(sf::Color::Green);
				openList.push_back(labyrinth[currCell->y][currCell->x + 1]);
			}
			else if (labyrinth[currCell->y][currCell->x + 1]->F > newH + newG)
			{
				labyrinth[currCell->y][currCell->x + 1]->F = newG + newH;
			}			
		}
		std::sort(openList.begin(), openList.end(), [](auto a, auto b){
			return (a->F > b->F);
		});
	}

	if (points.finishPoint->parent == nullptr)
	{
		return false;
	}

	return true;
}
