#include "stdafx.h"

void InitWindow(sf::RenderWindow & window)
{
	const int screenWidth = 800;
	const int screenHeight = 600;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(screenWidth, screenHeight), "SFML Animation", sf::Style::Close, settings);
}

void InitElements(sf::RectangleShape (&elements)[8])
{
	float width = 50;

	sf::Vector2f size{width, width};
	for(size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		elements[i].setSize(size);
		elements[i].setFillColor(sf::Color(0, 0, 128, 255));
		elements[i].setOrigin(elements[i].getGlobalBounds().width / 2, elements[i].getGlobalBounds().height / 2);
		sf::Vector2f position{100.f, (float) 0.5 * width + 10 + (width + 10) * i };
		elements[i].setPosition(position);
	}
}

void FirstAnimation(sf::RectangleShape(&elements)[8])
{
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		position.x += 0.4f;
		elements[i].setPosition(position);
	}
}

void SecondAnimation(sf::RectangleShape(&elements)[8])
{
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		sf::Vector2f size = elements[i].getSize();
		position.y += 0.2f;
		size.y -= 0.1f;
		elements[i].setPosition(position);
		elements[i].setSize(size);
	}
}

void ThirdAnimation(sf::RectangleShape(&elements)[8])
{
	float correctY = elements[7].getPosition().y;
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		if (position.y < correctY)
		{
			position.y += 0.1f * (8 - i);
			if (position.y > correctY)
			{
				position.y = correctY;
			}
			position.x -= 0.1f * (8 - i);
			elements[i].setPosition(position);
		}

	}
}

void FourthAnimation(sf::RectangleShape(&elements)[8])
{
	float correctX = elements[0].getPosition().x;
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		position.y -= 0.1f * (7 - i);
		if (position.x > correctX)
		{
			position.x -= 0.1f * i;
			if (position.x < correctX)
			{
				position.x = correctX;
			}
		}
		elements[i].setPosition(position);
	}
}

void FivethAnimation(sf::RectangleShape(&elements)[8])
{
	float correctX = 100;
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		float correctY = (float) 0.5 * 50 + 10 + (50 + 10) * i;
		sf::Vector2f position = elements[i].getPosition();
		sf::Vector2f size = elements[i].getSize();
		if (position.x > correctX)
		{
			position.x -= 0.4f;
			if (position.x < correctX)
			{
				position.x = correctX;
			}
		}
		if (position.y > correctY)
		{
			position.y -= 0.2f;
			if (position.y < correctY)
			{
				position.y = correctY;
			}
		}
		if (elements[i].getGlobalBounds().height < 50)
		{
			size.y += 0.1f;
			if (elements[i].getGlobalBounds().height > 50)
			{
				size.y = 50;
			}
		}
		elements[i].setPosition(position);
		elements[i].setSize(size);
	}
}

struct Application
{
	sf::RenderWindow window;
	sf::RectangleShape elements[8];
	sf::Event event;
	int animation = 1;

	void InitApplication()
	{
		InitWindow(window);
		InitElements(elements);
	}

	void Update()
	{
		switch (animation)
		{
			case 1:
			{
				FirstAnimation(elements);
				if (elements[1].getPosition().x > 700)
				{
					++animation;
				}
				break;
			}
			case 2:
			{
				SecondAnimation(elements);
				if (elements[7].getPosition().y > 550)
				{
					++animation;
				}
				break;
			}
			case 3:
			{
				ThirdAnimation(elements);
				if (elements[0].getPosition().y == elements[7].getPosition().y)
				{
					++animation;
				}
				break;
			}
			case 4:
			{
				FourthAnimation(elements);
				if (elements[0].getPosition().x == elements[7].getPosition().x)
				{
					++animation;
				}
				break;
			}
			case 5:
			{
				FivethAnimation(elements);
				if (elements[0].getPosition().x == 100 && elements[0].getPosition().y == 0.5 * 50 + 10)
				{
					++animation;
				}
				break;
			}
			default:
			{
				animation = 1;
				break;
			}
		}
	}

	void Draw()
	{
		for (auto &item : elements)
		{
			window.draw(item);
		}
	}
	void HandleEvents()
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
};

int main()
{
	Application app;
	
	app.InitApplication();

	while (app.window.isOpen())
	{
		app.HandleEvents();

		app.Update();

		app.window.clear(sf::Color::White);

		app.Draw();

		app.window.display();
	}

    return 0;
}

