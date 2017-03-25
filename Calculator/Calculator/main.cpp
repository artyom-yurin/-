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

struct Button
{
	sf::Text name;
	sf::RectangleShape button;
};

void InitText(Button (&buttons)[SIZE])
{
	buttons[0].name.setString("C");
	buttons[1].name.setString("CE");
	buttons[2].name.setString(L"←");
	buttons[3].name.setString("(");
	buttons[4].name.setString(")");
	buttons[5].name.setString("7");
	buttons[6].name.setString("8");
	buttons[7].name.setString("9");
	buttons[8].name.setString("/");
	buttons[9].name.setString("%");
	buttons[10].name.setString("4");
	buttons[11].name.setString("5");
	buttons[12].name.setString("6");
	buttons[13].name.setString("*");
	buttons[14].name.setString("±");
	buttons[15].name.setString("1");
	buttons[16].name.setString("2");
	buttons[17].name.setString("3");
	buttons[18].name.setString("-");
	buttons[19].name.setString("=");
	buttons[20].name.setString("0");
	buttons[22].name.setString(".");
	buttons[23].name.setString("+");
}

void InitButtons(Button (&buttons)[SIZE], sf::Font & font)
{
	for (int i = 0; i < COUNT_BUTTON_HORIZONTAL; ++i)
	{
		for (int j = 0; j < COUNT_BUTTON_VERTICAL; ++j)
		{
			auto currButton = &buttons[j * COUNT_BUTTON_HORIZONTAL + i];
			currButton->button.setSize({ BUTTON_WIDTH, BUTTON_HEIGHT });
			currButton->button.setPosition(SPACE_BETWEEN_BUTTONS + i * (BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS), 2 * SPACE_BETWEEN_BUTTONS + SCREEN_HEIGHT + j * (BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS));
			currButton->button.setOutlineThickness(1);
			currButton->button.setOutlineColor(sf::Color::Black);

			currButton->name.setCharacterSize(18);
			currButton->name.setFont(font);
			currButton->name.setFillColor(sf::Color::Black);
			currButton->name.setOrigin(currButton->name.getGlobalBounds().width / 2, currButton->name.getGlobalBounds().height / 2);
			currButton->name.setPosition(SPACE_BETWEEN_BUTTONS + i * (BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS) + BUTTON_WIDTH / 2, 2 * SPACE_BETWEEN_BUTTONS + SCREEN_HEIGHT + j * (BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS) + BUTTON_HEIGHT / 2 - SPACE_BETWEEN_BUTTONS / 2);
		}
	}
	// arrow up
	auto pos = buttons[2].name.getPosition();
	pos -= {0, SPACE_BETWEEN_BUTTONS/2};
	buttons[2].name.setPosition(pos);
	// first big button
	buttons[21].button = sf::RectangleShape();
	buttons[20].button.setSize({ 2 * BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS , BUTTON_HEIGHT });
	buttons[20].name.setOrigin(buttons[20].name.getGlobalBounds().width / 2, buttons[20].name.getGlobalBounds().height / 2);
	buttons[20].name.setPosition(1.5 * SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH, 2 * SPACE_BETWEEN_BUTTONS + SCREEN_HEIGHT + 4 * (BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS) + BUTTON_HEIGHT / 2 - SPACE_BETWEEN_BUTTONS / 2);
	// second big button
	buttons[19].button.setSize({BUTTON_WIDTH, 2 * BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS});
	buttons[24].button = sf::RectangleShape();
	buttons[19].name.setOrigin(buttons[19].name.getGlobalBounds().width / 2, buttons[19].name.getGlobalBounds().height / 2);
	buttons[19].name.setPosition(SPACE_BETWEEN_BUTTONS + 4 * (BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS) + BUTTON_WIDTH / 2, 2 * SPACE_BETWEEN_BUTTONS + SCREEN_HEIGHT + 3 * (BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS) + BUTTON_HEIGHT - SPACE_BETWEEN_BUTTONS / 2);
}

void InitFont(sf::Font & font)
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Error not find file with font" << std::endl;
		std::exit(1);
	}
}

struct Application
{
	sf::RenderWindow window;
	sf::RectangleShape screen;
	Button buttons[SIZE];
	sf::Font font;

	void InitWindow()
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Calculator", sf::Style::Close, settings);
	}
	void InitApplication()
	{
		InitScreen(screen);
		InitFont(font);
		InitText(buttons);
		InitButtons(buttons, font);
	}

	void Update()
	{
		std::string str = buttons[0].name.getString();
	}

	void Draw()
	{
		window.draw(screen);
		for (Button & button : buttons)
		{
			window.draw(button.button);
			window.draw(button.name);
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

		app.Update();

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
