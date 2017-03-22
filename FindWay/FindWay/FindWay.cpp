#include "stdafx.h"
#include "Labyrinth.h"

void LoadImageFromFile(sf::Image & img, const std::string & fileName)
{
	if (!img.loadFromFile(fileName))
	{
		std::cout << "Error: Failed to load " + fileName + "\n";
		std::exit(1);
	}
}

void InitFox(sf::RectangleShape & fox, const float cellSize, const Points & points, const sf::Texture & texture)
{
	fox.setSize({ cellSize , cellSize });
	fox.setPosition(cellSize * (points.finishPoint->x - 1), cellSize * (points.finishPoint->y - 1));
	fox.setTexture(&texture);
}

void InitKolobok(sf::CircleShape & kolobok, const float cellSize, const Points & points)
{
	kolobok.setPosition(cellSize * (points.startPoint->x - 1), cellSize * (points.startPoint->y - 1));
	kolobok.setRadius(cellSize / 2);
	kolobok.setFillColor(sf::Color::Yellow);
}

void InitRectLabirinth(Labyrinth & labyrinth, const float cellSize, const Size & size)
{
	for (size_t j = 1; j <= size.height; j++)
	{
		for (size_t i = 1; i <= size.width; i++)
		{
			labyrinth[i][j]->rect.setSize({ cellSize , cellSize });
			labyrinth[i][j]->rect.setPosition(cellSize * (i - 1), cellSize * (j - 1));
		}
	}
}

struct Application
{
	sf::RenderWindow window;
	sf::Image image;
	sf::Texture texture;
	sf::RectangleShape fox;
	sf::CircleShape kolobok;
	Points points;
	Size size;
	Labyrinth labyrinth;
	bool isFoundWay;
	bool isBuildPath;
	std::vector<std::shared_ptr<Cell>> openList;

	void InitWindow(int width, int height)
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window.create(sf::VideoMode(width, height), "Kolobok", sf::Style::Close, settings);
	}

	void InitApplication(std::istream & input)
	{
		InitLabyrinth(labyrinth);
		if (!ReadLabyrinth(input, labyrinth, size, points))
		{
			std::cout << "Error: can't read labyrinth\n";
			std::exit(1);
		}
		LoadImageFromFile(image, "images/fox.png");
		texture.loadFromImage(image);

		float cellSize = min(600.0f / size.height, 600.0f / size.width);
		InitWindow((int)(cellSize * size.width), (int)(cellSize * size.height));

		InitRectLabirinth(labyrinth, cellSize, size);

		InitFox(fox, cellSize, points, texture);

		InitKolobok(kolobok, cellSize, points);
		isFoundWay = false;
		isBuildPath = false;

		points.startPoint->watch = true;
		openList.push_back(points.startPoint);
	}

	void Update()
	{
		if (!isFoundWay)
		{
			isFoundWay = FindWay(openList, labyrinth, points);
		}
		else
		{
			if (!openList.empty())
			{
				isBuildPath = BuildPath(points.finishPoint);
			}
			else
			{
				isBuildPath = true;
			}
		}
	}

	void Draw()
	{
		for (size_t j = 1; j <= size.width; j++)
		{
			for (size_t i = 1; i <= size.height; i++)
			{
				window.draw(labyrinth[i][j]->rect);
			}
		}
		window.draw(kolobok);
		window.draw(fox);
	}

	void HandleEvents()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}
};

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		std::cout << "Invalid arguments count\n"
			<< "Usage: findway.exe <file name>\n";
		return 1;
	}

	std::ifstream input(argv[1]);
	if (!input.is_open())
	{
		std::cout << "Failed to open " << argv[1] << " for reading\n";
		return 1;
	}

	Application app;

	app.InitApplication(input);

	while (app.window.isOpen())
	{
		app.HandleEvents();

		if (!app.isBuildPath)
		{
			app.Update();
		}

		app.window.clear(sf::Color::White);

		app.Draw();

		app.window.display();

		if (!app.isBuildPath)
		{
			Sleep(500);
		}
	}
    return 0;
}

