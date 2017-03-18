#include "stdafx.h"
#include "Labyrinth.h"

void InitWindow(sf::RenderWindow & window, int width, int height)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(width, height), "Kolobok", sf::Style::Close, settings);
}

void DrawLabyrinth(Labyrinth & labyrinth, const Size & size, sf::RenderWindow & window, Heroes & heroes)
{
	window.clear(sf::Color::White);
	for (size_t i = 1; i <= size.width; i++)
	{
		for (size_t j = 1; j <= size.height; j++)
		{
			window.draw(labyrinth[i][j]->rect);
		}
	}
	window.draw(heroes.kolobok);
	window.draw(heroes.fox);
	window.display();
}

void BuildPath(Labyrinth & labyrinth, const Size & size, Points & points, sf::RenderWindow & window, Heroes & heroes)
{
	auto currCell = points.finishPoint;
	while (currCell->parent != nullptr)
	{
		currCell->rect.setFillColor(sf::Color::Cyan);
		currCell = currCell->parent;
		DrawLabyrinth(labyrinth, size, window, heroes);
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

bool ReadLabyrinth(std::istream & input, Labyrinth & labyrinth, Size & size, Points & points, sf::RenderWindow & window, Heroes & heroes)
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
					(labyrinth[i + 1][j + 1])->x = j + 1;
					(labyrinth[i + 1][j + 1])->y = i + 1;
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
					labyrinth[i + 1][j + 1]->rect.setFillColor(sf::Color::White);
					(labyrinth[i + 1][j + 1])->x = j + 1;
					(labyrinth[i + 1][j + 1])->y = i + 1;
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
					labyrinth[i + 1][j + 1]->rect.setFillColor(sf::Color::White);
					(labyrinth[i + 1][j + 1])->x = j + 1;
					(labyrinth[i + 1][j + 1])->y = i + 1;
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
	float cellSize = min(600.0f / size.height, 600.0f / size.width);
	InitWindow(window, (int)(cellSize * size.width), (int)(cellSize * size.height));
	for (size_t k = 1; k <= size.height; k++)
	{
		for (size_t j = 1; j <= size.width; j++)
		{
			labyrinth[k][j]->rect.setSize({cellSize , cellSize});
			labyrinth[k][j]->rect.setPosition(cellSize * (j-1), cellSize * (k-1));
		}
	}
	heroes.kolobok.setRadius(cellSize / 2);
	heroes.fox.setSize({ cellSize , cellSize });
	heroes.kolobok.setPosition(cellSize * (points.startPoint->x - 1), cellSize * (points.startPoint->y - 1));
	heroes.fox.setPosition(cellSize * (points.finishPoint->x - 1), cellSize * (points.finishPoint->y - 1));
	heroes.kolobok.setFillColor(sf::Color::Yellow);
	heroes.fox.setTexture(&heroes.texture);
	DrawLabyrinth(labyrinth, size, window, heroes);
	return true;
}

size_t h(size_t y, size_t x, Points & points)
{
	return abs((int)(points.finishPoint->x - x)) + abs((int)(points.finishPoint->y - y));
}

bool FindWay(Labyrinth & labyrinth, const Size & size, Points & points, sf::RenderWindow & window, Heroes & heroes)
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
		DrawLabyrinth(labyrinth, size, window, heroes);
		std::sort(openList.begin(), openList.end(), [](auto a, auto b){
			return (a->F > b->F);
		});
		Sleep(100);
	}

	if (points.finishPoint->parent == nullptr)
	{
		return false;
	}

	return true;
}
