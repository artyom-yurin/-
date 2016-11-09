#include "stdafx.h"

const int MAX_INDEX_ELEMENT = 9;

typedef void (*pfn)(sf::RectangleShape (&elements)[MAX_INDEX_ELEMENT]);

void InitWindow(sf::RenderWindow & window)
{
	const int screenWidth = 800;
	const int screenHeight = 600;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(screenWidth, screenHeight), "SFML Animation", sf::Style::Close, settings);
}

void InitElements(sf::RectangleShape (&elements)[MAX_INDEX_ELEMENT])
{
	float width = 600 / (2 * MAX_INDEX_ELEMENT);

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

void FirstAnimation(sf::RectangleShape(&elements)[MAX_INDEX_ELEMENT])
{
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		position.x += 0.4f;
		elements[i].setPosition(position);
	}
}

void SecondAnimation(sf::RectangleShape(&elements)[MAX_INDEX_ELEMENT])
{

	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		sf::Vector2f size = elements[i].getSize();
		position.y += 0.4f;
		if (size.y > (600 / (2 * MAX_INDEX_ELEMENT)) / 2)
		{
			size.y -= 0.1f;
			if (size.y < (600 / (2 * MAX_INDEX_ELEMENT)) / 2)
			{
				size.y = (600 / (2 * MAX_INDEX_ELEMENT)) / 2;
			}
		}
		elements[i].setPosition(position);
		elements[i].setSize(size);
	}
}

void ThirdAnimation(sf::RectangleShape(&elements)[MAX_INDEX_ELEMENT])
{
	float correctY = elements[MAX_INDEX_ELEMENT - 1].getPosition().y;
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		if (position.y < correctY)
		{
			position.y += 0.1f * (MAX_INDEX_ELEMENT - i);
			if (position.y > correctY)
			{
				position.y = correctY;
			}
			position.x -= 0.1f * (MAX_INDEX_ELEMENT - i);
			elements[i].setPosition(position);
		}

	}
}

void FourthAnimation(sf::RectangleShape(&elements)[MAX_INDEX_ELEMENT])
{
	float correctX = elements[0].getPosition().x;
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		sf::Vector2f position = elements[i].getPosition();
		position.y -= 0.1f * (MAX_INDEX_ELEMENT - 1 - i);
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

void FivethAnimation(sf::RectangleShape(&elements)[MAX_INDEX_ELEMENT])
{
	float correctX = 100;
	float width = (600 / (2 * MAX_INDEX_ELEMENT));
	for (size_t i = 0; i < sizeof(elements) / sizeof(*elements); ++i)
	{
		float correctY = (float) 0.5 * width + 10 + (width + 10) * i;
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
			position.y -= 0.4f;
			if (position.y < correctY)
			{
				position.y = correctY;
			}
		}
		if (size.y < width)
		{
			size.y += 0.1f;
			if (size.y > width)
			{
				size.y = width;
			}
		}
		elements[i].setPosition(position);
		elements[i].setSize(size);
	}
}

void InitAnimation(pfn(&animations)[5])
{
	animations[0] = FirstAnimation;
	animations[1] = SecondAnimation;
	animations[2] = ThirdAnimation;
	animations[3] = FourthAnimation;
	animations[4] = FivethAnimation;
}

void SwitchAnimation(const sf::RectangleShape (&elements)[MAX_INDEX_ELEMENT], int & animation)
{
	if (animation == 1 && elements[1].getPosition().x > 700)
	{
		++animation;
	}
	else if (animation == 2 && elements[MAX_INDEX_ELEMENT - 1].getPosition().y > 550)
	{
		++animation;
	}
	else if (animation == 3 && elements[0].getPosition().y == elements[MAX_INDEX_ELEMENT - 1].getPosition().y)
	{
		++animation;
	}
	else if (animation == 4 && elements[0].getPosition().x == elements[MAX_INDEX_ELEMENT - 1].getPosition().x)
	{
		++animation;
	}
	else if (animation == 5 && elements[0].getPosition().x == 100 && elements[0].getPosition().y == 0.5 * (600 / (2 * MAX_INDEX_ELEMENT)) + 10)
	{
		animation = 1;
	}

}

struct Application
{
	sf::RenderWindow window;
	sf::RectangleShape elements[MAX_INDEX_ELEMENT];
	sf::Event event;
	pfn animations[5];
	int animation = 1;

	void InitApplication()
	{
		InitWindow(window);
		InitElements(elements);
		InitAnimation(animations);
	}

	void Update()
	{
		animations[animation - 1](elements);
		SwitchAnimation(elements, animation);
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

