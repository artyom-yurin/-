#include "stdafx.h"
#include "Labyrinth.h"

bool BuildPath(std::shared_ptr<Cell> & currCell)
{
	currCell->rect.setFillColor(sf::Color::Cyan);
	currCell = currCell->parent;

	if (currCell == nullptr)
	{
		return true;
	}

	return false;
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

void InitCell(std::shared_ptr<Cell> & currCell, int x, int y)
{
	currCell->rect.setFillColor(sf::Color::White);
	currCell->x = x;
	currCell->y = y;
}

bool ReadLabyrinth(std::istream & input, Labyrinth & labyrinth, Size & size, Points & points)
{
	std::string line;
	size_t i = 0;
	int countPoints = 0;
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
			auto currCell = labyrinth[j + 1][i + 1];
			InitCell(currCell, j + 1, i + 1);
			switch (line[j])
			{
				case '#':
				{
					currCell->close = true;
					currCell->rect.setFillColor(sf::Color::Black);
					break;
				}
				case ' ':
				{
					break;
				}
				case 'C':
				{
					if (countPoints < 2)
					{
						++countPoints;
					}
					else
					{
						return false;
					}
					points.startPoint = currCell;
					break;
				}
				case 'F':
				{
					if (countPoints < 2)
					{
						++countPoints;
					}
					else
					{
						return false;
					}
					points.finishPoint = currCell;
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
	if (countPoints != 2)
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

bool FindWay(std::vector<std::shared_ptr<Cell>> & openList, Labyrinth & labyrinth, Points & points)
{
	auto currCell = openList.back();
	currCell->close = true;
	currCell->rect.setFillColor(sf::Color::Blue);
	openList.pop_back();

	if (!labyrinth[currCell->x][currCell->y - 1]->close)
	{
		size_t newG = currCell->G + 10;
		size_t newH = h(currCell->y - 1, currCell->x, points) * 10;
		if (!labyrinth[currCell->x][currCell->y - 1]->watch)
		{
			labyrinth[currCell->x][currCell->y - 1]->F = newG + newH;
			labyrinth[currCell->x][currCell->y - 1]->G = newG;
			labyrinth[currCell->x][currCell->y - 1]->watch = true;
			labyrinth[currCell->x][currCell->y - 1]->parent = currCell;
			labyrinth[currCell->x][currCell->y - 1]->rect.setFillColor(sf::Color::Green);
			openList.push_back(labyrinth[currCell->x][currCell->y - 1]);
		}
		else if (labyrinth[currCell->x][currCell->y - 1]->F > newH + newG)
		{
			labyrinth[currCell->x][currCell->y - 1]->F = newG + newH;
		}
	}

	if (!labyrinth[currCell->x][currCell->y + 1]->close)
	{
		size_t newG = currCell->G + 10;
		size_t newH = h(currCell->y + 1, currCell->x, points) * 10;
		if (!labyrinth[currCell->x][currCell->y + 1]->watch)
		{
			labyrinth[currCell->x][currCell->y + 1]->F = newG + newH;
			labyrinth[currCell->x][currCell->y + 1]->G = newG;
			labyrinth[currCell->x][currCell->y + 1]->parent = currCell;
			labyrinth[currCell->x][currCell->y + 1]->rect.setFillColor(sf::Color::Green);
			labyrinth[currCell->x][currCell->y + 1]->watch = true;
			openList.push_back(labyrinth[currCell->x][currCell->y + 1]);
		}
		else if (labyrinth[currCell->x][currCell->y + 1]->F > newH + newG)
		{
			labyrinth[currCell->x][currCell->y + 1]->F = newG + newH;
		}
	}

	if (!labyrinth[currCell->x - 1][currCell->y]->close)
	{
		size_t newG = currCell->G + 10;
		size_t newH = h(currCell->y, currCell->x - 1, points) * 10;
		if (!labyrinth[currCell->x - 1][currCell->y]->watch)
		{
			labyrinth[currCell->x - 1][currCell->y]->F = newG + newH;
			labyrinth[currCell->x - 1][currCell->y]->G = newG;
			labyrinth[currCell->x - 1][currCell->y]->watch = true;
			labyrinth[currCell->x - 1][currCell->y]->rect.setFillColor(sf::Color::Green);
			labyrinth[currCell->x - 1][currCell->y]->parent = currCell;
			openList.push_back(labyrinth[currCell->x - 1][currCell->y]);
		}
		else if (labyrinth[currCell->x - 1][currCell->y]->F > newH + newG)
		{
			labyrinth[currCell->x - 1][currCell->y]->F = newG + newH;
		}
	}

	if (!labyrinth[currCell->x + 1][currCell->y]->close)
	{
		size_t newG = currCell->G + 10;
		size_t newH = h(currCell->y, currCell->x + 1, points) * 10;
		if (!labyrinth[currCell->x + 1][currCell->y]->watch)
		{
			labyrinth[currCell->x + 1][currCell->y]->F = newG + newH;
			labyrinth[currCell->x + 1][currCell->y]->G = newG;
			labyrinth[currCell->x + 1][currCell->y]->watch = true;
			labyrinth[currCell->x + 1][currCell->y]->parent = currCell;
			labyrinth[currCell->x + 1][currCell->y]->rect.setFillColor(sf::Color::Green);
			openList.push_back(labyrinth[currCell->x + 1][currCell->y]);
		}
		else if (labyrinth[currCell->x + 1][currCell->y]->F > newH + newG)
		{
			labyrinth[currCell->x + 1][currCell->y]->F = newG + newH;
		}			
	}

	std::sort(openList.begin(), openList.end(), [](auto a, auto b){
		return (a->F > b->F);
	});

	if (points.finishPoint->parent != nullptr || openList.empty())
	{
		return true;
	}

	return false;
}
