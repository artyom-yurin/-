#include <iostream>
#include <conio.h>

#pragma warning(disable:4996) // POSIX устаревшее имя

// Размеры карты
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

// Параметры змеи
struct SSnake
{
	int headxpos;
	int headypos;
	int direction = 0;
	// Кол-во съеденых продуктов (На сколько длинная змея)
	int food = 3;
};

void run();
void printMap(int * map);
void initMap(int  * map, SSnake & snake);
bool move(SSnake & snake, int * map, int dx, int dy);
void update(SSnake & snake, int * map, bool running);
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
	// Карта размером (mapwidth X mapheight)
	int map[size] = { 0 };
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
	std::cout << "\t\t!!!Game over!" << std::endl << "\t\tYour score is: " << snake.food;

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
		if (snake.direction != 2) snake.direction = 0;
		break;
	case 'd':
		if (snake.direction != 3) snake.direction = 1;
		break;
	case 's':
		if (snake.direction != 0) snake.direction = 2;
		break;
	case 'a':
		if (snake.direction != 1) snake.direction = 3;
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
	if (map[newx + newy * mapwidth] == -2) {
		// Увелечение числа съеденых продуктов (длины змеи)
		snake.food++;

		// Генерация нового продукта
		generateFood(map);
	}

	// Проверка на свободное место в новой позиции
	else if (map[newx + newy * mapwidth] != 0) {
		running = false;
	}

	// Перемещение головы в новую позицию
	snake.headxpos = newx;
	snake.headypos = newy;
	map[snake.headxpos + snake.headypos * mapwidth] = snake.food + 1;
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
		x = rand() % (mapwidth - 2) + 1;
		y = rand() % (mapheight - 2) + 1;

		// Если локация занята, то перегенерация
	} while (map[x + y * mapwidth] != 0);

	// Добавление еды на карту
	map[x + y * mapwidth] = -2;
}

// Обновление карты
void update(SSnake & snake, int * map, bool running) {
	// Перемещение змеи в нужном направлении
	switch (snake.direction) {
	case 0: running = move(snake, map, -1, 0);
		break;
	case 1: running = move(snake, map, 0, 1);
		break;
	case 2: running = move(snake, map, 1, 0);
		break;
	case 3: running = move(snake, map, 0, -1);
		break;
	}

	// Перемещение тела змеи
	for (int i = 0; i < size; i++) {
		if (map[i] > 0) map[i]--;
	}
}

// Иницилизация карты
void initMap(int * map, SSnake & snake)
{
	// Добавление головы змеи на карту 
	snake.headxpos = mapwidth / 2;
	snake.headypos = mapheight / 2;
	map[snake.headxpos + snake.headypos * mapwidth] = 1;

	// Добавление верхней и нижней стенки
	for (int x = 0; x < mapwidth; ++x) {
		map[x] = -1;
		map[x + (mapheight - 1) * mapwidth] = -1;
	}

	// Добавление правой и левой стенки
	for (int y = 0; y < mapheight; y++) {
		map[0 + y * mapwidth] = -1;
		map[(mapwidth - 1) + y * mapwidth] = -1;
	}

	// Генерация первой еды
	generateFood(map);
}

// Печать карты в консоле
void printMap(int * map)
{
	for (int x = 0; x < mapwidth; ++x) {
		for (int y = 0; y < mapheight; ++y) {
			// Печать карты по значению ячейки
			std::cout << getMapValue(map[x + y * mapwidth]);
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
	case -1: return 'X';
		// Возвращает значок еды
	case -2: return 'O';
	}

	return ' ';
}