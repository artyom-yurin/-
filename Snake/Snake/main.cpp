#include <iostream>
#include <conio.h>

#pragma warning(disable:4996) // POSIX устаревшее имя

// Размеры карты
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;
const int SIZE = MAP_WIDTH * MAP_HEIGHT;
const int SPACE = 0;
const int WALL = -1;
const int FOOD = -2;
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
	Direction direction = UP_DIRECTION;
	// Кол-во съеденых продуктов (На сколько длинная змея)
	int food = 3;
};

void run();
void printMap(int * map);
void initMap(int  * map, SSnake & snake);
bool move(SSnake & snake, int * map, int dx, int dy);
void update(SSnake & snake, int * map, bool & running);
void changeDirection(char key, SSnake & snake);
void clearScreen();
void generateFood(int * map);

char getMapValue(int value);

int main()
{
	run();
	return 0;
}

// Главная функция игры
void run()
{
	// Карта размером (MAP_WIDTH X MAP_HEIGHT)
	int map[SIZE] = { 0 };
	SSnake snake;
	// Инициализация карт
	initMap(map, snake);
	// Флаг, запущена ли игра
	bool running = true;
	while (running) {
		// Если клавиша нажата
		if (kbhit()) {
			// Меняем нарпавление нажатием клавиши
			changeDirection(getch(), snake);
		}
		// Обновление карты
		update(snake, map, running);

		// Очистка карты
		clearScreen();

		// Отрисовка карты
		printMap(map);

		// Задержка 0.5 секунды
		_sleep(500);
	}

	// Печать текста после завершения игры
	std::cout << "\t\t!!!Game over!!!" << std::endl << "\t\tYour score is: " << snake.food;

	// Задержка, чтобы консоль не закрылась мгонвенно
	std::cin.ignore();
}

// Меняет направление змеи
void changeDirection(char key, SSnake & snake) {
	/*
	W ▲
	A ◄
	D ►
	S ▼
	*/
	switch (key) {
	case 'w':
		if (snake.direction != DOWN_DIRECTION) snake.direction = UP_DIRECTION;
		break;
	case 'd':
		if (snake.direction != LEFT_DIRECTION) snake.direction = RIGHT_DIRECTION;
		break;
	case 's':
		if (snake.direction != UP_DIRECTION) snake.direction = DOWN_DIRECTION;
		break;
	case 'a':
		if (snake.direction != RIGHT_DIRECTION) snake.direction = LEFT_DIRECTION;
		break;
	}
}

// Перемещает голову змеи
bool move(SSnake & snake, int * map, int dx, int dy) {
	// Определение новой позиции головы
	int newx = snake.headxpos + dx;
	int newy = snake.headypos + dy;
	bool running = true;
	// Проверка на еду в новой позиции
	if (map[newx + newy * MAP_WIDTH] == FOOD) {
		// Увелечение числа съеденых продуктов (длины змеи)
		snake.food++;

		// Генерация нового продукта
		generateFood(map);
	}

	// Проверка на свободное место в новой позиции
	else if (map[newx + newy * MAP_WIDTH] != SPACE) {
		running = false;
	}

	// Перемещение головы в новую позицию
	snake.headxpos = newx;
	snake.headypos = newy;
	map[snake.headxpos + snake.headypos * MAP_WIDTH] = snake.food + 1;
	return running;
}

// Очистка экрана
void clearScreen() {
	system("cls");
}

// Генерация новой еды
void generateFood(int * map) {
	int x = 0;
	int y = 0;
	do {
		// Генерация координат для новой еды
		x = rand() % (MAP_WIDTH - 2) + 1;
		y = rand() % (MAP_HEIGHT - 2) + 1;

		// Если локация занята, то перегенерация
	} while (map[x + y * MAP_WIDTH] != SPACE);

	// Добавление еды на карту
	map[x + y * MAP_WIDTH] = FOOD;
}

// Обновление карты
void update(SSnake & snake, int * map, bool & running) {
	// Перемещение змеи в нужном направлении
	switch (snake.direction) {
	case UP_DIRECTION: running = move(snake, map, -1, 0);
		break;
	case RIGHT_DIRECTION: running = move(snake, map, 0, 1);
		break;
	case DOWN_DIRECTION: running = move(snake, map, 1, 0);
		break;
	case LEFT_DIRECTION: running = move(snake, map, 0, -1);
		break;
	}

	// Перемещение тела змеи
	for (int i = 0; i < SIZE; i++) {
		if (map[i] > 0) map[i]--;
	}
}

// Иницилизация карты
void initMap(int * map, SSnake & snake)
{
	// Добавление головы змеи на карту 
	snake.headxpos = MAP_WIDTH / 2;
	snake.headypos = MAP_HEIGHT / 2;
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
	generateFood(map);
}

// Печать карты в консоле
void printMap(int * map)
{
	for (int x = 0; x < MAP_WIDTH; ++x) {
		for (int y = 0; y < MAP_HEIGHT; ++y) {
			// Печать карты по значению ячейки
			std::cout << getMapValue(map[x + y * MAP_WIDTH]);
		}
		// Конец строки
		std::cout << std::endl;
	}
}

// Возвращает псевдографическое представление значения
char getMapValue(int value)
{
	// Возвращает значок тела змеи 
	if (value > 0) return 'o';

	switch (value) {
		// Возвращает значок стены
	case WALL: return 'X';
		// Возвращает значок еды
	case FOOD: return 'O';
	}

	return ' ';
}