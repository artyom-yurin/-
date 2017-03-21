#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>

// Размеры карты
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;
const int MAP_SIZE = MAP_WIDTH * MAP_HEIGHT;

// Размеры окна
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const float CELL_WIDTH = WINDOW_WIDTH / MAP_WIDTH;
const float CELL_HEIGHT = WINDOW_HEIGHT / MAP_HEIGHT;

// Y координаты для текста
const float Y_GAME_OVER = 250;
const float Y_SCORE = 300;
const float Y_RESTART = 350;


const int SPACE = 0;
const int WALL = -1;
const int FOOD = -2;

//  Направление
enum Direction
{
	UP_DIRECTION = 0,
	RIGHT_DIRECTION = 1,
	DOWN_DIRECTION = 2,
	LEFT_DIRECTION = 3,
};

// Параметры змеи
struct SSnake
{
	int headxpos;
	int headypos;
	Direction direction;
	bool canChangeDirection;
	// Флаг, двигается ли змея
	bool move;
	// Кол-во съеденых продуктов (На сколько длинная змея)
	int food;
};

void Run();
void InitFont(sf::Font & font);
void InitMessage(sf::Text & text, sf::Font const & font, std::string const & message, sf::Vector2f & position);
void PrintMap(int * map, sf::RenderWindow & window);
void InitMap(int  * map, SSnake & snake);
void Move(SSnake & snake, int * map, int dx, int dy);
void Update(SSnake & snake, int * map);
void ChangeDirection(sf::Event & event, SSnake & snake);
void GenerateFood(int * map);
void InitResultWindow(sf::RectangleShape & resultWindow);
sf::Color GetMapValue(int value);

int main()
{
	Run();
	return 0;
}

// Настройка шрифта
void InitFont(sf::Font & font)
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Error not find file with font" << std::endl;
		std::exit(1);
	}
}

// Настройка сообащения
void InitMessage(sf::Text & text, sf::Font const & font, std::string const & message, sf::Vector2f & position)
{
	text.setFont(font);
	text.setCharacterSize(30);
	text.setString(message);
	text.setFillColor(sf::Color::Black);
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(position);
}

// Настройка окна
void InitWindow(sf::RenderWindow & window, int width, int height)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(width, height), "Snake", sf::Style::Close, settings);
	window.setKeyRepeatEnabled(false);
}

// Обработка событий
void HandleEvents(sf::RenderWindow & window, SSnake & snake, int * map)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		// Перезапуск игры
		if (!snake.move && event.key.code == (sf::Keyboard::R))
		{
			InitMap(map, snake);
		}
		// Меняем направление по нажатию клавиши
		ChangeDirection(event, snake);
	}
}

// Главная функция игры
void Run()
{
	sf::RenderWindow window;
	InitWindow(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Карта размером (MAP_WIDTH X MAP_HEIGHT)
	int map[MAP_SIZE] = { 0 };
	// Шрифт
	sf::Font font;
	// Сообщение GAME OVER
	sf::Text gameOver;
	gameOver.setStyle(sf::Text::Bold);
	// Сообщение с очками игрока
	sf::Text score;
	// Сообщение о рестарте
	sf::Text restart;
	// Змейка
	SSnake snake;
	InitFont(font);
	// Окно для сообщений
	sf::RectangleShape resultWindow;
	InitResultWindow(resultWindow);
	sf::Vector2f position = { WINDOW_WIDTH / 2, Y_GAME_OVER };
	InitMessage(gameOver, font, "GAME OVER", position);
	position.y = Y_RESTART;
	InitMessage(restart, font, "Press R to restart", position);
	position.y = Y_SCORE;
	// Инициализация карт
	InitMap(map, snake);

	while (window.isOpen()) 
	{
		HandleEvents(window, snake, map);
		if (snake.move)
		{
			// Обновление карты
			Update(snake, map);
		}
		
		// Очистка карты
		window.clear(sf::Color::White);

		// Отрисовка карты
		PrintMap(map, window);
		if (!snake.move)
		{
			window.draw(resultWindow);
			InitMessage(score, font, "Your score " + std::to_string(snake.food), position);
			window.draw(gameOver);
			window.draw(score);
			window.draw(restart);
		}
		window.display();
		if (snake.move)
		{
			Sleep(500);
		}
	}
}

// Меняет направление змеи
void ChangeDirection(sf::Event & event, SSnake & snake)
{
	/*
	W ▲
	A ◄
	D ►
	S ▼
	*/
	if (snake.canChangeDirection)
	{
		if (event.key.code == (sf::Keyboard::Up))
		{
			if (snake.direction != DOWN_DIRECTION && snake.direction != UP_DIRECTION)
			{
				snake.direction = UP_DIRECTION;
				snake.canChangeDirection = false;
			}
		}
		if (event.key.code == (sf::Keyboard::Right))
		{
			if (snake.direction != LEFT_DIRECTION && snake.direction != RIGHT_DIRECTION)
			{
				snake.direction = RIGHT_DIRECTION;
				snake.canChangeDirection = false;
			}
		}
		if (event.key.code == (sf::Keyboard::Down))
		{
			if (snake.direction != UP_DIRECTION && snake.direction != DOWN_DIRECTION)
			{
				snake.direction = DOWN_DIRECTION;
				snake.canChangeDirection = false;
			}
		}
		if (event.key.code == (sf::Keyboard::Left))
		{
			if (snake.direction != RIGHT_DIRECTION && snake.direction != LEFT_DIRECTION)
			{
				snake.direction = LEFT_DIRECTION;
				snake.canChangeDirection = false;
			}
		}
	}
}

// Перемещает голову змеи
void Move(SSnake & snake, int * map, int dx, int dy) {
	// Определение новой позиции головы
	int newx = snake.headxpos + dx;
	int newy = snake.headypos + dy;
	// Проверка на еду в новой позиции
	if (map[newx + newy * MAP_WIDTH] == FOOD) {
		// Увелечение числа съеденых продуктов (длины змеи)
		snake.food++;

		// Генерация нового продукта
		GenerateFood(map);
	}

	// Проверка на свободное место в новой позиции
	else if (map[newx + newy * MAP_WIDTH] != SPACE) {
		snake.move = false;
	}

	// Перемещение головы в новую позицию
	snake.headxpos = newx;
	snake.headypos = newy;
	snake.canChangeDirection = true;
	map[snake.headxpos + snake.headypos * MAP_WIDTH] = snake.food + 1;
}

// Генерация новой еды
void GenerateFood(int * map) {
	int x = 0;
	int y = 0;
	do {
		srand((unsigned int)time(0));
		// Генерация координат для новой еды
		x = rand() % (MAP_WIDTH - 2) + 1;
		y = rand() % (MAP_HEIGHT - 2) + 1;

		// Если локация занята, то перегенерация
	} while (map[x + y * MAP_WIDTH] != SPACE);

	// Добавление еды на карту
	map[x + y * MAP_WIDTH] = FOOD;
}

// Обновление карты
void Update(SSnake & snake, int * map) {
	// Перемещение змеи в нужном направлении
	switch (snake.direction) {
	case UP_DIRECTION: Move(snake, map, 0, -1);
		break;
	case RIGHT_DIRECTION: Move(snake, map, 1, 0);
		break;
	case DOWN_DIRECTION: Move(snake, map, 0, 1);
		break;
	case LEFT_DIRECTION: Move(snake, map, -1, 0);
		break;
	}

	// Перемещение тела змеи
	for (int i = 0; i < MAP_SIZE; i++) {
		if (map[i] > 0) map[i]--;
	}
}

// Настройка змейки
void InitSnake(SSnake & snake)
{
	snake.headxpos = MAP_WIDTH / 2;
	snake.headypos = MAP_HEIGHT / 2;
	snake.direction = UP_DIRECTION;
	snake.canChangeDirection = true;
	snake.food = 3;
	snake.move = true;
}

// Иницилизация карты
void InitMap(int * map, SSnake & snake)
{
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		map[i] = 0;
	}
	
	InitSnake(snake);

	// Добавление головы змеи на карту
	map[snake.headxpos + snake.headypos * MAP_WIDTH] = 1;

	// Добавление верхней и нижней стенки
	for (int x = 0; x < MAP_WIDTH; ++x) {
		map[x] = WALL;
		map[x + (MAP_HEIGHT - 1) * MAP_WIDTH] = WALL;
	}

	// Добавление правой и левой стенки
	for (int y = 0; y < MAP_HEIGHT; y++) {
		map[0 + y * MAP_WIDTH] = WALL;
		map[(MAP_WIDTH - 1) + y * MAP_WIDTH] = WALL;
	}

	// Генерация первой еды
	GenerateFood(map);
}

// Печать карты в окне
void PrintMap(int * map, sf::RenderWindow & window)
{
	sf::RectangleShape block({CELL_WIDTH, CELL_HEIGHT});
	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < MAP_WIDTH; ++x) {
			block.setPosition(CELL_WIDTH * x, CELL_HEIGHT * y);
			// Печать карты по значению ячейки
			block.setFillColor(GetMapValue(map[x + y * MAP_WIDTH]));
			window.draw(block);
		};
	}
}

// Возвращает псевдографическое представление значения
sf::Color GetMapValue(int value)
{
	// Возвращает цвет тела змеи 
	if (value > 0) return sf::Color::Green;

	switch (value) {
		// Возвращает цвет стены
	case WALL: return sf::Color::Black;
		// Возвращает цвет еды
	case FOOD: return sf::Color::Red;
	}

	return sf::Color::White;
}

// Настройка окна для сообщения
void InitResultWindow(sf::RectangleShape & resultWindow)
{
	resultWindow.setSize({300, 200});
	resultWindow.setOrigin(resultWindow.getGlobalBounds().width / 2, resultWindow.getGlobalBounds().height / 2);
	resultWindow.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	resultWindow.setOutlineThickness(3);
	resultWindow.setOutlineColor(sf::Color::Black);
}
