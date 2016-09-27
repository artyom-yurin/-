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
	void InitCenterCircle(sf::CircleShape &centerCircle, sf::Vector2f &windowCenter)
	{
		centerCircle.setPointCount(100);
		centerCircle.setFillColor(sf::Color::Red);
		centerCircle.setOrigin(centerCircle.getGlobalBounds().width / 2, centerCircle.getGlobalBounds().height / 2);
		centerCircle.setPosition(windowCenter);
	}
	void InitWindow(sf::RenderWindow &window)
	{
		const int screenWidth = 800;
		const int screenHeight = 600;
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		window.create(sf::VideoMode(screenWidth, screenHeight), "SFML Analog Clock", sf::Style::Close, settings);
	}
	struct Coordinates
	{
		int x = 0;
		int y = 0;
	};
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
	void InitDot(sf::CircleShape &dot, sf::RenderWindow & window, int & position, Coordinates &point)
	{
		if (position % 5 == 0)
			dot = sf::CircleShape(3);
		else
			dot = sf::CircleShape(1);
		dot.setFillColor(sf::Color::Black);
		dot.setOrigin(dot.getGlobalBounds().width / 2, dot.getGlobalBounds().height / 2);
		dot.setPosition(point.x + window.getSize().x / 2, point.y + window.getSize().y / 2);	
	}
	void InitDots(sf::CircleShape (&dots)[60], sf::RenderWindow & window, const int &clockCircleSize)
	{
		const float PI = 3.1415927;
		float angle = 0.0;
		Coordinates point;
		for (int i = 0; i < 60; i++)
		{
			point.x = (clockCircleSize - 10) * cos(angle);
			point.y = (clockCircleSize - 10) * sin(angle);
			InitDot(dots[i], window, i, point);
			angle = angle + ((2 * PI) / 60);
		}
	}
	void InitOutlineClock(sf::CircleShape &clockCircle, sf::RenderWindow &window)
	{
		const int clockCircleThickness = 2;
		clockCircle.setPointCount(100);
		clockCircle.setOutlineThickness(clockCircleThickness);
		clockCircle.setOutlineColor(sf::Color::Black);
		clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
		clockCircle.setPosition(window.getSize().x / 2 + clockCircleThickness, window.getSize().y / 2 + clockCircleThickness);
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
	const int clockCircleSize = 250;
	

	// Create the window of the application
	sf::RenderWindow window;
	InitWindow(window);

	// Define windowCenter which gets the center of the window here, right after creating window
	sf::Vector2f windowCenter = GetWindowCenter(window);

	// Create a list for clock's dots
	sf::CircleShape dot[60];
	InitDots(dot, window, clockCircleSize);

	// Create outline of the clock
	sf::CircleShape clockCircle(clockCircleSize);
	InitOutlineClock(clockCircle, window);

	// Crate another circle for center
	sf::CircleShape centerCircle(10);
	InitCenterCircle(centerCircle, windowCenter);

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