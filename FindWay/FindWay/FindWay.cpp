#include "stdafx.h"
#include "Labyrinth.h"

void InitWindow(sf::RenderWindow & window)
{
	const int screenWidth = 600;
	const int screenHeight = 600;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(screenWidth, screenHeight), "Kolobok", sf::Style::Close, settings);
}

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

	Labyrinth labyrinth;

	Size size;
	Points points;

	sf::RenderWindow window;
	InitWindow(window);
	InitLabyrinth(labyrinth);
	if (!ReadLabyrinth(input, labyrinth, size, points, window))
	{
		std::cout << "Error: can't read labyrinth\n";
		return 1;
	}
	sf::Event event;
	FindWay(labyrinth, size, points, window);
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
    return 0;
}

