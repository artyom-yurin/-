#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.

const float G = 9.8f;
const float STEP = 0.1f;

void PrintResult(const float g, const float maxTimeLift, const float currentTime)
{
	const float initialVelocity = g * maxTimeLift;
	const float currentHeight = initialVelocity * currentTime - 0.5 * g * currentTime * currentTime;
	printf("Current Time=%f, Current Height=%f\n", currentTime, currentHeight);
}

int SafeScanf()
{
	int buffer = 0;
	if (0 == scanf("%d", &buffer))
	{
		printf("\n" "expected floating-point number" "\n");
		exit(1);
	}
	return buffer;
}

float CalculateMaxTimeLift(const int maxHeight)
{
	return sqrt(2 * maxHeight / G);
}

int main(int, char *[])
{
	printf("Max Height: ");
	const int maxHeight = SafeScanf();
	const float maxTimeLift = CalculateMaxTimeLift(maxHeight);
	printf("Max Time Lift=%f\n", maxTimeLift);
	bool doesMaxHeight = false;
	float currentTime = 0;
	while (currentTime < maxTimeLift * 2)
	{
		if (currentTime > maxTimeLift && !doesMaxHeight)
		{
			doesMaxHeight = true;
			PrintResult(G, maxTimeLift, maxTimeLift);
		}
		PrintResult(G, maxTimeLift, currentTime);
		currentTime += STEP;
	}

	PrintResult(G, maxTimeLift, maxTimeLift * 2);

	return 0;
}