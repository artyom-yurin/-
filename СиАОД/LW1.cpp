#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.
//
// TODO: Fix all warnings on high warning level (/W4, -Wall -Wextra).
// TODO: Rename variables and split to several functions,
// see also http://github.com/ps-group/ips-wiki/blob/master/prog_theory/coding-style/cpp-style-sfml.md

void printResult(float g, float maxTimeLift, float currentTime)
{
	float initialVelocity = g * maxTimeLift;
	float currentHeight = initialVelocity * currentTime - 0.5 * g * currentTime * currentTime;
	printf("Current Time=%f, Current Height=%f\n", currentTime, currentHeight);
}

int main(int, char *[])
{
	const float g = 9.8f;
	int maxHeight;
	printf("Max Height: ");
	if (0 == scanf("%d", &maxHeight))
	{
		printf("\n" "expected floating-point number" "\n");
		exit(1);
	}
	// maxTimeLift - time point when height is at maximum.
	// currentTime - current time point
	// v(currentTime) == v0 - g * currentTime
	// v0 = g * maxTimeLift
	// currentHeight(currentTime) == v0 * currentTime - 0.5 * g * currentTime * currentTime
	float maxTimeLift = sqrt(maxHeight * 2 / g);
	printf("Max Time Lift=%f\n", maxTimeLift);
	bool doesMaxHeight = false;
	for (float currentTime = 0; currentTime < maxTimeLift * 2; currentTime += 0.1f) //TODO: REPLACE FOR TO WHILE
	{
		if (currentTime > maxTimeLift && !doesMaxHeight)
		{
			doesMaxHeight = true;
			printResult(g, maxTimeLift, maxTimeLift);
		}
		printResult(g, maxTimeLift, currentTime);
	}

	printResult(g, maxTimeLift, maxTimeLift * 2);

	return 0;
}