#include <iostream>
#include <conio.h>

#pragma warning(disable:4996) // POSIX устаревшее имя

// Размеры карты
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

void run();
void printMap(int * map);
void initMap(int  * map);
void move(int * map, int dx, int dy);
void update(int * map);
void changeDirection(char key);
void clearScreen();
void generateFood(int * map);

char getMapValue(int value);



// Параметры змеи
int headxpos;
int headypos;
int direction;

// Кол-во съеденых продуктов (На сколько длинная змея)
int food = 3;

// Флаг, запущена ли игра
bool running;

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
	// Инициализация карты
	initMap(map);
	running = true;
	while (running) {
		// Если клавиша нажата
		if (kbhit()) {
			// Меняем нарпавление нажатием клавиши
			changeDirection(getch());
		}
		// Обновление карты
		update(map);

		// Очистка карты
		clearScreen();

		// Отрисовка карты
		printMap(map);

		// Задержка 0.5 секунды
		_sleep(500);
	}

	// Печать текста после завершения игры
	std::cout << "\t\t!!!Game over!" << std::endl << "\t\tYour score is: " << food;

	// Задержка, чтобы консоль не закрылась мгонвенно
	std::cin.ignore();
}

// Меняет направление змеи
void changeDirection(char key) {
	/*
	W ▲
	A ◄
	D ►
	S ▼
	*/
	switch (key) {
	case 'w':
		if (direction != 2) direction = 0;
		break;
	case 'd':
		if (direction != 3) direction = 1;
		break;
	case 's':
		if (direction != 0) direction = 2;
		break;
	case 'a':
		if (direction != 1) direction = 3;
		break;
	}
}

// Перемещает голову змеи
void move(int * map, int dx, int dy) {
	// Определение новой позиции головы
	int newx = headxpos + dx;
	int newy = headypos + dy;

	// Проверка на еду в новой позиции
	if (map[newx + newy * mapwidth] == -2) {
		// Увелечение числа съеденых продуктов (длины змеи)
		food++;

		// Генерация нового продукта
		generateFood(map);
	}

	// Проверка на свободное место в новой позиции
	else if (map[newx + newy * mapwidth] != 0) {
		running = false;
	}

	// Перемещение головы в новую позицию
	headxpos = newx;
	headypos = newy;
	map[headxpos + headypos * mapwidth] = food + 1;

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
void update(int * map) {
	// Перемещение змеи в нужном направлении
	switch (direction) {
	case 0: move(map, -1, 0);
		break;
	case 1: move(map, 0, 1);
		break;
	case 2: move(map, 1, 0);
		break;
	case 3: move(map, 0, -1);
		break;
	}

	// Перемещение тела змеи
	for (int i = 0; i < size; i++) {
		if (map[i] > 0) map[i]--;
	}
}

// Иницилизация карты
void initMap(int * map)
{
	// Добавление головы змеи на карту 
	headxpos = mapwidth / 2;
	headypos = mapheight / 2;
	map[headxpos + headypos * mapwidth] = 1;

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