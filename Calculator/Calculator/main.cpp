#include <iostream>
#include <string>
#include "Expression.h"
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 230;
const int WINDOW_HEIGHT = 290;
const float SCREEN_WIDTH = 210;
const float SCREEN_HEIGHT = 50;
const float BUTTON_WIDTH = 34;
const float BUTTON_HEIGHT = 34;
const float SPACE_BETWEEN_BUTTONS = 10;
const int COUNT_BUTTON_HORIZONTAL = 5;
const int COUNT_BUTTON_VERTICAL = 5;
const int SIZE = COUNT_BUTTON_HORIZONTAL * COUNT_BUTTON_VERTICAL;

double Calculate(const std::string &expression)
{
	Expression *pExpression = CreateExpression(expression);
	const double result = CalculateExpression(pExpression);
	DisposeExpression(pExpression);

	return result;
}

void PrintExpressionResult(const std::string &expression)
{
	try
	{
		const double result = Calculate(expression);
		std::cout << "'" << expression << "' = " << result << std::endl;
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Error in '" << expression << "': " << ex.what() << std::endl;
	}
}

void InitScreen(sf::RectangleShape & screen)
{
	screen.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	screen.setPosition(SPACE_BETWEEN_BUTTONS,SPACE_BETWEEN_BUTTONS);
	screen.setFillColor(sf::Color::White);
	screen.setOutlineThickness(1);
	screen.setOutlineColor(sf::Color::Black);
}

void InitButtons(sf::RectangleShape (&buttons)[SIZE])
{
	for (int i = 0; i < COUNT_BUTTON_HORIZONTAL; ++i)
	{
		for (int j = 0; j < COUNT_BUTTON_VERTICAL; ++j)
		{
			buttons[j * COUNT_BUTTON_HORIZONTAL + i].setSize({ BUTTON_WIDTH, BUTTON_HEIGHT });
			buttons[j * COUNT_BUTTON_HORIZONTAL + i].setPosition(SPACE_BETWEEN_BUTTONS + i * (BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS), 2 * SPACE_BETWEEN_BUTTONS + SCREEN_HEIGHT + j * (BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS));
			buttons[j * COUNT_BUTTON_HORIZONTAL + i].setOutlineThickness(1);
			buttons[j * COUNT_BUTTON_HORIZONTAL + i].setOutlineColor(sf::Color::Black);
		}
	}
	// first big button
	buttons[4] = sf::RectangleShape();
	buttons[3].setSize({ 2 * BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS , BUTTON_HEIGHT });
	// second big button
	buttons[21] = sf::RectangleShape();
	buttons[20].setSize({ 2 * BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS , BUTTON_HEIGHT });
	// third big button
	buttons[19].setSize({BUTTON_WIDTH, 2 * BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS});
	buttons[24] = sf::RectangleShape();
}

struct Application
{
	sf::RenderWindow window;
	sf::RectangleShape screen;
	sf::RectangleShape buttons[SIZE];
	void InitWindow()
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Calculator", sf::Style::Close, settings);
	}
	void InitApplication()
	{
		InitScreen(screen);
		InitButtons(buttons);
	}
	void Draw()
	{
		window.draw(screen);
		for (sf::RectangleShape button : buttons)
		{
			window.draw(button);
		}
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

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	Application app;
	app.InitWindow();
	app.InitApplication();
	while (app.window.isOpen())
	{
		app.HandleEvents();

		app.window.clear(sf::Color::White);

		app.Draw();

		app.window.display();
	}
	PrintExpressionResult("1.2");
	PrintExpressionResult("404");
	PrintExpressionResult("   404  ");
	PrintExpressionResult("3 * 3 * 3");
	PrintExpressionResult("12 / 12 / 12");
	PrintExpressionResult("25 + 17 / 45 / 2");
	PrintExpressionResult("42 + 42 / 2 * 2");
	PrintExpressionResult(" 4 * 4 + 3 * 3 + 2 * 2 ");
	PrintExpressionResult(" + 2 - - 2 - 4");
	PrintExpressionResult("-(2 - 3) * 2");
	PrintExpressionResult("(4 * 6 - 7) * 3 - 9 * (5 - (6 + 3))");
	PrintExpressionResult("(4.2 * 6 - 7.2) * 3.45 - 9.0005 * (5.1 - (6 + 3))");

	return 0;
}
