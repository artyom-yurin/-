#include "stdafx.h"
#include "Labyrinth.h"



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
	Heroes heroes;
	sf::RenderWindow window;
	sf::Image image;
	image.loadFromFile("images/fox.png");
	heroes.texture.loadFromImage(image);
	InitLabyrinth(labyrinth);
	if (!ReadLabyrinth(input, labyrinth, size, points, window,heroes))
	{
		std::cout << "Error: can't read labyrinth\n";
		return 1;
	}
	sf::Event event;
	FindWay(labyrinth, size, points, window, heroes);
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		DrawLabyrinth(labyrinth, size, window, heroes);
	}
    return 0;
}

