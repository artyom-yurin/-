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

size_t HeuristicFunction(size_t y, size_t x, const Points & points)
{
	return abs((int)(points.finishPoint->x - x)) + abs((int)(points.finishPoint->y - y));
}



void EvaluationCell(std::shared_ptr<Cell> & currCell, std::shared_ptr<Cell> & parent, size_t x, size_t y, const Points & points, std::vector<std::shared_ptr<Cell>> & openList)
{
	if (!currCell->close)
	{
		size_t newPriceFromStart = parent->priceFromStart + 10;
		size_t newHeuristicResult = HeuristicFunction(y, x, points) * 10;
		if (!currCell->watch)
		{
			currCell->priceCell = newPriceFromStart + newHeuristicResult;
			currCell->priceFromStart = newPriceFromStart;
			currCell->watch = true;
			currCell->parent = parent;
			currCell->rect.setFillColor(sf::Color::Green);
			openList.push_back(currCell);
		}
		else if (currCell->priceCell > newHeuristicResult + newPriceFromStart)
		{
			currCell->priceCell = newPriceFromStart + newHeuristicResult;
			currCell->priceFromStart = newPriceFromStart;
		}
	}
}

bool FindWay(std::vector<std::shared_ptr<Cell>> & openList, Labyrinth & labyrinth, Points & points)
{
	auto currCell = openList.back();
	currCell->close = true;
	currCell->rect.setFillColor(sf::Color::Blue);
	openList.pop_back();
	size_t x = currCell->x;
	size_t y = currCell->y;

	EvaluationCell(labyrinth[x][y - 1], currCell, x, y - 1, points, openList);

	EvaluationCell(labyrinth[x][y + 1], currCell, x, y + 1, points, openList);
	
	EvaluationCell(labyrinth[x - 1][y], currCell, x - 1, y, points, openList);

	EvaluationCell(labyrinth[x + 1][y], currCell, x + 1, y, points, openList);

	std::sort(openList.begin(), openList.end(), [](auto a, auto b){
		return (a->priceCell > b->priceCell);
	});

	if (points.finishPoint->parent != nullptr || openList.empty())
	{
		return true;
	}

	return false;
}
