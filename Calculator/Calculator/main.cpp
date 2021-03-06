﻿#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
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
const int NOT_FOUND = -2;
const int ANY_BUTTONS = -1;

enum ButtonIndex
{
	DELETE_EXPRESSION = 0,
	DELETE_ONE_ELEMENT = 1,
	DELETE_ONE_SYMBOL = 2,
	TOGGLE_SIGN = 14,
	COUNT = 19,
};
const char STR_INFINITY[] = "inf";

double Calculate(const std::string &expression)
{
	double result = 0;
	if (expression.find(STR_INFINITY) == std::string::npos)
	{
		Expression *pExpression = CreateExpression(expression);
		result = CalculateExpression(pExpression);
		DisposeExpression(pExpression);
	}
	else
	{
		result = INFINITY;
	}


	return result;
}

std::string PrintExpressionResult(const std::string &expression)
{
	try
	{
		const double result = Calculate(expression);
		std::cout << "'" << expression << "' = " << result << std::endl;
		std::ostringstream output;
		output.setf(std::ios_base::fixed, std::ios_base::floatfield);
		output << std::setiosflags(std::ios_base::fixed) << result;
		// TODO: delete ZEROS
		return output.str();
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Error in '" << expression << "': " << ex.what() << std::endl;
		return "";
	}
}

void InitScreen(sf::RectangleShape & screen)
{
	screen.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	screen.setPosition(SPACE_BETWEEN_BUTTONS,SPACE_BETWEEN_BUTTONS);
	screen.setFillColor(sf::Color::White);
	screen.setOutlineThickness(1);
	screen.setOutlineColor(sf::Color(218, 218, 218, 255));
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
			currButton->button.setOutlineColor(sf::Color(218, 218, 218, 255));

			currButton->name.setCharacterSize(18);
			currButton->name.setFont(font);
			currButton->name.setFillColor(sf::Color(77, 77, 77, 255));
			currButton->name.setOrigin(currButton->name.getGlobalBounds().width / 2, currButton->name.getGlobalBounds().height / 2);
			currButton->name.setPosition(SPACE_BETWEEN_BUTTONS + i * (BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS) + BUTTON_WIDTH / 2, 2 * SPACE_BETWEEN_BUTTONS + SCREEN_HEIGHT + j * (BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS) + BUTTON_HEIGHT / 2 - SPACE_BETWEEN_BUTTONS / 2);
		}
	}
	buttons[8].name.setFillColor(sf::Color(239, 130, 13, 255));
	buttons[13].name.setFillColor(sf::Color(239, 130, 13, 255));
	buttons[18].name.setFillColor(sf::Color(239, 130, 13, 255));
	buttons[23].name.setFillColor(sf::Color(239, 130, 13, 255));
	buttons[19].name.setFillColor(sf::Color::White);
	// arrow up
	auto pos = buttons[2].name.getPosition();
	pos -= {0, SPACE_BETWEEN_BUTTONS/2};
	buttons[2].name.setPosition(pos);
	pos = buttons[18].name.getPosition();
	pos -= {0, SPACE_BETWEEN_BUTTONS/2};
	buttons[18].name.setPosition(pos);
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

void InitTextExpression(sf::Text & textExpression, const sf::Font & font)
{
	textExpression.setCharacterSize(14);
	textExpression.setFont(font);
	textExpression.setFillColor(sf::Color(239, 130, 13, 255));
}

void DeleteUnarySign(std::string & expression)
{
	if (!expression.empty())
	{
		if (expression[expression.length() - 1] == '+'
			|| expression[expression.length() - 1] == '-')
		{
			if (expression.length() == 1
				|| expression[expression.length() - 2] == '+'
				|| expression[expression.length() - 2] == '-'
				|| expression[expression.length() - 2] == '*'
				|| expression[expression.length() - 2] == '/'
				|| expression[expression.length() - 2] == '%')
			{
				expression.pop_back();
			}
		}
	}
}

void InsertUnarySign(std::string & expression, int index)
{
	auto iter = expression.begin();
	std::advance(iter, index);
	expression.insert(iter, '-');
}

struct Parser
{
	bool canInsertNumber = true;
	bool canPoint = false;
	bool canOpenBracke = true;
	bool canCloseBracke = false;
	bool canNigativeValue = false;
	bool canSign = false;
	bool canCount = false;
	bool firstNumber = false;
	bool isResult = false;
};

void ParseSymbol(std::string & symbol, Parser & parser, int & countBrack)
{
	if ("0" <= symbol && symbol <= "9")
	{
		if (parser.isResult)
		{
			parser.canInsertNumber = true;
		}
		if (!parser.canInsertNumber)
		{
			symbol = "";
		}
		else
		{
			parser.canCloseBracke = true;
			parser.canSign = true;
			parser.canNigativeValue = true;
			parser.canCount = true;
			parser.canOpenBracke = false;
			if (!parser.firstNumber)
			{
				parser.firstNumber = true;
				parser.canPoint = true;
			}
		}
	}
	else if (symbol == "+"
		|| symbol == "-"
		|| symbol == "*"
		|| symbol == "/"
		|| symbol == "%")
	{
		if (!parser.canSign)
		{
			symbol = "";
		}
		else
		{
			parser.isResult = false;
			parser.canInsertNumber = true;
			parser.canOpenBracke = true;
			parser.firstNumber = false;
			parser.canPoint = false;
			parser.canSign = false;
			parser.canCloseBracke = false;
			parser.canCount = false;
			parser.canNigativeValue = false;
		}
	}
	else if (symbol == ".")
	{
		if (!parser.canPoint)
		{
			symbol = "";
		}
		else
		{
			parser.canPoint = false;
			parser.canSign = false;
			parser.canCount = false;
		}
	}
	else if (symbol == "(")
	{
		if (parser.canOpenBracke)
		{
			countBrack++;
		}
		else
		{
			symbol = "";
		}
	}
	else if (symbol == ")")
	{
		if (parser.canCloseBracke)
		{
			if (countBrack > 0)
			{
				countBrack--;
				parser.canSign = true;
			}
			else
			{
				symbol = "";
			}
			if (countBrack == 0)
			{
				parser.canCloseBracke = false;
			}
		}
		else
		{
			symbol = "";
		}
	}
}

int DeterminationButtonIndex(const sf::Vector2i & mousePosition)
{
	sf::Vector2i position = mousePosition;
	int x = NOT_FOUND;
	int y = NOT_FOUND;
	int i = 0;
	while (x == NOT_FOUND)
	{
		if (i == COUNT_BUTTON_HORIZONTAL)
		{
			x = ANY_BUTTONS;
		}
		else
		{
			if (SPACE_BETWEEN_BUTTONS + i * (BUTTON_WIDTH + SPACE_BETWEEN_BUTTONS) <= position.x
				&& position.x <= (i + 1) * (SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH))
			{
				x = i;
			}
			++i;
		}
	}
	i = 0;
	while (y == NOT_FOUND)
	{
		if (i == COUNT_BUTTON_VERTICAL)
		{
			y = ANY_BUTTONS;
		}
		else
		{
			if (SCREEN_HEIGHT + 2 * SPACE_BETWEEN_BUTTONS + i * (BUTTON_HEIGHT + SPACE_BETWEEN_BUTTONS) <= position.y
				&& position.y <= SCREEN_HEIGHT + SPACE_BETWEEN_BUTTONS + (i + 1) * (SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH))
			{
				y = i;
			}
			++i;
		}
	}

	if (x == 4)
	{
		if ((SCREEN_HEIGHT + 3 * BUTTON_HEIGHT + COUNT_BUTTON_VERTICAL * SPACE_BETWEEN_BUTTONS) <= position.y
			&& position.y <= (SCREEN_HEIGHT + COUNT_BUTTON_VERTICAL * BUTTON_HEIGHT + (COUNT_BUTTON_VERTICAL + 1) * SPACE_BETWEEN_BUTTONS))
		{
			y = 3;
		}
	}

	if (y == 4)
	{
		if (SPACE_BETWEEN_BUTTONS <= position.x
			&& position.x <= 2 * (SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH))
		{
			x = 0;
		}
	}

	int index;

	if (x == ANY_BUTTONS || 
		y == ANY_BUTTONS)
	{
		index = ANY_BUTTONS;
	}
	else
	{
		index = x + COUNT_BUTTON_HORIZONTAL * y;
	}

	return index;
}

void ReturnDefaultColors(Button(&buttons)[SIZE])
{
	for (Button & button : buttons)
	{
		button.button.setFillColor(sf::Color::White);
	}
	buttons[19].button.setFillColor(sf::Color(239, 105, 8, 255));
}

void DeleteExpression(std::string & expression, Parser & parser)
{
	expression = "";
	parser.canInsertNumber = true;
	parser.canPoint = false;
	parser.canOpenBracke = true;
	parser.canCloseBracke = false;
	parser.canNigativeValue = false;
	parser.canSign = false;
	parser.canCount = false;
	parser.firstNumber = false;
	parser.isResult = false;
}

void DeleteElement(std::string & expression, Parser & parser)
{
	while (!expression.empty() &&
		(isdigit(expression[expression.length() - 1]) ||
			expression[expression.length() - 1] == '.'))
	{
		expression.pop_back();
	}
	parser.canInsertNumber = true;
	parser.canOpenBracke = true;
	parser.firstNumber = false;
	parser.canPoint = false;
	parser.canSign = false;
	parser.canCloseBracke = false;
	parser.canCount = false;
	parser.canNigativeValue = false;
	DeleteUnarySign(expression);
}

void DeleteSymbol(std::string & expression, Parser & parser, int & countBrack)
{
	if (!expression.empty())
	{
		char sym = expression[expression.length() - 1];
		expression.pop_back();
		DeleteUnarySign(expression);
		if (sym == '(')
		{
			countBrack--;
		}
		else if (sym == ')')
		{
			countBrack++;
		}
		else if (sym == '.')
		{
			parser.canPoint = true;
		}
		else if (sym == '+'
			|| sym == '-'
			|| sym == '*'
			|| sym == '/'
			|| sym == '%')
		{
			parser.canSign = true;
			parser.canCount = true;
		}
		else if (expression.empty()
			|| expression[expression.length() - 1] == '+'
			|| expression[expression.length() - 1] == '-'
			|| expression[expression.length() - 1] == '*'
			|| expression[expression.length() - 1] == '/'
			|| expression[expression.length() - 1] == '%'
			|| expression[expression.length() - 1] == '(')
		{
			parser.canInsertNumber = true;
			parser.canPoint = false;
			parser.canOpenBracke = true;
			parser.canCloseBracke = (!expression.empty() && expression[expression.length() - 1] == '(');
			parser.canNigativeValue = false;
			parser.canSign = false;
			parser.canCount = false;
			parser.firstNumber = false;
		}
	}
}

void MakeNegativeValue(std::string & expression, Parser & parser)
{
	if (parser.canNigativeValue)
	{
		int k = expression.length() - 1;
		while (k > 0 &&
			(isdigit(expression[k]) ||
				expression[k] == '.'))
		{
			k--;
		}
		if (expression[k] == ')')
		{
			int closeBracker = 1;
			while (closeBracker)
			{
				k--;
				if (expression[k] == ')')
				{
					closeBracker++;
				}
				else if (expression[k] == '(')
				{
					closeBracker--;
				}
			}
			k--;
		}
		if (expression[k] == '+')
		{
			if (k == 0
				|| expression[k - 1] == '+'
				|| expression[k - 1] == '-'
				|| expression[k - 1] == '*'
				|| expression[k - 1] == '/'
				|| expression[k - 1] == '%'
				|| expression[k - 1] == '(')
			{
				expression[k] = '-';
			}
			else
			{
				InsertUnarySign(expression, k + 1);
			}

		}
		else if (expression[k] == '-')
		{
			if (k == 0
				|| expression[k - 1] == '+'
				|| expression[k - 1] == '-'
				|| expression[k - 1] == '*'
				|| expression[k - 1] == '/'
				|| expression[k - 1] == '%'
				|| expression[k - 1] == '(')
			{
				expression[k] = '+';
			}
			else
			{
				InsertUnarySign(expression, k + 1);
			}
		}
		else if (expression[k] != 'f')
		{
			if (expression[k] == '*'
				|| expression[k] == '/'
				|| expression[k] == '%'
				|| expression[k] == '(')
			{
				k++;
			}
			InsertUnarySign(expression, k);
		}
		parser.isResult = false;
	}
}

void CountExpression(std::string & expression, Parser & parser, int & countBrack)
{
	if (!expression.empty() && parser.canCount)
	{
		while (countBrack)
		{
			expression += ")";
			countBrack--;
		}
		expression = PrintExpressionResult(expression);
		parser.canCloseBracke = true;
		parser.canSign = true;
		parser.canNigativeValue = true;
		parser.canCount = true;
		parser.canPoint = false;
		parser.firstNumber = false;
		parser.canInsertNumber = false;
		parser.canOpenBracke = false;
		parser.isResult = true;
	}
}

void AddSymbol(std::string & expression, Parser & parser, int & countBrack, Button * currButton)
{
	std::string sym = currButton->name.getString();
	ParseSymbol(sym, parser, countBrack);
	if (parser.isResult &&
		parser.canInsertNumber)
	{
		expression = "";
		parser.isResult = false;
	}
	expression += sym;
}

struct Application
{
	sf::RenderWindow window;
	sf::RectangleShape screen;
	Button buttons[SIZE];
	sf::Font font;
	sf::Text textExpression;
	std::string expression;
	bool isMousePressed = false;
	int countBrack = 0;
	Parser parser;

	void InitWindow()
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Calculator", sf::Style::Close, settings);
		window.setKeyRepeatEnabled(false);
	}
	void InitApplication()
	{
		InitScreen(screen);
		InitFont(font);
		InitText(buttons);
		InitButtons(buttons, font);
		InitTextExpression(textExpression, font);
	}

	void Update()
	{
		if (!isMousePressed)
		{
			ReturnDefaultColors(buttons);

			int index = DeterminationButtonIndex(sf::Mouse::getPosition(window));

			if (index != ANY_BUTTONS)
			{
				auto currButton = &buttons[index];
				currButton->button.setFillColor(sf::Color(179, 179, 179, 255));
				if (index == 19)
				{
					currButton->button.setFillColor(sf::Color(168, 91, 9, 255));
				}

				// TODO: use enum instead of magic number
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					switch (index)
					{
						case DELETE_EXPRESSION:
						{
							DeleteExpression(expression, parser);
							break;
						}
						case DELETE_ONE_ELEMENT:
						{
							DeleteElement(expression, parser);
							break;
						}
						case DELETE_ONE_SYMBOL:
						{
							DeleteSymbol(expression, parser, countBrack);
							break;
						}
						case TOGGLE_SIGN:
						{
							MakeNegativeValue(expression, parser);
							break;
						}
						case COUNT:
						{
							CountExpression(expression, parser, countBrack);
							break;
						}
						default:
						{
							if (textExpression.getGlobalBounds().width < SCREEN_WIDTH - SPACE_BETWEEN_BUTTONS)
							{
								AddSymbol(expression, parser, countBrack, currButton);
							}
							break;
						}
					}

					textExpression.setString(expression);
					textExpression.setOrigin(textExpression.getGlobalBounds().width, textExpression.getGlobalBounds().height / 2);
					textExpression.setPosition(0.5 * SPACE_BETWEEN_BUTTONS + SCREEN_WIDTH, SPACE_BETWEEN_BUTTONS + SCREEN_HEIGHT / 2);
					isMousePressed = true;
				}
			}
		}
		else
		{
			isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		}
	}

	void Draw()
	{
		window.draw(screen);
		window.draw(textExpression);
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

	return 0;
}
