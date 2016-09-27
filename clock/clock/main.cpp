////////////////////////////////////////////////////////////
// Headers:
// ctime for getting system time and
// cmath for sin and cos functions
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>

namespace
{
	sf::Vector2f GetWindowCenter(sf::RenderWindow & window)
	{
		return sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	}
	struct Hands
	{
		sf::RectangleShape hourHand;
		sf::RectangleShape minuteHand;
		sf::RectangleShape secondsHand;
	};
	void initHand(sf::RectangleShape &hand, sf::Vector2f &size, sf::Color &color, sf::Vector2f &windowCenter)
	{
		hand.setSize(size);
		hand.setFillColor(color);
		hand.setOrigin(hand.getGlobalBounds().width / 2, hand.getGlobalBounds().height - 25);
		hand.setPosition(windowCenter);
	}
	void InitHands(Hands &clockHands, sf::Vector2f &windowCenter)
	{
		initHand(clockHands.hourHand, sf::Vector2f(5, 180), sf::Color(0, 0, 0), windowCenter);
		initHand(clockHands.minuteHand, sf::Vector2f(3, 240), sf::Color(0, 0, 0), windowCenter);
		initHand(clockHands.secondsHand, sf::Vector2f(2, 250), sf::Color(255, 0, 0), windowCenter);
	}
}
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
	// Define some variables and constants
	const int screenWidth = 800;
	const int screenHeight = 600;
	const float PI = 3.1415927;
	const int clockCircleSize = 250;
	const int clockCircleThickness = 2;
	int x, y;
	float angle = 0.0;

	// Set multisampling level
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML Analog Clock", sf::Style::Close, settings);

	// Define windowCenter which gets the center of the window here, right after creating window
	sf::Vector2f windowCenter = GetWindowCenter(window);

	// Create a list for clock's dots
	sf::CircleShape dot[60];

	// Create dots and place them to very right positions
	for (int i = 0; i<60; i++)
	{
		x = (clockCircleSize - 10) * cos(angle);
		y = (clockCircleSize - 10) * sin(angle);

		if (i % 5 == 0)
			dot[i] = sf::CircleShape(3);
		else
			dot[i] = sf::CircleShape(1);
		dot[i].setFillColor(sf::Color::Black);
		dot[i].setOrigin(dot[i].getGlobalBounds().width / 2, dot[i].getGlobalBounds().height / 2);
		dot[i].setPosition(x + window.getSize().x / 2, y + window.getSize().y / 2);

		angle = angle + ((2 * PI) / 60);
	}

	// Create outline of the clock
	sf::CircleShape clockCircle(clockCircleSize);

	clockCircle.setPointCount(100);
	clockCircle.setOutlineThickness(clockCircleThickness);
	clockCircle.setOutlineColor(sf::Color::Black);
	clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
	clockCircle.setPosition(window.getSize().x / 2 + clockCircleThickness, window.getSize().y / 2 + clockCircleThickness);

	// Crate another circle for center
	sf::CircleShape centerCircle(10);

	centerCircle.setPointCount(100);
	centerCircle.setFillColor(sf::Color::Red);
	centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2, centerCircle.getGlobalBounds().height / 2);
	centerCircle.setPosition(windowCenter);

	// Create hour, minute, and seconds hands
	Hands clockHands;

	InitHands(clockHands, windowCenter);

	// Create sound effect
	/*sf::Music clockTick;
	if (!clockTick.openFromFile("resources/clock-1.wav"))
		return EXIT_FAILURE;
	clockTick.setLoop(true);
	clockTick.play();*/

	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Get system time
		std::time_t currentTime = std::time(NULL);

		struct tm * ptm = localtime(&currentTime);

		clockHands.hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
		clockHands.minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
		clockHands.secondsHand.setRotation(ptm->tm_sec * 6);

		// Clear the window
		window.clear(sf::Color::White);

		// Draw all parts of clock
		window.draw(clockCircle);

		for (int i = 0; i<60; i++)
		{
			window.draw(dot[i]);
		}

		window.draw(clockHands.hourHand);
		window.draw(clockHands.minuteHand);
		window.draw(clockHands.secondsHand);
		window.draw(centerCircle);

		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}