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
		centerCircle.setRadius(10);
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

	void InitOutlineClock(sf::CircleShape &clockCircle, sf::RenderWindow &window, const int & clockCircleSize)
	{
		const int clockCircleThickness = 2;
		clockCircle.setRadius(clockCircleSize);
		clockCircle.setPointCount(100);
		clockCircle.setOutlineThickness(clockCircleThickness);
		clockCircle.setOutlineColor(sf::Color::Black);
		clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
		clockCircle.setPosition(window.getSize().x / 2 + clockCircleThickness, window.getSize().y / 2 + clockCircleThickness);
	}

	auto GetSystemTime()
	{
		std::time_t currentTime = std::time(NULL);

		struct tm * ptm = localtime(&currentTime);

		return ptm;
	}

	struct Application
	{
		sf::RenderWindow window;
		sf::Vector2f windowCenter = GetWindowCenter(window);
		sf::CircleShape dot[60];
		sf::CircleShape clockCircle;
		sf::CircleShape centerCircle;
		Hands clockHands;
		void InitApplication()
		{
			InitWindow(window);
			sf::Vector2f windowCenter = GetWindowCenter(window);
			const int clockCircleSize = 250;
			InitDots(dot, window, clockCircleSize);
			InitOutlineClock(clockCircle, window, clockCircleSize);
			InitCenterCircle(centerCircle, windowCenter);
			InitHands(clockHands, windowCenter);
		}
		void Update()
		{
			auto ptm = GetSystemTime();

			clockHands.hourHand.setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
			clockHands.minuteHand.setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
			clockHands.secondsHand.setRotation(ptm->tm_sec * 6);
		}
	};
}
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
	Application app;
	app.InitApplication();

	// Create sound effect
	/*sf::Music clockTick;
	if (!clockTick.openFromFile("resources/clock-1.wav"))
		return EXIT_FAILURE;
	clockTick.setLoop(true);
	clockTick.play();*/

	while (app.window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (app.window.pollEvent(event))
		{
			// Window closed: exit
			if (event.type == sf::Event::Closed)
				app.window.close();
		}

		app.Update();

		// Clear the window
		app.window.clear(sf::Color::White);

		// Draw all parts of clock
		app.window.draw(app.clockCircle);

		for (int i = 0; i<60; i++)
		{
			app.window.draw(app.dot[i]);
		}

		app.window.draw(app.clockHands.hourHand);
		app.window.draw(app.clockHands.minuteHand);
		app.window.draw(app.clockHands.secondsHand);
		app.window.draw(app.centerCircle);

		// Display things on screen
		app.window.display();
	}

	return EXIT_SUCCESS;
}